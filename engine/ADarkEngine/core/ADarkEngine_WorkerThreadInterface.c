#include <stdio.h>

#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.h"

inline void
BeginTicketMutex(OS_call* call, ticket_mutex* ticketMutex)
{
    u64 ticket = AtomicAddU64(&ticketMutex->ticketServed, 1);
    while(ticket != ticketMutex->ticketServing)
    {
        //call->ThreadSleep(8.0f);
    }
}

inline void
EndTicketMutex(ticket_mutex* ticketMutex)
{
    AtomicAddU64(&ticketMutex->ticketServing, 1);
}

// TODO(winston): sync loop with main thread
internal u32
WT_ProcessWorkQueue(void* temp)
{
    worker_thread_queue* queue = (worker_thread_queue*)temp;
    OS_call os = queue->osCall;
    
    f32 lastTime = os.GetTime_MS();
    
    while(!queue->isRunning)
    {
        
        for(worker_thread_queue_member* member = (queue->head)->next;
            member != queue->tail;
            member = member->next)
        {
            BeginTicketMutex(&queue->osCall, &member->queueMemberMutex);
            
#if 0
            FUNCTION_CHECK:
            if(member->function)
            {
                (*member->function)(member->parameter);
            }
            else
            {
                goto FUNCTION_CHECK;
            }
#else
            if(member->function)
            {
                (*member->function)(member->parameter);
            }
#endif
            member->function = 0;
            member->parameter = 0;
            
            --queue->numberOfJobsInQueue;
            
            EndTicketMutex(&member->queueMemberMutex);
        }
        
        f32 currentTime = os.GetTime_MS();
        f32 timeElapsed = currentTime - lastTime;
        
        if(timeElapsed <= 16.667f)
        {
            f32 timeToSleep = 16.667f - timeElapsed;
            os.ThreadSleep(timeToSleep);
        }
        
        lastTime = currentTime;
    }
    
    char buffer[64] = {0};
    
    // TODO(winston): check to see if _snprintf_s is available on POSIX systems
    // NOTE(winston): _snprintf_s is NOT available on POSIX based systems
    //_snprintf_s(buffer, 64, 64, "Queue Nodes Created: %d", queue->queueNodesCreated);
    
    snprintf(buffer, 64, "Queue Nodes Created: %d", queue->queueNodesCreated);
    DE_WriteFile("../data/worker_thread.txt", buffer);
    
    return 0;
}

internal worker_thread_queue
new_worker_thread_queue(memory_arena* arena)
{
    worker_thread_queue result = {0};
    
    result.head = ArenaAlloc(arena, sizeof(worker_thread_queue_member));
    result.tail = ArenaAlloc(arena, sizeof(worker_thread_queue_member));
    
    result.parameterStorage = mini_arena_from_arena(arena, WORKER_THREAD_STORAGE_SIZE);
    
    (result.head)->next = result.tail;
    (result.tail)->back = result.head;
    
    worker_thread_queue_member* member = WT_PushMember(arena, &result);
    result.at = member;
    
    return result;
}

internal worker_thread_queue_member*
WT_PushMember(memory_arena* arena, worker_thread_queue* queue)
{
    ++queue->queueNodesCreated;
    
    worker_thread_queue_member* newMember = 
        Arena_PushStruct(arena, worker_thread_queue_member);
    
    ((queue->tail)->back)->next = newMember;
    (queue->tail)->back = newMember;
    
    newMember->back = queue->tail->back;
    newMember->next = queue->tail;
    
    return newMember;
}

#define FILL_IN_AT(queue, function, parameter) \
queue->at->function = function; \
queue->at->parameter = parameter 

// TODO(winston): time for rework
internal void 
WT_PushQueue(memory_arena* arena, 
             worker_thread_queue* workerThreadQueue,
             void* (*function)(void*), void* parameter)
{
    ++workerThreadQueue->numberOfJobsInQueue;
    
    BeginTicketMutex(&workerThreadQueue->osCall, 
                     &workerThreadQueue->at->queueMemberMutex);
    
    FILL_IN_AT(workerThreadQueue, function, parameter);
    
    EndTicketMutex(&workerThreadQueue->at->queueMemberMutex);
    
    if(workerThreadQueue->at->next == workerThreadQueue->tail)
    {
        if(workerThreadQueue->head->next->function == 0)
        {
            workerThreadQueue->at = workerThreadQueue->head->next;
        }
        else
        {
            worker_thread_queue_member* newMember = WT_PushMember(arena, workerThreadQueue);
            
            BeginTicketMutex(&workerThreadQueue->osCall, 
                             &newMember->queueMemberMutex);
            
            workerThreadQueue->at = newMember;
            
            newMember->parameter = parameter;
            newMember->function = function;
            
            EndTicketMutex(&newMember->queueMemberMutex);
            
        }
    }
    else
    {
        workerThreadQueue->at = workerThreadQueue->at->next;
    }
    
}
#undef FILL_IN_AT

// NOTE(winston): will break if queue is not handled quickly enough
internal void* 
WT_CustomArenaAlloc(memory_arena* arena, 
                    u32 sizeToAlloc)
{
    if(arena->sizeLeft < sizeToAlloc)
    {
        arena->offset = 0;
    }
    
    return ArenaAlloc(arena, sizeToAlloc);
}
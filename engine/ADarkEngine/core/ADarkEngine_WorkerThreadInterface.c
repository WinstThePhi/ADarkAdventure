#include <stdio.h>

#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.h"

inline void
BeginTicketMutex(OS_call* call, ticket_mutex* ticketMutex)
{
    u64 ticket = AtomicAddU64(&ticketMutex->ticketServed, 1);
    while(ticket != ticketMutex->ticketServing)
    {
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
    
    while(!queue->isRunning)
    {
        for(worker_thread_queue_member* member = (queue->head)->next;
            member != queue->tail;
            member = member->next)
        {
            BeginTicketMutex(&queue->osCall, &queue->workerThreadQueueMutex);
            
            //while(member->function == 0);
            
            if(member->function)
                (*member->function)(member->parameter);
            
            member->function = 0;
            member->parameter = 0;
            
            --queue->numberOfJobsInQueue;
            
            EndTicketMutex(&queue->workerThreadQueueMutex);
        }
    }
    
    char buffer[64] = {0};
    
    // TODO(winston): check to see if _snprintf_s is available on POSIX systems
    _snprintf_s(buffer, 64, 64, "Queue Nodes Created: %d", queue->queueNodesCreated);
    
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
WT_GetOpenSlot(worker_thread_queue* queue)
{
    for(worker_thread_queue_member* i = (queue->head)->next;
        i != queue->tail;
        i = i->next)
    {
        if(i->function == 0)
        {
            return i;
        }
    }
    
    return 0;
}

internal worker_thread_queue_member*
WT_PushMember(memory_arena* arena, 
              worker_thread_queue* queue)
{
    ++queue->queueNodesCreated;
    
    worker_thread_queue_member* newMember = Arena_PushStruct(arena, worker_thread_queue_member);
    
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
    
    BeginTicketMutex(&workerThreadQueue->osCall, &workerThreadQueue->workerThreadQueueMutex);
    
    FILL_IN_AT(workerThreadQueue, function, parameter);
    
    if(workerThreadQueue->at->next == workerThreadQueue->tail)
    {
        if(workerThreadQueue->head->next->function == 0)
        {
            workerThreadQueue->at = workerThreadQueue->head->next;
        }
        else
        {
            worker_thread_queue_member* newMember = WT_PushMember(arena, workerThreadQueue);
            
            workerThreadQueue->at = newMember;
            
            newMember->parameter = parameter;
            newMember->function = function;
        }
    }
    else
    {
        workerThreadQueue->at = workerThreadQueue->at->next;
    }
    
    EndTicketMutex(&workerThreadQueue->workerThreadQueueMutex);
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
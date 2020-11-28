#include <stdio.h>

#include "ADarkEngine/core/worker_thread_interface.h"

inline void
BeginTicketMutex(ticket_mutex* ticketMutex)
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

#define TICKET_BEGIN() \
BeginTicketMutex(&QueueMember(queue, memberIndex).queueMemberMutex)
#define TICKET_END() \
EndTicketMutex(&QueueMember(queue, memberIndex).queueMemberMutex)

internal u32
WT_ProcessWorkQueue(void* temp)
{
    worker_thread_queue* queue = (worker_thread_queue*)temp;
    os_call os = queue->osCall;
    
    f32 lastTime = os.GetTime_MS();
    
    while(queue->isRunning)
    {
        
        for(u32 memberIndex = 0;
            memberIndex < NUM_OF_WORKER_THREAD_QUEUE_MEMBER; 
            ++memberIndex)
        {
            {
                FUNCTION_CHECK:;
                
                TICKET_BEGIN();
                
                if((QueueMember(queue, memberIndex)).function)
                {
                    RunQueueFunction(queue, memberIndex);
                }
                else 
                {
                    TICKET_END();
                    
                    if((!queue->isRunning) && (queue->pendingJobCount == 0))
                    {
                        break;
                    }
                    
                    goto FUNCTION_CHECK;
                }
                
                ClearQueueMember(queue, memberIndex);
                
                TICKET_END();
            }
            
            AtomicDecrement(&queue->pendingJobCount);
            
            {
                f32 currentTime = os.GetTime_MS();
                f32 timeElapsed = currentTime - lastTime;
                
                if(timeElapsed <= 16.667f)
                {
                    f32 timeToSleep = 16.667f - timeElapsed;
                    os.ThreadSleep(timeToSleep);
                }
                
                lastTime = currentTime;
            }
            
            if((!queue->isRunning) && (queue->pendingJobCount == 0))
            {
                break;
            }
        }
    }
    
    return 0;
}

#undef TICKET_BEGIN
#undef TICKET_END

internal worker_thread_queue
new_worker_thread_queue(memory_arena* arena)
{
    worker_thread_queue result = {};
    
    result.parameterStorage = mini_arena_from_arena(arena, WORKER_THREAD_STORAGE_SIZE);
    
    result.isRunning = 1;
    
    return result;
}

#define TICKET_BEGIN() \
BeginTicketMutex(&memberAt->queueMemberMutex)
#define TICKET_END() \
EndTicketMutex(&memberAt->queueMemberMutex)

internal void
WT_PushQueue(worker_thread_queue* workerThreadQueue, 
             void* (*function)(void*), void* parameter)
{
    if(workerThreadQueue->writeOffset >
       (NUM_OF_WORKER_THREAD_QUEUE_MEMBER - 1))
    {
        workerThreadQueue->writeOffset = 0;
    }
    
    worker_thread_queue_member* memberAt =
        &QueueMember(workerThreadQueue, workerThreadQueue->writeOffset);
    
    TICKET_BEGIN();
    memberAt->function = function;
    memberAt->parameter = parameter;
    TICKET_END();
    
    AtomicIncrement(&workerThreadQueue->pendingJobCount);
    AtomicIncrement(&workerThreadQueue->writeOffset);
}

#undef TICKET_BEGIN
#undef TICKET_END

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
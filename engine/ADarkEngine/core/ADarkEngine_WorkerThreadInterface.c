#include <stdio.h>

#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.h"

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

// TODO(winston): sync loop with main thread
internal u32
WT_ProcessWorkQueue(void* temp)
{
    worker_thread_queue* queue = (worker_thread_queue*)temp;
    OS_call os = queue->osCall;
    
    f32 lastTime = os.GetTime_MS();
    
    while(queue->isRunning)
    {
        
        for(i32 memberIndex = 0;
            memberIndex < NUM_OF_WORKER_THREAD_QUEUE_MEMBER; 
            ++memberIndex)
        {
            FUNCTION_CHECK:;
            BeginTicketMutex(&queue->members[memberIndex].queueMemberMutex);
            
            if(queue->members[memberIndex].function)
            {
                (*queue->members[memberIndex].function)(queue->members[memberIndex].parameter);
            }
            else 
            {
                EndTicketMutex(&queue->members[memberIndex].queueMemberMutex);
                
                if((!queue->isRunning) && (queue->pendingJobCount == 0))
                {
                    break;
                }
                
                goto FUNCTION_CHECK;
            }
            
            queue->members[memberIndex].function = 0;
            queue->members[memberIndex].parameter = 0;
            
            EndTicketMutex(&queue->members[memberIndex].queueMemberMutex);
            
            --queue->pendingJobCount;
            
            f32 currentTime = os.GetTime_MS();
            f32 timeElapsed = currentTime - lastTime;
            
            if(timeElapsed <= 16.667f)
            {
                f32 timeToSleep = 16.667f - timeElapsed;
                os.ThreadSleep(timeToSleep);
            }
            
            lastTime = currentTime;
        }
    }
    
    return 0;
}

internal worker_thread_queue
new_worker_thread_queue(memory_arena* arena)
{
    worker_thread_queue result = {0};
    
    result.parameterStorage = mini_arena_from_arena(arena, WORKER_THREAD_STORAGE_SIZE);
    
    result.isRunning = 1;
    
    return result;
}

// TODO(winston): time for rework
internal void
WT_PushQueue(memory_arena* arena,
             worker_thread_queue* workerThreadQueue, void* (*function)(void*), void* parameter)
{
    if(workerThreadQueue->writeOffset >
       (NUM_OF_WORKER_THREAD_QUEUE_MEMBER - 1))
    {
        workerThreadQueue->writeOffset = 0;
    }
    
    BeginTicketMutex(&workerThreadQueue->members[workerThreadQueue->writeOffset].queueMemberMutex);
    
    workerThreadQueue->members[workerThreadQueue->writeOffset].function = 
        function;
    workerThreadQueue->members[workerThreadQueue->writeOffset].parameter = 
        parameter;
    
    EndTicketMutex(&workerThreadQueue->members[workerThreadQueue->writeOffset].queueMemberMutex);
    
    ++workerThreadQueue->pendingJobCount;
    ++workerThreadQueue->writeOffset;
}

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
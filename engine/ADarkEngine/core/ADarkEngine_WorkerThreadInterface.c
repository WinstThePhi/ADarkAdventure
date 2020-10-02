#include <stdlib.h>

#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.h"

// TODO(winston) [FIXED]: fix bug (stale member pointer points to an area not allowed to access)
// NOTE(winston): problem above came from worker thread calling function while passing an parameter that is not filled out 
// NOTE(winston): for function to be called, parameter cannot be a nullptr
// TODO(winston): maybe fix for above?
// NOTE(winston): maybe capping is a bad idea because does not sync with main thread
// TODO(winston): sync loop with main thread
internal u32
ProcessWorkQueue(void* temp)
{
    worker_thread_queue* queue = (worker_thread_queue*)temp;
    f32 lastTime = (queue->osCall).GetTime_MS();
    
    while(!queue->breakCommand)
    {
        for(worker_thread_queue_member* member = (queue->head)->next;
            member != queue->tail;
            member = member->next)
        {
            // NOTE(winston): this point here forces parameter to not be nullptr to be called
            if((member->function != 0) && (member->parameter))
            {
                (*member->function)(member->parameter);
                
                member->function = 0;
                member->parameter = 0;
                
                --queue->numberOfJobsInQueue;
                continue;
            }
            else
            {
                break;
            }
        }
        
#if 0
        f32 currentTime = (queue->osCall).GetTime_MS();
        f32 timeElapsed = currentTime - lastTime;
        if((queue->numberOfJobsInQueue) == 0 && (timeElapsed < 16.667f))
        {
            (queue->osCall).ThreadSleep((i32)16.667f - (i32)timeElapsed);
        }
        lastTime = currentTime;
#endif
        //printf("%d", queue->parameterStorage.offset);
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
    
    return result;
}

internal void 
PushWorkQueue(memory_arena* arena, 
              worker_thread_queue* workerThreadQueue,
              void* (*function)(void*), void* parameter)
{
    ++workerThreadQueue->numberOfJobsInQueue;
    
    worker_thread_queue_member* openSlot = 0;
    
    for(worker_thread_queue_member* i = (workerThreadQueue->head)->next;
        i != workerThreadQueue->tail;
        i = i->next)
    {
        if(i->function == 0)
        {
            openSlot = i;
            break;
        }
    }
    
    if(openSlot)
    {
        openSlot->function = function;
        openSlot->parameter = parameter;
    }
    else
    {
        ++workerThreadQueue->queueNodesCreated;
        
        worker_thread_queue_member* newMember = Arena_PushStruct(arena, worker_thread_queue_member);
        
        newMember->function = function;
        newMember->parameter = parameter;
        
        newMember->back = workerThreadQueue->tail->back;
        newMember->next = workerThreadQueue->tail;
        
        ((workerThreadQueue->tail)->back)->next = newMember;
        (workerThreadQueue->tail)->back = newMember;
    }
}

// NOTE(winston): will break if queue is not handled quickly enough
internal void* 
WorkerThread_CustomArenaAlloc(memory_arena* arena, 
                              u32 sizeToAlloc)
{
    if(arena->sizeLeft < sizeToAlloc)
    {
        arena->offset = 0;
    }
    
    return ArenaAlloc(arena, sizeToAlloc);
}
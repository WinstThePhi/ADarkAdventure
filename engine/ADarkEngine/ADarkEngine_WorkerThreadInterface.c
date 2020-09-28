#include <stdlib.h>

#include "ADarkEngine/ADarkEngine_WorkerThreadInterface.h"

internal u32
ProcessWorkQueue(void* temp)
{
    worker_thread_queue* queue = (worker_thread_queue*)temp;
    f32 lastTime = (queue->osCall).GetTime_MS();
    
    loop
    {
        for(worker_thread_queue_member* member = (queue->head)->next;
            member != queue->tail;
            member = member->next)
        {
            if(member->function)
            {
                (*member->function)(member->parameter);
                
                if(member->parameter)
                    free(member->parameter);
                
                member->function = 0;
                member->parameter = 0;
                
                continue;
            }
            else
            {
                break;
            }
        }
        
        if(queue->breakCommand && (queue->head->next == 0))
        {
            queue->isDone = 1;
            return 0;
        }
        
        f32 currentTime = (queue->osCall).GetTime_MS();
        f32 timeElapsed = currentTime - lastTime;
        
        if(timeElapsed < 60)
        {
            (queue->osCall).ThreadSleep(60 - (i32)timeElapsed);
        }
        
        lastTime = currentTime;
    }
}

internal worker_thread_queue
new_worker_thread_queue(memory_arena* arena)
{
    worker_thread_queue result = {0};
    
    result.head = ArenaAlloc(arena, sizeof(worker_thread_queue_member));
    result.tail = ArenaAlloc(arena, sizeof(worker_thread_queue_member));
    
    (result.head)->next = result.tail;
    (result.tail)->back = result.head;
    
    return result;
}

internal void 
PushWorkQueue(memory_arena* arena, 
              worker_thread_queue* workerThreadQueue,
              void* (*function)(void*), void* parameter)
{
    worker_thread_queue_member* openSlot = 0;
    
    if(workerThreadQueue->head->next != workerThreadQueue->tail)
    {
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
    }
    
    if(openSlot)
    {
        openSlot->function = function;
        openSlot->parameter = parameter;
    }
    else
    {
        printf("hi");
        
        worker_thread_queue_member* newMember = 
            ArenaAlloc(arena, sizeof(worker_thread_queue_member));
        
        newMember->function = function;
        newMember->parameter = parameter;
        
        newMember->back = workerThreadQueue->tail->back;
        newMember->next = workerThreadQueue->tail;
        
        ((workerThreadQueue->tail)->back)->next = newMember;
        (workerThreadQueue->tail)->back = newMember;
    }
}

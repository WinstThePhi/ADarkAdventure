#include "win32_worker_thread_interface.h"

internal worker_thread_queue* 
Win32_NewWorkerThreadQueue(memory_arena& arena)
{
    worker_thread_queue* queue = Arena_PushStruct(arena, worker_thread_queue);
    
    queue->semaphoreHandle = CreateSemaphoreA(0, 
                                              0, 
                                              64, 
                                              "Worker Queue Semaphore");
    DWORD threadContext = 0;
    
    queue->threadHandle = CreateThread(0,
                                       0,
                                       (WIN32_THREAD_FUNC_TEMPLATE)Win32_ThreadProc,
                                       &queue,
                                       0,
                                       &threadContext);
    return queue;
}

internal void* 
Win32_ThreadProc(worker_thread_queue* queue)
{
    DE_LogInfo("Hi.");
    return 0;
}

internal void
Win32_DestroyWorkerThreadQueue(worker_thread_queue* queue)
{
    TerminateThread(queue->threadHandle, 0);
    CloseHandle(queue->semaphoreHandle);
    
    worker_thread_queue empty = {};
    *queue = empty;
}
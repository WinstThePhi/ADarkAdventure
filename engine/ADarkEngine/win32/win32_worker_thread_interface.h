#ifndef WIN32_WORKER_THREAD_INTERFACE_H
#define WIN32_WORKER_THREAD_INTERFACE_H

#define WIN32_THREAD_FUNC_TEMPLATE DWORD (__cdecl*)(void*)

typedef void function_temp(void* temp);

typedef struct worker_thread_queue_member
{
    function_temp* function;
    void* parameter;
} worker_thread_queue_member;

typedef struct worker_thread_queue
{
    worker_thread_queue_member members[64];
    
    b32 volatile isRunning;
    
    u64 volatile readOffset;
    u64 volatile writeOffset;
    
    HANDLE semaphoreHandle;
    HANDLE threadHandle;
} worker_thread_queue;

internal worker_thread_queue* Win32_NewWorkerThreadQueue(memory_arena& arena);

internal void* Win32_ThreadProc(worker_thread_queue* queue);

internal void Win32_DestroyWorkerThreadQueue(worker_thread_queue* queue);

#endif

#ifndef _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H
#define _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H

#ifndef WORKER_THREAD_STORAGE_SIZE
#define WORKER_THREAD_STORAGE_SIZE Megabytes(32)
//#define WORKER_THREAD_STORAGE_SIZE 1024
#endif

typedef struct worker_thread_queue_member worker_thread_queue_member;

struct worker_thread_queue_member
{
    void* (*function)(void*);
    void* parameter;
    
    worker_thread_queue_member* next;
    worker_thread_queue_member* back;
};

// TODO(winston): Also move this out

#define GET_TIME(name) f32 name()
typedef GET_TIME(get_time);

#define THREAD_SLEEP(name) void name(i32 numOfMS)
typedef THREAD_SLEEP(thread_sleep);

typedef struct OS_call
{
    get_time* GetTime_MS;
    thread_sleep* ThreadSleep;
} OS_call;

typedef struct worker_thread_queue
{
    worker_thread_queue_member* head;
    worker_thread_queue_member* tail;
    
    b32 breakCommand;
    
    // NOTE(winston): use this for passing into queue functions
    memory_arena parameterStorage;
    
    u32 queueNodesCreated;
    u32 numberOfJobsInQueue;
    
    // TODO(winston): move this out of here
    OS_call osCall;
} worker_thread_queue;

internal u32 ProcessWorkQueue(void* temp);

internal void* WorkerThread_CustomArenaAlloc(memory_arena* arena, 
                                             u32 sizeToAlloc);

internal void PushWorkQueue(memory_arena* arena,
                            worker_thread_queue* workerThreadQueue, void* (*function)(void*), void* parameter);

internal worker_thread_queue new_worker_thread_queue(memory_arena* arena);

// NOTE(winston): quick macro for the ArenaAlloc of the WorkerThread
#define WT_PushStruct(arena, structName) \
(structName*)WorkerThread_CustomArenaAlloc(arena, sizeof(structName))

#endif

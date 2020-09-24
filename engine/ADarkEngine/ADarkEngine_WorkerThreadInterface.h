#ifndef _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H
#define _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H

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
    b32 isDone;
    
    // TODO(winston): move this out of here
    OS_call osCall;
} worker_thread_queue;

internal void PushWorkQueue(memory_arena* arena,
                            worker_thread_queue* workerThreadQueue, void* function, void* parameter);

internal worker_thread_queue new_worker_thread_queue(memory_arena* arena);

#endif

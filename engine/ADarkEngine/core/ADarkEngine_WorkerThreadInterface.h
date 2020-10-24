#ifndef _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H
#define _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H

#ifndef WORKER_THREAD_STORAGE_SIZE
#define WORKER_THREAD_STORAGE_SIZE Megabytes(32)
#endif

#ifndef NUM_OF_WORKER_THREAD_QUEUE_MEMBER
#define NUM_OF_WORKER_THREAD_QUEUE_MEMBER 32
#endif

typedef struct ticket_mutex
{
    u64 volatile ticketServed;
    u64 volatile ticketServing;
} ticket_mutex;

// TODO(winston): Also move this out
#define GET_TIME(name) f32 name()
typedef GET_TIME(get_time);

#define THREAD_SLEEP(name) void name(f32 numOfMS)
typedef THREAD_SLEEP(thread_sleep);

typedef struct OS_call
{
    get_time* GetTime_MS;
    thread_sleep* ThreadSleep;
} OS_call;

typedef struct worker_thread_queue_member worker_thread_queue_member;
struct worker_thread_queue_member
{
    void* (*function)(void*);
    void* parameter;
    
    ticket_mutex queueMemberMutex;
};

typedef struct worker_thread_queue
{
    worker_thread_queue_member members[NUM_OF_WORKER_THREAD_QUEUE_MEMBER];
    
    b32 isRunning;
    memory_arena parameterStorage;
    
    u32 writeOffset;
    
    u32 pendingJobCount;
    
    // TODO(winston): move this out of here
    OS_call osCall;
} worker_thread_queue;

internal void BeginTicketMutex(ticket_mutex* ticketMutex);

internal void EndTicketMutex(ticket_mutex* ticketMutex);

internal u32 WT_ProcessWorkQueue(void* temp);

internal void* WT_CustomArenaAlloc(memory_arena* arena, 
                                   u32 sizeToAlloc);

internal void WT_PushQueue(memory_arena* arena,
                           worker_thread_queue* workerThreadQueue, void* (*function)(void*), void* parameter);

internal worker_thread_queue new_worker_thread_queue(memory_arena* arena);

// NOTE(winston): quick macro for the ArenaAlloc of the WorkerThread
#define WT_PushStruct(arena, structName) \
(structName*)WT_CustomArenaAlloc(arena, sizeof(structName))

#endif

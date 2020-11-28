#ifndef _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H
#define _A_DARK_ENGINE__WORKER_THREAD_INTERFACE_H

#ifndef WORKER_THREAD_STORAGE_SIZE
#define WORKER_THREAD_STORAGE_SIZE Megabytes(32)
#endif

#ifndef NUM_OF_WORKER_THREAD_QUEUE_MEMBER
#define NUM_OF_WORKER_THREAD_QUEUE_MEMBER 32
#endif

#define QueueMember(queue, index) \
queue->members[index]

#define RunQueueFunction(queue, index) \
(*QueueMember(queue, index).function)(QueueMember(queue, index).parameter)

// TODO(winston): Also move this out
#define GET_TIME(name) f32 name()
typedef GET_TIME(get_time);

#define THREAD_SLEEP(name) void name(f32 numOfMS)
typedef THREAD_SLEEP(thread_sleep); 

#define LOAD_OPENGL_PROCEDURE(name) void* name(char* procedureName)
typedef LOAD_OPENGL_PROCEDURE(load_opengl_procedure);

typedef struct os_call
{
    get_time* GetTime_MS;
    thread_sleep* ThreadSleep;
    load_opengl_procedure* LoadOpenGLProcedure;
} os_call;

typedef struct ticket_mutex
{
    u64 volatile ticketServed;
    u64 volatile ticketServing;
} ticket_mutex;


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
    
    u64 volatile writeOffset;
    u64 volatile pendingJobCount;
    
    // TODO(winston): move this out of here
    os_call osCall;
} worker_thread_queue;

inline void 
ClearQueueMember(worker_thread_queue* queue, 
                 u32 index)
{
    queue->members[index].function = 0;
    queue->members[index].parameter = 0;
}

internal void BeginTicketMutex(ticket_mutex* ticketMutex);

internal void EndTicketMutex(ticket_mutex* ticketMutex);

internal u32 WT_ProcessWorkQueue(void* temp);

internal void* WT_CustomArenaAlloc(memory_arena* arena, 
                                   u32 sizeToAlloc);

internal void WT_PushQueue(worker_thread_queue* workerThreadQueue, 
                           void* (*function)(void*), 
                           void* parameter);

internal worker_thread_queue new_worker_thread_queue(memory_arena* arena);

// NOTE(winston): quick macro for the ArenaAlloc of the WorkerThread
#define WT_PushStruct(arena, structName) \
(structName*)WT_CustomArenaAlloc(arena, sizeof(structName))

#endif

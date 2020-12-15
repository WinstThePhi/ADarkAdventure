#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

struct gl_procedures;

// TODO(winston): Also move this out
typedef f32 get_time();
typedef void thread_sleep(f32 numOfMS);
typedef void* load_opengl_procedure(char* procedureName);

typedef struct os_call os_call;
struct os_call
{
    get_time* GetTime_MS;
    thread_sleep* ThreadSleep;
    load_opengl_procedure* LoadOpenGLProcedure;
};

#define NUM_OF_OS_EVENT 3

typedef enum key_code key_code;
enum key_code
{
#define Key(name) KEY_##name,
    KEY_NULL,
#include "../key_list.inc"
#undef Key
    KEY_MAX
};

typedef enum os_event_type os_event_type;
enum os_event_type
{
    EVENT_NULL,
    WINDOW_CLOSE,
    EVENT_KEY
};

typedef enum key_state key_state;
enum key_state
{
    KEY_PRESS,
    KEY_RELEASE
};

typedef struct os_event os_event;
struct os_event
{
    os_event_type eventType;
    
    key_code keyCode;
    key_state keyState;
};

typedef struct os_event_list os_event_list;
struct os_event_list
{
    os_event* events;
    
    u32 size;
    
    u32 writeOffset;
    u32 readOffset;
};

typedef struct key_data key_data;
struct key_data
{
    b32 isDown;
};

#undef NUM_OF_KEYS
#undef NUM_OF_OS_EVENT

#include "generated/game_state.h"

#define GAME_ARGS game_state& gameState, memory_arena& arena

#define GAME_UPDATE_AND_RENDER(name) void name(GAME_ARGS)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

#define START_GAME(name) void name(GAME_ARGS)
typedef START_GAME(start_game);

#define END_GAME(name) void name(GAME_ARGS)
typedef END_GAME(end_game);

END_GAME(Game_EndStub)
{
}

START_GAME(Game_StartStub)
{
}

GAME_UPDATE_AND_RENDER(Game_UpdateAndRenderStub)
{
}

internal os_event_list GenerateEventList(memory_arena& arena, u32 size);

internal void PushOSEvent(os_event_list* list,
                          os_event event);

inline os_event OSEvent(os_event_type type);

internal os_event KeyEvent(key_code keyCode,
                           key_state keyState);

internal void ClearEventList(os_event_list* list);

#endif

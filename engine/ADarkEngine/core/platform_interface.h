#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#define NUM_OF_OS_EVENT 3

typedef enum key_code
{
#define Key(name) KEY_##name,
    KEY_NULL,
#include "../key_list.inc"
#undef Key
    KEY_MAX
} key_code;

typedef enum OS_event_type
{
    EVENT_NULL,
    WINDOW_CLOSE,
    EVENT_KEY
} OS_event_type;

typedef enum key_state
{
    KEY_PRESS,
    KEY_RELEASE
} key_state;

typedef struct OS_event OS_event;

struct OS_event
{
    OS_event_type eventType;
    
    key_code keyCode;
    key_state keyState;
};

typedef struct OS_event_list
{
    OS_event* events;
    
    u32 size;
    
    u32 writeOffset;
    u32 readOffset;
} OS_event_list;

typedef struct key_data
{
    b32 isDown;
} key_data;

#undef NUM_OF_KEYS
#undef NUM_OF_OS_EVENT

typedef struct back_buffer
{
    void* memory;
    
    u16 height;
    u16 width;
    
    u16 pitch;
    u32 bytesPerPixel;
} back_buffer;

typedef struct render_group
{
    memory_arena* arena;
    worker_thread_queue* workerThreadQueue;
    back_buffer* backBuffer;
} render_group;

#include "ADarkEngine/generated/game_state.h"

#define GAME_ARGS game_state* gameState, memory_arena* arena, worker_thread_queue* queue

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

internal OS_event_list GenerateEventList(memory_arena* arena, u32 size);

internal void PushOSEvent(OS_event_list* list,
                          OS_event event);

internal OS_event OSEvent(OS_event_type type);

internal OS_event KeyEvent(key_code keyCode,
                           key_state keyState);

internal void ClearEventList(OS_event_list* list);

global game_state* globalGameGameState;

inline b32 
IsKeyDown(key_code keyCode)
{
    return (keyCode < KEY_MAX && keyCode > KEY_NULL) ?
        globalGameGameState->keyData[keyCode].isDown : 0;
}

#endif

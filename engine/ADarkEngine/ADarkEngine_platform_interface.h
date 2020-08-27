#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#define NUM_OF_OS_EVENT   2

typedef enum key_code
{
#define Key(name) KEY_##name,
    KEY_NULL,
#include "key_list.inc"
#undef Key
    KEY_MAX
} key_code;

typedef enum OS_event_type
{
    EVENT_NULL,
    WINDOW_CLOSE,
    KEY_PRESS,
    KEY_RELEASE
} OS_event_type;

typedef struct OS_event OS_event;

struct OS_event
{
    OS_event_type eventType;
    
    union
    {
        key_code keyCode;
    };
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

typedef struct game_state
{
    b32 isRunning;
    
    OS_event_list eventList;
    key_data keyData[KEY_MAX];
    
    b32 isFullscreen;
    
    u16 x;
    u16 y;
    
    u16 fpsCap;
} game_state;

typedef struct back_buffer
{
    void* memory;
    
    u16 height;
    u16 width;
    
    u16 pitch;
} back_buffer;

#define GAME_UPDATE_AND_RENDER(name) void name(game_state* gameState, back_buffer* backBuffer, MemoryArena* arena)

typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

GAME_UPDATE_AND_RENDER(Game_UpdateAndRenderStub)
{
    
}

#define START_GAME(name) void name(game_state* gameState, \
back_buffer* backBuffer, \
MemoryArena* arena)

typedef START_GAME(start_game);

START_GAME(Game_StartStub)
{
    
}

#define END_GAME(name) void name(game_state* gameState, \
back_buffer* backBuffer, \
MemoryArena* arena)

typedef END_GAME(end_game);

END_GAME(Game_EndStub)
{
    
}

#include "ADarkEngine/ADarkEngine_platform_interface.c"

#endif

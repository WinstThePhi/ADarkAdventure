#ifndef GAME_H
#define GAME_H

typedef struct game_state
{
    b32 isRunning;
    OS_event_list eventList;
    key_data keyData[KEY_MAX];
} game_state;

typedef struct back_buffer
{
    void* memory;
    
    u16 height;
    u16 width;
    
    u16 pitch;
} back_buffer;

#endif 

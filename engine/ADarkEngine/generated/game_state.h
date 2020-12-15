#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct game_state
{
    //do not remove these
    b32 isRunning;
    os_event_list eventList;
    key_data keyData[KEY_MAX];
    b32 isFullscreen;
    u16 fpsCap;
    
} game_state;

#endif
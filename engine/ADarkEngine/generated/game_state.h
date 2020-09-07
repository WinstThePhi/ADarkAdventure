#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct game_state
{
//do not remove these
b32 isRunning;
OS_event_list eventList;
key_data keyData[KEY_MAX];
b32 isFullscreen;
u16 fpsCap;

u16 x;
u16 y;

back_buffer tiger;

} game_state;

#endif
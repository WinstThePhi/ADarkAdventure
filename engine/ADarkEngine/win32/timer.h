#ifndef TIMER_H
#define TIMER_H

typedef struct timer_info
{
    f32 lastTime;
    f32 currentTime;
    
    LARGE_INTEGER performanceFrequency;
} timer_info;

internal f32 GetTime_MS();

internal timer_info new_timer_info(LARGE_INTEGER performanceFrequency);

inline void Win32_BeginFrame(timer_info* timerInfo);

inline void Win32_EndFrame(timer_info* timerInfo, f32 msCap);

#endif

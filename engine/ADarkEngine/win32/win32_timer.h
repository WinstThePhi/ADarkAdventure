#ifndef TIMER_H
#define TIMER_H

// TODO(winston): fix dis

typedef struct timer_info
{
    f32 lastTime;
    f32 currentTime;
} timer_info;

global LARGE_INTEGER performanceFrequency = {};

inline f32 
GetTime_MS()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    
    f64 MS_elapsed = 
        ((f64)time.QuadPart / (f64)performanceFrequency.QuadPart);
    
    MS_elapsed *= 1000;
    
    return (f32)MS_elapsed;
}

internal timer_info new_timer_info();

inline void Win32_BeginFrame(timer_info* timerInfo);

inline void Win32_EndFrame(timer_info* timerInfo, f32 msCap);

#endif

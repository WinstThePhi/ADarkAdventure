
#include "win32/win32_timer.h"

internal timer_info
new_timer_info()
{
    timer_info timerInfo = {};
    
    timerInfo.lastTime = GetTime_MS();
    
    return timerInfo;
}

inline void 
Win32_BeginFrame(timer_info* timerInfo)
{
}

inline void 
Win32_EndFrame(timer_info* timerInfo, 
               f32 msCap)
{
    timerInfo->currentTime = GetTime_MS();
    
    f32 timeElapsed = timerInfo->currentTime - timerInfo->lastTime;
    
    if(timeElapsed < msCap)
    {
        u32 timeToSleep = (u32)(msCap - timeElapsed);
        Sleep((DWORD)(timeToSleep));
    }
    
    timerInfo->lastTime = timerInfo->currentTime;
}
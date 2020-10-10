#ifndef _A_DARK_ENGINE_SHARED_H
#define _A_DARK_ENGINE_SHARED_H

#ifdef COMPILER_MSVC
inline u64
AtomicAddU64(u64 volatile* value, u64 addend)
{
    return (u64)_InterlockedExchangeAdd64((__int64 volatile*)value,
                                          addend);
}
#endif

#endif

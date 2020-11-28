#ifndef _A_DARK_ENGINE_SHARED_H
#define _A_DARK_ENGINE_SHARED_H

#ifdef COMPILER_MSVC
#include <intrin.h>
inline u64
AtomicAddU64(u64 volatile* value, u64 addend)
{
    return (u64)_InterlockedExchangeAdd64((__int64 volatile*)value,
                                          addend);
}

inline u64 
AtomicIncrement(u64 volatile* value)
{
    return (u64)_InterlockedIncrement64((__int64 volatile*) value);
}

inline u64
AtomicDecrement(u64 volatile* value)
{
    return (u64)_InterlockedDecrement64((__int64 volatile*) value);
}
#endif

#endif

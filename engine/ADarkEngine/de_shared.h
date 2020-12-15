#ifndef _A_DARK_ENGINE_SHARED_H
#define _A_DARK_ENGINE_SHARED_H

typedef union v4
{
    struct
    {
        u16 x;
        u16 y;
        u16 z;
        u16 w;
    };
    struct
    {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };
} v4;

typedef union v3
{
    struct
    {
        u16 x;
        u16 y;
        u16 z;
    };
    struct
    {
        u8 r;
        u8 g;
        u8 b;
    };
} v3;


typedef union v2
{
    struct
    {
        u16 x;
        u16 y;
    };
} v2;

inline v3 
v3_color(u8 r, u8 g, u8 b)
{
    v3 result = {};
    result.r = r;
    result.g = g;
    result.b = b;
    
    return result;
}

inline v3
v3_coords(u16 x, u16 y, u16 z)
{
    v3 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

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

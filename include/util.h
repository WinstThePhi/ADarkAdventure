#ifndef UTIL_H
#define UTIL_H

#define Kilobytes(value) (u64)value * 1024
#define Megabytes(value) Kilobytes(value) * 1024
#define Gigabytes(value) Megabytes(value) * 1024

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

#endif

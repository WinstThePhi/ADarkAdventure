#ifndef _DARK_ENGINE_RENDERER_H
#define _DARK_ENGINE_RENDERER_H
#if 0
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
#endif
#pragma pack(push, 1)
typedef struct bitmap_header
{
    u16 fileType;
    u32 fileSize;
    u16 reserved_1;
    u16 reserved_2;
    u32 bitmapOffset;
    u32 size;
    i32 width;
    i32 height;
    u16 planes;
    u16 bitsPerPixel;
} bitmap_header;
#pragma pack(pop)

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

#endif 

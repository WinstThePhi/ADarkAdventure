#ifndef _DARK_ENGINE_RENDERER_H
#define _DARK_ENGINE_RENDERER_H

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

typedef struct pixel_render_group
{
    back_buffer* backBuffer;
    u16 x;
    u16 y;
    v3 color;
} pixel_render_group;

typedef struct background_render_group
{
    back_buffer* backBuffer;
    v3 color;
} background_render_group;

typedef struct rect_render_group
{
    back_buffer* backBuffer;
    
    u16 x, y;
    u16 width, height;
    v3 color;
} rect_render_group;

inline v3 
v3_color(u8 r, u8 g, u8 b)
{
    v3 result = {.r = r, .g = g, .b = b};
    return result;
}

inline v3
v3_coords(u16 x, u16 y, u16 z)
{
    v3 result = {.x = x, .y = y, .z = z};
    return result;
}

inline void DE_DrawPixel(void* temp);

internal void* DE_2d_FillBackground(void* temp);

internal void* DE_2d_DrawRectangle(void* temp);

internal void DE_2d_DrawRectangleOutline(back_buffer* backBuffer,
                                         u16 x, u16 y,
                                         u16 width, u16 height,
                                         v3 color);

internal back_buffer DE_LoadBMP(memory_arena* arena, 
                                char* filename);

internal void DE_2d_DrawBMP(back_buffer* dest, 
                            back_buffer* src);

internal void 
DE2d_PushSolidBackground(memory_arena* globalArena,
                         worker_thread_queue* workerThreadQueue,
                         back_buffer* backBuffer,
                         v3 color);
internal void
DE2d_PushRectangle(memory_arena* globalArena, 
                   worker_thread_queue* workerThreadQueue,
                   back_buffer* backBuffer,
                   u16 x, u16 y,
                   u16 width, u16 height,
                   v3 color);

#endif 

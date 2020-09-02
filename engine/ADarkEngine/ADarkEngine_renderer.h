#ifndef _DARK_ENGINE_RENDERER_H
#define _DARK_ENGINE_RENDERER_H

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

inline void DarkEngine_DrawPixel(back_buffer* backBuffer, 
                                 u16 x, u16 y,
                                 v3 color);

internal void DarkEngine_2d_FillBackground(back_buffer* backBuffer,
                                           v3 color);

internal void DarkEngine_2d_DrawRectangle(back_buffer* backBuffer,
                                          u16 x, u16 y,
                                          u16 width, u16 height,
                                          v3 color);

internal back_buffer DarkEngine_LoadBMP(memory_arena* arena, 
                                        char* filename);

internal void DarkEngine_2d_DrawBMP(back_buffer* dest, 
                                    back_buffer* src);

#include "ADarkEngine/ADarkEngine_renderer.c"

#endif 

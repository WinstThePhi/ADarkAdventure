#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#define Win32_LargeToI64(num) num##.QuadPart

#define BYTES_PER_PIXEL 4

typedef struct win32_back_buffer
{
    BITMAPINFO bitmapInfo;
    
    void* memory;
    
    u16 height;
    u16 width;
    
    u16 pitch;
} win32_back_buffer;

typedef struct window_dimensions
{
    u16 width;
    u16 height;
} window_dimensions;

#endif

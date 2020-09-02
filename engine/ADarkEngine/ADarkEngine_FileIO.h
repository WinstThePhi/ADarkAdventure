#ifndef _DARK_ENGINE__FILE_I_O_H
#define _DARK_ENGINE__FILE_I_O_H

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

internal char* DarkEngine_ReadFile(memory_arena* arena,
                                   char* filename);

internal void DarkEngine_WriteFile(char* filename, 
                                   char* contents);

internal void DarkEngine_WriteFileAppend(char* filename, 
                                         char* contents);

#ifndef ERROR_LOG_PATH
#define ERROR_LOG_PATH "../engine/ADarkEngine/error_log/default_error_log.txt"
#endif

internal void DarkEngine_LogError(char* error);

internal void DarkEngine_ClearFile(char* filename);

#include "ADarkEngine/ADarkEngine_FileIO.c"

#endif

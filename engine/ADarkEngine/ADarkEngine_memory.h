#ifndef DARKENGINE_MEMORY_H
#define DARKENGINE_MEMORY_H

// NOTE(winston): sum very epic memory management

typedef struct MemoryArena MemoryArena;

struct MemoryArena
{
    void* memory;
    
    u32 offset;
    u32 size;
    u32 sizeLeft;
};

#include "ADarkEngine/ADarkEngine_memory.c"

#endif

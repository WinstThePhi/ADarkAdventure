#ifndef MEMORY_H
#define MEMORY_H

// NOTE(winston): sum very epic memory management

typedef struct MemoryArena MemoryArena;

struct MemoryArena
{
    void* memory;
    
    u32 offset;
    u32 size;
    u32 sizeLeft;
};

#endif

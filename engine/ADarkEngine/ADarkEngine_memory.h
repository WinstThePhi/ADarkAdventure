#ifndef DARKENGINE_MEMORY_H
#define DARKENGINE_MEMORY_H

// NOTE(winston): sum very epic memory management

typedef struct memory_arena memory_arena;

struct memory_arena
{
    void* memory;
    
    u32 offset;
    u32 size;
    u32 sizeLeft;
};

typedef struct temp_memory temp_memory;

struct temp_memory
{
    memory_arena* parentArena;
    
    u32 parentOffset;
    
    void* memory;
    
    u32 offset;
    u32 size;
    u32 sizeLeft;
};

// NOTE(winston): should we deal with memory alignment?
internal b32 IsPowerOfTwo(uintptr_t test);

internal memory_arena new_arena(void* memory, u32 size);

internal void* ArenaAlloc(memory_arena* arena, u32 sizeToAlloc);

internal temp_memory temp_arena_start(memory_arena* arena, u32 size);

internal void temp_arena_end(temp_memory* tempArena);

internal void* TempAlloc(temp_memory* tempArena, u32 sizeToAlloc);

#endif

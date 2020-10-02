#ifndef DARKENGINE_MEMORY_H
#define DARKENGINE_MEMORY_H

// NOTE(winston): sum very epic memory management

typedef enum memory_allocation_type
{
    MEMORY_ARENA,
    TEMP_MEMORY_ARENA
} memory_allocation_type;

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

typedef struct memory_parameter memory_parameter;

struct memory_parameter
{
    union
    {
        memory_arena* arena;
        temp_memory* tempMemory;
    };
    
    memory_allocation_type type;
};

// NOTE(winston): should we deal with memory alignment?
internal b32 IsPowerOfTwo(uintptr_t test);

internal memory_arena new_arena(void* memory, u32 size);

internal void* ArenaAlloc(memory_arena* arena, u32 sizeToAlloc);

internal temp_memory temp_arena_start(memory_arena* arena, u32 size);

internal void temp_arena_end(temp_memory* tempArena);

internal void* TempAlloc(temp_memory* tempArena, u32 sizeToAlloc);

internal memory_arena new_arena_from_arena(memory_arena* arena, u32 sizeOfNewArena);

internal void ClearArenaToZero(memory_arena* arena);

// NOTE(winston): some memory helper macros

#define Arena_PushStruct(arena, structName) \
(structName*)ArenaAlloc(arena, sizeof(structName))

#endif

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
    
    memory_arena(void* memory, u32 arenaSize);
    memory_arena(memory_arena& arena, u32 arenaSize);
    
    void* ArenaAlloc(u32 sizeToAlloc);
    void ClearArenaToZero();
};

typedef struct temp_memory temp_memory;
struct temp_memory
{
    void* memory;
    
    memory_arena* parentArena;
    u32 parentOffset;
    
    u32 offset;
    u32 size;
    u32 sizeLeft;
    
    temp_memory(memory_arena& arena, u32 memorySize);
    ~temp_memory();
    
    void* TempAlloc(u32 size);
};

internal memory_arena new_arena_from_arena(memory_arena* arena, u32 sizeOfNewArena);

#define Arena_PushStruct(arena, structName) \
(structName*)arena.ArenaAlloc(sizeof(structName))

#endif

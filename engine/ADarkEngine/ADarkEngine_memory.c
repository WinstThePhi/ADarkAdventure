#include <string.h>
#include <assert.h>

#include "ADarkEngine/ADarkEngine_memory.h"

internal memory_arena
new_arena(void* memory, u32 size)
{
    memory_arena arena;
    
    arena.memory = memory;
    arena.offset = 0;
    arena.size = size;
    
    arena.sizeLeft = size;
    
    return arena;
}

internal void*
ArenaAlloc(memory_arena* arena, u32 sizeToAlloc)
{
    void* result = 0;
    
    Assert(sizeToAlloc <= arena->sizeLeft && "No more space left in arena!");
    
    result = (void*)((char*)arena->memory + arena->offset);
    
    Assert(result);
    
    arena->offset += sizeToAlloc;
    arena->sizeLeft -= sizeToAlloc;
    
    MemorySet(result, 0, sizeToAlloc);
    
    return result;
}

internal temp_memory
temp_arena_start(memory_arena* arena, u32 size)
{
    temp_memory tempArena = {0};
    
    tempArena.parentArena = arena;
    tempArena.parentOffset = arena->offset;
    
    tempArena.memory = ArenaAlloc(arena, size);
    
    Assert(tempArena.memory);
    
    tempArena.size = size;
    tempArena.sizeLeft = size;
    
    return tempArena;
}

internal void
temp_arena_end(temp_memory* tempArena)
{
    (tempArena->parentArena)->offset = tempArena->parentOffset;
    
    // NOTE(winston): zeroing the struct
    tempArena->parentArena = 0;
    tempArena->parentOffset = 0;
    tempArena->memory = 0;
    tempArena->offset = 0;
    tempArena->size = 0;
    tempArena->sizeLeft = 0;
}

internal void* TempAlloc(temp_memory* tempArena, 
                         u32 sizeToAlloc)
{
    void* result = 0;
    
    Assert(sizeToAlloc <= tempArena->sizeLeft && "No more space left in tempArena!");
    
    result = (void*)((char*)tempArena->memory + tempArena->offset);
    
    Assert(result);
    
    tempArena->offset += sizeToAlloc;
    tempArena->sizeLeft -= sizeToAlloc;
    
    MemorySet(result, 0, sizeToAlloc);
    
    return result;
}
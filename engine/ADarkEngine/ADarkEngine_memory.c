#include <string.h>
#include <assert.h>

internal MemoryArena
new_arena(void* memory, u32 size)
{
    MemoryArena arena;
    
    arena.memory = memory;
    arena.offset = 0;
    arena.size = size;
    
    arena.sizeLeft = size;
    
    return arena;
}

internal void*
ArenaAlloc(MemoryArena* arena, u32 sizeToAlloc)
{
    void* result = 0;
    
    Assert(sizeToAlloc < arena->sizeLeft && "No more space left in arena!");
    
    result = (void*)((char*)arena->memory + arena->offset);
    
    Assert(result);
    
    arena->offset += sizeToAlloc;
    arena->sizeLeft -= sizeToAlloc;
    
    MemorySet(result, 0, sizeToAlloc);
    
    return result;
}


#include <string.h>
#include <assert.h>

#include "de_memory.h"

memory_arena::memory_arena(void* memory, u32 arenaSize)
{
    this->memory = memory;
    offset = 0;
    size = arenaSize;
    sizeLeft = arenaSize;
}

memory_arena::memory_arena(memory_arena& parentArena, 
                           u32 sizeOfNewArena)
{
    memory = parentArena.ArenaAlloc(sizeOfNewArena);
    offset = 0;
    size = sizeOfNewArena;
    sizeLeft = size;
}

void* memory_arena::ArenaAlloc(u32 sizeToAlloc)
{
    void* result = 0;
    
    Assert(sizeToAlloc <= sizeLeft);
    
    result = (void*)((char*)memory + offset);
    
    offset += sizeToAlloc;
    sizeLeft -= sizeToAlloc;
    MemorySet(result, 0, sizeToAlloc);
    
    u32 align = offset % 16;
    offset += align;
    sizeLeft -= align;
    
    return result;
}

void memory_arena::ClearArenaToZero()
{
    MemorySet(memory, 0, size);
    offset = 0;
    sizeLeft = size;
}

temp_memory::temp_memory(memory_arena& arena, u32 memorySize)
{
    parentArena = &arena;
    parentOffset = arena.offset;
    
    memory = arena.ArenaAlloc(memorySize);
    
    Assert(memory);
    
    size = memorySize;
    sizeLeft = size;
    offset = 0;
}

temp_memory::~temp_memory()
{
    parentArena->offset = parentOffset;
    
    parentArena = 0;
    parentOffset = 0;
    memory = 0;
    offset = 0;
    size = 0;
    sizeLeft = 0;
}

void* temp_memory::TempAlloc(u32 sizeToAlloc)
{
    void* result = 0;
    
    Assert(sizeToAlloc <= sizeLeft);
    
    result = (void*)((char*)memory + offset);
    
    offset += sizeToAlloc;
    sizeLeft -= sizeToAlloc;
    MemorySet(result, 0, sizeToAlloc);
    
    u32 align = offset % 16;
    offset += align;
    sizeLeft -= align;
    
    return result;
}

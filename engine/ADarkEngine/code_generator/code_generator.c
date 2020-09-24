#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOG_TO_STDERR

#include "ADarkEngine\ADarkEngine_layer.h"
#include "ADarkEngine\ADarkEngine_memory.c"
#include "ADarkEngine\ADarkEngine_util.c"
#include "ADarkEngine\ADarkEngine_FileIO.c"

#define BIG_BOI_ALLOC_SIZE Megabytes(20)

global memory_arena arena;

internal char*
GetFileName(char* test)
{
    // NOTE(winston): check if string has a period
    if(!(strchr(test, '.')))
    {
        return test;
    }
    
    char* result = strrchr(test, '.'); 
    *result = '\0';
    
    return test;
}

internal char*
GetExtension(char* test)
{
    // NOTE(winston): check if string has a period
    if(!(strchr(test, '.')))
    {
        Assert(0 && "No extension.");
    }
    
    char* result = strrchr(test, '.'); 
    
    return result + 1;
}

internal b32 
IsExtension(char* test, 
            char* extension)
{
    b32 isExtension = !(strcmp(GetExtension(test), extension));
    
    return isExtension;
}

internal void
GenerateGameStateStruct(char* gameStatePath, 
                        char* additionalInfo)
{
    FILE* file = fopen(gameStatePath, "w");
    
    fprintf(file, "#ifndef GAME_STATE_H\n#define GAME_STATE_H\n\n");
    fprintf(file, "typedef struct game_state\n");
    fprintf(file, "{\n");
    fprintf(file, additionalInfo);
    fprintf(file, "\n} game_state;");
    fprintf(file, "\n\n#endif");
    
    fclose(file);
}

int main(i32 argCount, char** args)
{
    if(argCount == 1)
    {
        fprintf(stderr, "Expected one filename\n");
        return -1;
    }
    
    u32* memory = (u32*)malloc(BIG_BOI_ALLOC_SIZE);
    
    arena = new_arena((void*)memory, BIG_BOI_ALLOC_SIZE);
    
    for(i32 i = 1; i <= argCount; ++i)
    {
        if(!IsExtension(args[i], "dark"))
        {
            fprintf(stderr, "Expected file extension: dark\n");
        }
        
        
        char* fileContents = DarkEngine_ReadFile(&arena, args[i]);
        
        char path[100] = "../engine/ADarkEngine/generated/";
        
        strcat(path, GetFileName(args[i]));
        strcat(path, ".h");
        
        GenerateGameStateStruct(path,
                                fileContents);
    }
    
    free(memory);
    
    return 0;
}
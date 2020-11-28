#include <stdio.h>

#include "ADarkEngine/core/file_io.h"

// NOTE(winston): Does not count null-terminate byte, I think
// TODO(winston): Maybe check to see if the above is true.
internal u32
DE_GetFileSize(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    u32 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    return fileSize;
}

internal char* 
DE_ReadFileByLine(memory_parameter memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = 0;
    
    switch(memory.type)
    {
        case MEMORY_ARENA:
        {
            data = (char*)ArenaAlloc(memory.arena, fileSize + 1);
        } break;
        case TEMP_MEMORY_ARENA:
        {
            data = (char*)TempAlloc(memory.tempMemory, fileSize + 1);
        } break;
    }
    
    if(data)
    {
        char temp[256];
        
        while(fgets(temp, 256, file))
        {
            strncat(data, temp, 256);
        }
    }
    return data;
}

internal char* 
DE_ReadEntireFile(memory_parameter memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = 0;
    
    switch(memory.type)
    {
        case MEMORY_ARENA:
        {
            data = (char*)ArenaAlloc(memory.arena, fileSize + 1);
        } break;
        case TEMP_MEMORY_ARENA:
        {
            data = (char*)TempAlloc(memory.tempMemory, fileSize + 1);
        } break;
    }
    
    if(data)
    {
        fread(data, 1, fileSize + 1, file);
    }
    
    return data;
}

internal char* 
DE_ReadFile(memory_parameter memory, 
            char* filename, 
            read_type type)
{
    switch(type)
    {
        case ENTIRE_FILE:
        {
            return DE_ReadEntireFile(memory, filename);
        } break;
        case BY_LINE:
        {
            return DE_ReadFileByLine(memory, filename);
        } break;
    }
    
    return 0;
}

internal void 
DE_WriteFile(char* filename, 
             char* contents)
{
    FILE* file = fopen(filename, "w");
    
    if(file)
    {
        fprintf(file, "%s", contents);
        fclose(file);
    }
}

internal void 
DE_WriteFileAppend(char* filename, 
                   char* contents)
{
    FILE* file = fopen(filename, "a");
    
    if(file)
    {
        fprintf(file, "%s", contents);
        fclose(file);
    }
}

internal void
DE_LogError(char* error)
{
#ifndef LOG_TO_STDERR
    DE_WriteFileAppend(ERROR_LOG_PATH,
                       "ERROR: ");
    DE_WriteFileAppend(ERROR_LOG_PATH,
                       error);
    DE_WriteFileAppend(ERROR_LOG_PATH,
                       "\n");
#else 
    fprintf(stderr, "ERROR (DE2d): ");
    fprintf(stderr, "%s", error);
    fprintf(stderr, "\n");
#endif
}

internal void
DE_LogInfo(char* info)
{
#ifndef INFO_TO_STDOUT
    DE_WriteFileAppend(INFO_LOG_PATH,
                       "ERROR: ");
    DE_WriteFileAppend(INFO_LOG_PATH,
                       info);
    DE_WriteFileAppend(INFO_LOG_PATH,
                       "\n");
#else 
    fprintf(stderr, "INFO (DE2d): ");
    fprintf(stderr, "%s", info);
    fprintf(stderr, "\n");
#endif
}

internal void 
DE_ClearFile(char* filename)
{
    DE_WriteFile(filename,
                 "");
}

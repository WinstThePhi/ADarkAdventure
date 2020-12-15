#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "de_file_io.h"

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
    
    fclose(file);
    return fileSize;
}

internal char* 
DE_ReadFileByLine(memory_arena& memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = (char*)memory.ArenaAlloc(fileSize + 1);
    
    char temp[256];
    
    while(fgets(temp, 256, file))
    {
        strncat(data, temp, 256);
    }
    
    fclose(file);
    return data;
}

internal char* 
DE_ReadFileByLine(temp_memory& memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = (char*)memory.TempAlloc(fileSize + 1);
    
    char temp[256];
    
    while(fgets(temp, 256, file))
    {
        strncat(data, temp, 256);
    }
    
    fclose(file);
    return data;
}

internal char* 
DE_ReadEntireFile(temp_memory& memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = (char*)memory.TempAlloc(fileSize + 1);
    
    if(data)
    {
        fread(data, 1, fileSize + 1, file);
    }
    
    fclose(file);
    return data;
}

internal char* 
DE_ReadEntireFile(memory_arena& memory, 
                  char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DE_LogError("File to read not found.");
        return 0;
    }
    
    u32 fileSize = DE_GetFileSize(filename);
    char* data = (char*)memory.ArenaAlloc(fileSize + 1);
    
    if(data)
    {
        fread(data, 1, fileSize + 1, file);
    }
    
    fclose(file);
    return data;
}

internal void 
DE_WriteFile(char* filename, 
             char* contents)
{
    FILE* file = fopen(filename, "w");
    
    if(!file)
    {
        DE_LogError("File open error.");
    }
    
    fprintf(file, "%s", contents);
    fclose(file);
}

internal void 
DE_WriteFileAppend(char* filename, 
                   char* contents)
{
    FILE* file = fopen(filename, "a");
    
    if(!file)
    {
        DE_LogError("File open error.");
    }
    
    fprintf(file, "%s", contents);
    fclose(file);
}

internal void 
DE_ClearFile(char* filename)
{
    DE_WriteFile(filename,
                 "");
}

internal void
DE_LogError(char* error, ...)
{
    char buffer[512];
    
    va_list args;
    va_start(args, error);
    vsnprintf(buffer, 512, error, args);
    va_end(args);
    
#ifndef LOG_TO_STDERR
    DE_WriteFileAppend(ERROR_LOG_PATH, "ERROR (DE2d): ");
    DE_WriteFileAppend(ERROR_LOG_PATH, buffer);
    DE_WriteFileAppend(ERROR_LOG_PATH, "\n");
#else 
    fprintf(stdout, "INFO (DE2d): ");
    fprintf(stderr, buffer);
    fprintf(stderr, "\n");
#endif
}

internal void
DE_LogInfo(char* info, ...)
{
    char buffer[512];
    //= "INFO (DE2d): ";
    
    va_list args;
    va_start(args, info);
    vsnprintf(buffer, 512, info, args);
    va_end(args);
    
#ifndef INFO_TO_STDOUT
    DE_WriteFileAppend(INFO_LOG_PATH, "INFO (DE2d): ");
    DE_WriteFileAppend(INFO_LOG_PATH,
                       buffer);
#else 
    fprintf(stdout, "INFO (DE2d): ");
    fprintf(stdout, buffer);
    fprintf(stdout, "\n");
#endif
}

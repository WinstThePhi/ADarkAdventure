#include <stdio.h>

internal char* 
DarkEngine_ReadFile(memory_arena* arena,
                    char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        DarkEngine_LogError("File to read not found.");
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    i32 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* data = (char*)ArenaAlloc(arena, fileSize + 1);
    fread(data, fileSize + 1, 1, file);
    
    return data;
}

internal void 
DarkEngine_WriteFile(char* filename, 
                     char* contents)
{
    FILE* file = fopen(filename, "w");
    
    if(file && contents)
    {
        fprintf(file, contents);
    }
    
    
    fclose(file);
}

internal void 
DarkEngine_WriteFileAppend(char* filename, 
                           char* contents)
{
    FILE* file = fopen(filename, "a");
    
    if(file && contents)
    {
        fprintf(file, contents);
    }
    
    fclose(file);
}

internal void
DarkEngine_LogError(char* error)
{
#ifndef LOG_TO_STDERR
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               "ERROR: ");
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               error);
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               "\n");
#else 
    fprintf(stderr, "ERROR: ");
    fprintf(stderr, error);
    fprintf(stderr, "\n");
#endif
}

internal void 
DarkEngine_ClearFile(char* filename)
{
    DarkEngine_WriteFile(filename,
                         "");
}

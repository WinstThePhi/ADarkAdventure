#include <stdio.h>

internal char* 
DarkEngine_ReadFile(char* filename)
{
    
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
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               "ERROR: ");
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               error);
    DarkEngine_WriteFileAppend(ERROR_LOG_PATH,
                               "\n");
}

internal void 
DarkEngine_ClearFile(char* filename)
{
    DarkEngine_WriteFile(filename,
                         "");
}
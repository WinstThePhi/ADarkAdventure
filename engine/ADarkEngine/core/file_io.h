#ifndef FILE_IO_H
#define FILE_IO_H

typedef enum read_type
{
    ENTIRE_FILE,
    BY_LINE
} read_type;

#ifndef ERROR_LOG_PATH
#define LOG_TO_STDERR
#endif

#ifndef INFO_LOG_PATH
#define INFO_TO_STDOUT
#endif

internal u32 DE_GetFileSize(char* filename);

internal char* DE_ReadFileByLine(memory_parameter memory, 
                                 char* filename);

internal char* DE_ReadEntireFile(memory_parameter memory, 
                                 char* filename);

internal char* DE_ReadFile(memory_parameter memory, 
                           char* filename, 
                           read_type type);

internal void DE_WriteFile(char* filename, 
                           char* contents);

internal void DE_WriteFileAppend(char* filename, 
                                 char* contents);

internal void DE_LogError(char* error);

internal void DE_LogInfo(char* info);

internal void DE_ClearFile(char* filename);

#endif

#ifndef FILE_IO_H
#define FILE_IO_H

#ifndef ERROR_LOG_PATH
#define LOG_TO_STDERR
#endif

#ifndef INFO_LOG_PATH
#define INFO_TO_STDOUT
#endif

internal u32 DE_GetFileSize(char* filename);

internal char* DE_ReadFileByLine(memory_arena& memory, 
                                 char* filename);

internal char* DE_ReadFileByLine(temp_memory& memory, 
                                 char* filename);

internal char* DE_ReadEntireFile(memory_arena& memory, 
                                 char* filename);

internal char* DE_ReadEntireFile(temp_memory& memory, 
                                 char* filename);

internal void DE_WriteFile(char* filename, 
                           char* contents);

internal void DE_WriteFileAppend(char* filename, 
                                 char* contents);

internal void DE_ClearFile(char* filename);

// NOTE(winston): prints max of 512 bytes
internal void DE_LogError(char* error, ...);

internal void DE_LogInfo(char* info, ...);

#endif

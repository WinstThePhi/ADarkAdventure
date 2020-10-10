#ifndef _DARK_ENGINE__FILE_I_O_H
#define _DARK_ENGINE__FILE_I_O_H

typedef enum read_type
{
    ENTIRE_FILE,
    BY_LINE
} read_type;

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

#ifndef ERROR_LOG_PATH
#define ERROR_LOG_PATH "../engine/ADarkEngine/error_log/default_error_log.txt"
#endif

internal void DE_LogError(char* error);

internal void DE_ClearFile(char* filename);

#endif

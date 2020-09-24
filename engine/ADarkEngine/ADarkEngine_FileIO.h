#ifndef _DARK_ENGINE__FILE_I_O_H
#define _DARK_ENGINE__FILE_I_O_H

internal char* DarkEngine_ReadFile(memory_arena* arena,
                                   char* filename);

internal void DarkEngine_WriteFile(char* filename, 
                                   char* contents);

internal void DarkEngine_WriteFileAppend(char* filename, 
                                         char* contents);

#ifndef ERROR_LOG_PATH
#define ERROR_LOG_PATH "../engine/ADarkEngine/error_log/default_error_log.txt"
#endif

internal void DarkEngine_LogError(char* error);

internal void DarkEngine_ClearFile(char* filename);

#endif

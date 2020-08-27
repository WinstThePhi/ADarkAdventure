#ifndef _DARK_ENGINE__FILE_I_O_H
#define _DARK_ENGINE__FILE_I_O_H

internal char* DarkEngine_ReadFile(char* filename);

internal void DarkEngine_WriteFile(char* filename, 
                                   char* contents);

internal void DarkEngine_WriteFileAppend(char* filename, 
                                         char* contents);

internal void DarkEngine_LogError(char* error);

internal void DarkEngine_ClearFile(char* filename);

#include "ADarkEngine/ADarkEngine_FileIO.c"

#endif

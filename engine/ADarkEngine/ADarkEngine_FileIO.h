#ifndef _DARK_ENGINE__FILE_I_O_H
#define _DARK_ENGINE__FILE_I_O_H

internal char* DarkEngine_ReadFile(char* filename);

internal void DarkEngine_WriteFile(char* filename, 
                                   char* contents, 
                                   u32 contentsCount);

#include "ADarkEngine/ADarkEngine_FileIO.c"

#endif

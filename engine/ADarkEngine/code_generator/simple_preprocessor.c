#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADarkEngine\ADarkEngine_layer.h"
#include "ADarkEngine\code_generator\simple_preprocessor.h"

internal char*
ReadEntireFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        fprintf(stderr, "File not found!\n");
    }
    
    fseek(file, 0, SEEK_END);
    i32 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* data = (char*)malloc(fileSize + 1);
    fread(data, fileSize + 1, 1, file);
    
    return data;
}

internal u32
GetNumberOfNewLinesInFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        fprintf(stderr, "File not found!\n");
    }
    
    u32 count = 0;
    
    i32 character;
    character = getc(file);
    
    while(character != EOF)
    {
        if(character == '\n')
        {
            ++count;
        }
        character = getc(file);
    }
    
    return count;
}

internal u32
GetNumberOfLinesInFile(char* filename)
{
    return GetNumberOfNewLinesInFile(filename) + 1;
}

internal extensions
GetExtensions(char* extensionsFile)
{
    char* extensionsReadResult = ReadEntireFile(extensionsFile);
    
    u32 lineCount = GetNumberOfLinesInFile(extensionsFile);
    
    extensions result = {0};
    char** extensionChar = (char**)malloc(lineCount);
    
    extensionChar[0] = strtok(extensionsReadResult, "\n");
    
    for(u32 i = 1; i < lineCount; ++i)
    {
        extensionChar[i] = strtok(0, "\n");
    }
    
    result.extension = extensionChar;
    
    // TODO(winston): not always the case, FIX ME GAY BOI
    result.numberOfExtensions = lineCount;
    
    free(extensionsReadResult);
    
    return result;
}

int main(i32 argCount, char** args)
{
    if(argCount == 1)
    {
        fprintf(stderr, "Expected more than one argument.\n");
        return -1;
    }
    extensions extension = GetExtensions("extension.txt");
    
    for(i32 i = 0; i < extension.numberOfExtensions; ++i)
    {
        printf("extension: %s\n", extension.extension[i]);
    }
    
    for(i32 i = 1; i < argCount; ++i)
    {
        u32 num = GetNumberOfLinesInFile(args[i]);
        printf("File: %s\nNumber of lines: %d\n", args[i], num);
    }
    
    return 0;
}
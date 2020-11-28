#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "ADarkEngine\code_generator\simple_preprocessor.h"

#ifndef SUPPORTED_EXTENSIONS
#define SUPPORTED_EXTENSIONS "extensions.txt"
#endif 

// NOTE(winston): file operations
internal b32 
FileExists(char* filename)
{
    FILE* file = fopen(filename, "r");
    
    if(file)
    {
        fclose(file);
        return 1;
    }
    else
    {
        return 0;
    }
}

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
    fread(data, fileSize, 1, file);
    
    fclose(file);
    
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

// NOTE(winston): file extensions operations
internal extensions
GetExtensions(char* extensionsFile)
{
    char* extensionsReadResult = ReadEntireFile(extensionsFile);
    
    u32 lineCount = GetNumberOfLinesInFile(extensionsFile);
    
    extensions result = {};
    char** extensionChar = (char**)malloc(lineCount * sizeof(char*));
    
    extensionChar[0] = (char*)malloc(3 * sizeof(char));
    extensionChar[0] = strtok(extensionsReadResult, "\n");
    
    for(u32 i = 1; i < lineCount; ++i)
    {
        //extensionChar[i] = (char*)malloc(3 * sizeof(char));
        extensionChar[i] = strtok(0, "\n");
    }
    
    result.extension = extensionChar;
    
    // TODO(winston): not always the case, FIX ME, GAY BOI
    result.numberOfExtensions = lineCount;
    
    free(extensionsReadResult);
    
    return result;
}

internal void
FreeExtensions(extensions* extension)
{
#if 0
    for(i32 i = 0; i < extension->numberOfExtensions; ++i)
    {
        free(extension->extension[i]);
    }
#endif
    
    extension->numberOfExtensions = 0;
    free(extension->extension);
}

internal char*
GetExtension(char* test)
{
    // NOTE(winston): check if string has a period
    if(!(strchr(test, '.')))
    {
        Assert(0 && "No extension.");
    }
    
    char* result = strrchr(test, '.'); 
    
    return result + 1;
}

inline b32 
IsExtension(char* test, 
            char* extension)
{
    return !(strcmp(GetExtension(test), extension));;
}

// NOTE(winston): file_list operations
internal file_list
new_file_list()
{
    files* head = (files*)malloc(sizeof(files));
    files* tail = (files*)malloc(sizeof(files));
    
    head->next = tail;
    tail->back = head;
    
    file_list result = {};
    result.head = head;
    result.tail = tail;
    
    return result;
}

internal void
PushFileList(file_list* fileList, char* filename)
{
    u32 filenameLength = (u32)strlen(filename);
    
    files* last = (fileList->tail)->back;
    files* newFile = (files*)malloc(sizeof(files));
    
    newFile->fileName = (char*)malloc(filenameLength * sizeof(char));
    newFile->fileName = filename;
    
    newFile->back = last;
    newFile->next = fileList->tail;
    (fileList->tail)->back = newFile;
    last->next = newFile;
}

internal void
free_file_list(file_list* fileList)
{
    for(files* file = (fileList->head)->next; 
        file != fileList->tail; 
        file = file->next)
    {
        free(file->back);
        file->fileName = 0;
        free(file->fileName);
        file->back = 0;
    }
    free((fileList->tail)->back);
    free(fileList->tail);
}

internal void
PrintFileList(file_list* fileList)
{
    for(files* file = (fileList->head)->next; 
        file != fileList->tail; 
        file = file->next)
    {
        printf("%s\n", file->fileName);
    }
}

internal files* 
GetNextFileInList(file_list* list)
{
    files* file = (list->head)->next;
    
    for(u32 i = 0; i < list->readOffsetFromHead; ++i)
    {
        if(file == (list->tail)->back)
        {
            list->readOffsetFromHead = 0;
            return 0;
        }
        
        file = file->next;
    }
    
    ++list->readOffsetFromHead;
    
    return file;
}

// NOTE(winston): tokenizer
internal tokenizer
TokenizeFileData(char* fileData)
{
    tokenizer result = {};
    result.tokens = (token*)malloc((strlen(fileData) + 1) * sizeof(token));
    
    b32 tokenizerRunning = 1;
    i32 i = 0;
    b32 preprocessorStart = 0;
    
    while(tokenizerRunning)
    {
        if(preprocessorStart)
        {
            i32* iReference = &i;
            while(preprocessorStart && (i < strlen(fileData)))
            {
                char atChar = fileData[*iReference];
                
                token tok = {};
                tok.text = atChar;
                tok.type = TOKEN_PREPROCESSOR;
                
                if(atChar == '\n')
                {
                    preprocessorStart = 0;
                    break;
                }
                else
                {
                    result.tokens[*iReference] = tok;
                    ++(*iReference);
                }
            }
        }
        
        char atChar = fileData[i];
        
        token tok = {};
        tok.text = atChar;
        
        switch(atChar)
        {
            case '(':
            {
                tok.type = TOKEN_PAREN_OPEN;
            } break;
            case ')':
            {
                tok.type = TOKEN_PAREN_CLOSE;
            } break;
            case '{':
            {
                tok.type = TOKEN_BRACE_OPEN;
            } break;
            case '}':
            {
                tok.type = TOKEN_BRACE_CLOSE;
            } break;
            case '[':
            {
                tok.type = TOKEN_BRACKET_OPEN;
            } break;
            case ']':
            {
                tok.type = TOKEN_BRACKET_CLOSE;
            } break;
            case '\n':
            {
                tok.type = TOKEN_NEW_LINE;
            }break;
            case ' ':
            case '\t':
            {
                tok.type = TOKEN_WHITESPACE;
            } break;
            case '\0':
            {
                tok.type = TOKEN_END_OF_FILE;
                tokenizerRunning = 0;
            } break;
            case '#':
            {
                tok.type = TOKEN_PREPROCESSOR;
                preprocessorStart = 1;
            } break;
            default:
            {
                tok.type = TOKEN_IDENTIFIER;
            } break;
        }
        
        result.tokens[i] = tok;
        
        ++i;
    }
    
    result.at = result.tokens;
    result.numOfTokens = (u32)(strlen(fileData) + 1);
    
    return result;
}

inline token_type
GetTokenTypeAt(tokenizer* tokenizerThing)
{
    return tokenizerThing->at->type;
}

internal b32
IncrementTokenizerAll(tokenizer* tokenizerThing)
{
    if((tokenizerThing->at)->type != TOKEN_END_OF_FILE)
    {
        ++tokenizerThing->at;
        return 1;
    }
    else
    {
        return 0;
    }
}

internal b32 
IncrementTokenizerNoWhitespace(tokenizer* tokenizerThing)
{
    loop
    {
        if((tokenizerThing->at)->type != TOKEN_END_OF_FILE)
        {
            INCREMENT_TOKENIZER:;
            
            ++tokenizerThing->at;
            if((tokenizerThing->at)->type != TOKEN_WHITESPACE && 
               (tokenizerThing->at)->type != TOKEN_NEW_LINE)
            {
                return 1;
            }
            else
            {
                goto INCREMENT_TOKENIZER;
            }
        }
        else
        {
            return 0;
        }
    }
}

internal void
PrintTokenType(token* token)
{
    switch(token->type)
    {
#define Token(tokenName) \
case TOKEN_##tokenName: \
{ \
printf(#tokenName); \
printf("\n"); \
} break;
#include "token.inc"
#undef TOKEN
    }
}

internal void
FreeTokenizer(tokenizer* tokenizerThing)
{
    free(tokenizerThing->tokens);
}

// NOTE(winston): parser

int main(i32 argCount, char** args)
{
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    if(argCount == 1)
    {
        fprintf(stderr, "Expected more than one argument.\n");
        return -1;
    }
    
    extensions extension = GetExtensions(SUPPORTED_EXTENSIONS);
    
    // TODO(winston): assumes that all arguments are filenames. 
    file_list fileList = new_file_list();
    
    for(i32 i = 1; i < argCount; ++i)
    {
        if(FileExists(args[i]))
        {
            for(i32 index = 0; index < extension.numberOfExtensions; ++index)
            {
                if(IsExtension(args[i], extension.extension[index]))
                {
                    PushFileList(&fileList, args[i]);
                    break;
                }
                else if(index == (extension.numberOfExtensions - 1))
                {
                    printf("File format *.%s not supported.\n", GetExtension(args[i]));
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        else
        {
            fprintf(stderr, "File %s does not exist.\n", args[i]);
            continue;
        }
    }
    
    files* temp;
    
    while((temp = GetNextFileInList(&fileList)) != 0)
    {
        printf("%s\n", temp->fileName);
        
        char* fileData = ReadEntireFile(temp->fileName);
        
        tokenizer tokens = TokenizeFileData(fileData);
        
        printf("%c ", (tokens.at)->text);
        PrintTokenType(tokens.at);
        
        while(IncrementTokenizerNoWhitespace(&tokens))
        {
            printf("%c ", (tokens.at)->text);
            PrintTokenType(tokens.at);
        }
        
        FreeTokenizer(&tokens);
        
        free(fileData);
    }
    
    
    free_file_list(&fileList);
    
    FreeExtensions(&extension);
    
    printf("\nFreeing has been done without crashing.\n");
    
    return 0;
}
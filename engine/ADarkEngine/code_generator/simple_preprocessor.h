#ifndef SIMPLE_PREPROCESSOR_H
#define SIMPLE_PREPROCESSOR_H

typedef struct extensions
{
    i32 numberOfExtensions;
    char** extension;
} extensions;

typedef struct files files;

struct files
{
    char* fileName;
    files* next;
    files* back;
};

typedef struct file_list file_list;

struct file_list
{
    files* head;
    files* tail;
    u32 readOffsetFromHead;
};

typedef enum token_type
{
    TOKEN_IDENTIFIER,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_BRACE_OPEN,
    TOKEN_BRACE_CLOSE,
    TOKEN_WHITESPACE,
    TOKEN_END_OF_FILE
} token_type;

typedef struct token
{
    token_type type;
    
    char text;
    i32 length;
} token;

typedef struct tokenizer
{
    token* at;
    token* tokens;
    
    u32 numOfTokens;
} tokenizer;

#endif
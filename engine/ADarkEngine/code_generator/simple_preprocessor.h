#ifndef SIMPLE_PREPROCESSOR_H
#define SIMPLE_PREPROCESSOR_H

#define internal      static
#define global        static
#define local_persist static

#define MemorySet  memset
#define MemoryMove memmove
#define MemoryCopy memcpy

#define loop for(;;)

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef float  f32;
typedef double f64;

#include <assert.h>
#define Assert assert

#define DisableBuffering(stream) setvbuf(stream, 0, _IONBF, 0);

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
    TOKEN_NULL,
#define Token(tokenName) TOKEN_##tokenName,
#include "token.inc"
#undef Token
    TOKEN_MAX
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
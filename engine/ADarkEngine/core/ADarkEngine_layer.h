#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>

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

#define PRINT_FUNCTION_GEN(TYPE_NAME, TYPE, TYPE_SPECIFIER) \
internal void Print##TYPE_NAME##(TYPE identifier) \
{ \
printf(TYPE_SPECIFIER, identifier);\
}

PRINT_FUNCTION_GEN(Int, int, "%d\n")
PRINT_FUNCTION_GEN(Char, char, "%c\n")
PRINT_FUNCTION_GEN(CharArray, char*, "%s\n")
PRINT_FUNCTION_GEN(FloatingPoint, float, "%f\n")

#define DEBUG_PRINT(identifier) _Generic((identifier), \
i16:PrintInt, \
i32:PrintInt, \
i64:PrintInt, \
u16:PrintInt, \
u32:PrintInt, \
u64:PrintInt, \
char:PrintChar, \
char*:PrintCharArray, \
float:PrintFloatingPoint)(identifier)


#ifdef STD_ASSERT
#include <assert.h>
#define Assert assert
#else 

#define Assert(condition) \
if(!condition) \
{ \
fprintf(stderr, "Assertion failed: %s\n", #condition); \
*(i32*)0 = 0;\
}

// TODO(winston): change to use message box

#endif

#endif


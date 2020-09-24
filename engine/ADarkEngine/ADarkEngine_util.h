#ifndef UTIL_H
#define UTIL_H

#define Kilobytes(value) (u64)value * 1024
#define Megabytes(value) Kilobytes(value) * 1024
#define Gigabytes(value) Megabytes(value) * 1024

#define DisableBuffering(value) setvbuf(value, 0, _IONBF, 0);

#define ArrayCount(value) (sizeof(value) / sizeof(value[0]))

inline b32 isPowerOfTwo();

#endif

#ifndef UTIL_H
#define UTIL_H

#define Kilobytes(value) (u64)value * 1024
#define Megabytes(value) Kilobytes(value) * 1024
#define Gigabytes(value) Megabytes(value) * 1024

#define ArrayCount(value) (sizeof(value) / sizeof(value [0]))

#endif

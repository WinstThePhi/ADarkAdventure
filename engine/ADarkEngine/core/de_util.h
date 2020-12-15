#ifndef DE_UTIL_H
#define DE_UTIL_H

#define Kilobytes(num) 1024 * num
#define Megabytes(num) 1024 * Kilobytes(num)
#define Gigabytes(num) 1024 * Gigabytes(num)

#define DisableBuffering(stream) setvbuf(stream, 0, _IONBF, 0)

#endif

#include "ADarkEngine/core/ADarkEngine_util.h"

#if TESTING
b32 isPowerOfTwo(u32 pointer)
{
    return (pointer & (pointer - 1)) == 0;
}
#endif
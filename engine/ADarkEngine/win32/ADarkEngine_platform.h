#ifndef _A_DARK_ENGINE_PLATFORM_H
#define _A_DARK_ENGINE_PLATFORM_H

// NOTE(winston): in case file name is misleading, this is a file for platform layer includes

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>

#include "ADarkEngine/core/ADarkEngine_layer.h"

#include "ADarkEngine/ADarkEngine_shared.h"

#include "ADarkEngine/core/ADarkEngine_util.c"

#include "game_options.h"

#include "ADarkEngine/core/ADarkEngine_memory.c"
#include "ADarkEngine/core/ADarkEngine_FileIO.c"
#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.c"
#include "ADarkEngine/core/ADarkEngine_platform_interface.c"
#include "ADarkEngine/win32/ADarkEngine_win32_platform.h"
#include "ADarkEngine/win32/ADarkEngine_win32_opengl.c"

#include "ADarkEngine/event_handler.c"

#endif

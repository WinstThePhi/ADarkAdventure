#ifndef _A_DARK_ENGINE_PLATFORM_H
#define _A_DARK_ENGINE_PLATFORM_H

// NOTE(winston): in case file name is misleading, this is a file for platform layer includes

#include <stdio.h>

#include "ADarkEngine/core/layer.h"

#include "ADarkEngine/shared.h"

#include "ADarkEngine/core/util.cpp"

#include "game_options.h"

#include "ADarkEngine/core/memory.cpp"
#include "ADarkEngine/core/file_io.cpp"
#include "ADarkEngine/core/worker_thread_interface.cpp"
#include "ADarkEngine/core/platform_interface.cpp"
#include "ADarkEngine/win32/win32_platform.h"

#include "ADarkEngine/core/opengl.cpp"

#include "ADarkEngine/win32/win32_opengl.cpp"

#include "ADarkEngine/event_handler.cpp"

#include "ADarkEngine/win32/timer.cpp"


#endif

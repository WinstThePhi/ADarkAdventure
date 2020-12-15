#ifndef _A_DARK_ENGINE_PLATFORM_H
#define _A_DARK_ENGINE_PLATFORM_H

// NOTE(winston): in case file name is misleading, this is a file for platform layer includes


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "game_options.h"
#include "core/de_layer.h"
#include "de_shared.h"
#include "core/de_memory.cpp"
#include "core/de_file_io.cpp"
#include "core/de_util.cpp"

#include "win32/win32_d3d.cpp"

#include "core/de_platform_interface.cpp"
#include "win32/win32_worker_thread_interface.cpp"

#include "win32/win32_platform.h"
#include "win32/win32_timer.cpp"

#include "de_event_handler.cpp"

#endif

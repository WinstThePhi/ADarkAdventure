#ifndef _A_DARK_ENGINE_H
#define _A_DARK_ENGINE_H

#include <stdio.h>

//custom game engine
#include "ADarkEngine/core/ADarkEngine_layer.h"

#include "ADarkEngine/ADarkEngine_shared.h"

// NOTE(winston): maybe consider a modular system as the codebase gets bigger
#include "ADarkEngine/core/ADarkEngine_util.c"
#include "ADarkEngine/core/ADarkEngine_memory.c"
#include "ADarkEngine/core/ADarkEngine_FileIO.c"
#include "ADarkEngine/core/ADarkEngine_WorkerThreadInterface.c"
#include "ADarkEngine/core/ADarkEngine_platform_interface.c"
#include "ADarkEngine/core/ADarkEngine_renderer.c"

#endif

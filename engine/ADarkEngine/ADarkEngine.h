#ifndef _A_DARK_ENGINE_H
#define _A_DARK_ENGINE_H

#include <stdio.h>

//custom game engine
#include "ADarkEngine/core/layer.h"

#include "ADarkEngine/shared.h"

// NOTE(winston): maybe consider a modular system as the codebase gets bigger
#include "ADarkEngine/core/util.cpp"
#include "ADarkEngine/core/memory.cpp"
#include "ADarkEngine/core/file_io.cpp"
#include "ADarkEngine/core/worker_thread_interface.cpp"
#include "ADarkEngine/core/platform_interface.cpp"
#include "ADarkEngine/core/renderer.cpp"

inline void 
EnterTheDarkness(game_state* gameState)
{
    theGreatThreeOfRendering = &gameState->renderGroup;
    globalGameGameState = gameState;
}

#endif

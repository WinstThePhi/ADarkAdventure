
#define ERROR_LOG_PATH "../data/game_error.txt"

//custom game engine
#include "ADarkEngine/ADarkEngine_layer.h"
#include "ADarkEngine/ADarkEngine_memory.h"
#include "ADarkEngine/ADarkEngine_FileIO.h"
#include "ADarkEngine/ADarkEngine_platform_interface.h"
#include "ADarkEngine/ADarkEngine_renderer.h"
#include "ADarkEngine/ADarkEngine_util.h"

#include "game.h"
#include "event_handler.c"

// NOTE(winston): runs when the game starts
START_GAME(Game_Start)
{
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    gameState->fpsCap = 60;
    
    DarkEngine_ClearFile(ERROR_LOG_PATH);
    DarkEngine_2d_FillBackground(backBuffer,
                                 (v3){.r = 0, .g = 188, .b = 255});
    gameState->tiger = DarkEngine_LoadBMP(arena, "../data/smiley.bmp");
    
    //DarkEngine_PrintBackBuffer(&gameState->tiger);
    
    DarkEngine_2d_DrawBMP(backBuffer, &gameState->tiger);
}

// NOTE(winston): runs every frame
GAME_UPDATE_AND_RENDER(Game_UpdateAndRender)
{
    ProcessOSMessages(gameState);
    
#define SPEED 6
    if(gameState->keyData[KEY_W].isDown)
    {
        gameState->y -= SPEED;
    }
    
    if(gameState->keyData[KEY_A].isDown)
    {
        gameState->x -= SPEED;
    }
    
    if(gameState->keyData[KEY_S].isDown)
    {
        gameState->y += SPEED;
    }
    
    if(gameState->keyData[KEY_D].isDown)
    {
        gameState->x += SPEED;
    }
#undef SPEED
#if 0
    DarkEngine_2d_FillBackground(backBuffer,
                                 (v3){.r = 0, .g = 188, .b = 255});
#endif
#if 0
    DarkEngine_2d_DrawRectangle(backBuffer,
                                backBuffer->width / 2 - 50, backBuffer->height / 2 - 50,
                                100, 100,
                                (v3){.r = 255, .g = 255, .b = 255});
#endif
}

// NOTE(winston): Runs when unloaded or ended
END_GAME(Game_End)
{
    
}
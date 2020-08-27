
//custom game engine
#include "ADarkEngine/ADarkEngine_layer.h"
#include "ADarkEngine/ADarkEngine_memory.h"
#include "ADarkEngine/ADarkEngine_platform_interface.h"
#include "ADarkEngine/ADarkEngine_renderer.h"
#include "ADarkEngine/ADarkEngine_FileIO.h"
#include "ADarkEngine/ADarkEngine_util.h"

#include "game.h"
#include "event_handler.c"

// NOTE(winston): runs when the game starts
START_GAME(Game_Start)
{
    DarkEngine_2d_FillBackground(backBuffer,
                                 (v3){.r = 0, .g = 0, .b = 0});
    gameState->fpsCap = 60;
    
    char data[] = "Baba smell stinky!\n";
    DarkEngine_ClearFile("../data/output.txt");
    for(i32 i = 0; i < 20; ++i)
        DarkEngine_WriteFileAppend("../data/output.txt",
                                   data);
}

// NOTE(winston): runs every frame
GAME_UPDATE_AND_RENDER(Game_UpdateAndRender)
{
    ProcessOSMessages(gameState);
    
#define SPEED 4
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
    
    DarkEngine_2d_FillBackground(backBuffer,
                                 (v3){.r = 0, .g = 188, .b = 255});
    
    DarkEngine_2d_DrawRectangle(backBuffer,
                                gameState->x, gameState->y,
                                100, 100,
                                (v3){.r = 255, .g = 255, .b = 255});
}

END_GAME(Game_End)
{
    
}
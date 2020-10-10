
#define ERROR_LOG_PATH "../data/game_error.txt"

#include "ADarkEngine/ADarkEngine.h"
#include "game.h"

#include "event_handler.c"

//~ NOTE(winston): runs when the game starts
START_GAME(Game_Start)
{
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    gameState->fpsCap = 60;
    
    DE_ClearFile(ERROR_LOG_PATH);
}

//~ NOTE(winston): runs every frame
GAME_UPDATE_AND_RENDER(Game_UpdateAndRender)
{
    ProcessOSMessages(gameState);
    
    local_persist f32 velY = 0;
#define GRAVITY -0.163f
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
    
    if(gameState->keyData[KEY_SPACE].isDown)
    {
        velY = -10;
    }
#undef SPEED
    
    if((gameState->y + 100) < 600)
    {
        velY -= GRAVITY;
        gameState->y += (u16)velY;
    }
    else
    {
        gameState->y = 500;
        velY = 0;
    }
#undef GRAVITY
    
    DE2d_PushSolidBackground(arena,
                             queue,
                             backBuffer,
                             (v3){.r = 0, .g = 188, .b = 255});
    DE2d_PushRectangle(arena,
                       queue,
                       backBuffer,
                       0, 600,
                       backBuffer->width, backBuffer->height - 600,
                       (v3){.r = 0x9B, .g = 0x76, .b = 0x53});
    DE2d_PushRectangle(arena,
                       queue,
                       backBuffer,
                       gameState->x, gameState->y,
                       100, 100,
                       (v3){.r = 255, .g = 255, .b = 255});
}

//~ NOTE(winston): Runs when unloaded or ended
END_GAME(Game_End)
{
    
}

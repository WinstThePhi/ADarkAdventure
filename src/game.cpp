
#define ERROR_LOG_PATH "../data/game_error.txt"

#include "ADarkEngine/ADarkEngine.h"
#include "game.h"

//~ NOTE(winston): runs when the game starts
START_GAME(Game_Start)
{
    EnterTheDarkness(gameState);
    
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    DE_ClearFile(ERROR_LOG_PATH);
}


//~ NOTE(winston): runs every frame
GAME_UPDATE_AND_RENDER(Game_UpdateAndRender)
{
#if 0
#define SPEED 4
    {
        if(IsKeyDown(KEY_W))
        {
            gameState->y -= SPEED;
        }
        if(IsKeyDown(KEY_A))
        {
            gameState->x -= SPEED;
        }
        if(IsKeyDown(KEY_S))
        {
            gameState->y += SPEED;
        }
        if(IsKeyDown(KEY_D))
        {
            gameState->x += SPEED;
        }
    }
#undef SPEED
    
    DE2d_PushSolidBackground(v3_color(0, 188, 255));
    
    DE2d_PushRectangle(0, 550,
                       GetBackBufferWidth(), 
                       GetBackBufferHeight() - 550,
                       v3_color(155, 118, 83));
    
    DE2d_PushRectangle(gameState->x, gameState->y,
                       100, 100,
                       v3_color(255, 255, 255));
#endif
}

//~ NOTE(winston): Runs when unloaded or ended
END_GAME(Game_End)
{
    
}
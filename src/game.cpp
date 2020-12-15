
#define ERROR_LOG_PATH "../data/game_error.txt"

#include "ADarkEngine.h"

#include "game.h"

//~ NOTE(winston): runs when the game starts
START_GAME(Game_Start)
{
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    DE_ClearFile(ERROR_LOG_PATH);
}


//~ NOTE(winston): runs every frame
GAME_UPDATE_AND_RENDER(Game_UpdateAndRender)
{
    
}

//~ NOTE(winston): Runs when unloaded or ended
END_GAME(Game_End)
{
    
}
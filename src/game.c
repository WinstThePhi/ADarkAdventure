#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//custom game engine
#include "DarkEngine/DarkEngine_layer.h"
#include "DarkEngine/DarkEngine_memory.h"
#include "DarkEngine/DarkEngine_platform_interface.h"
#include "DarkEngine/DarkEngine_renderer.h"

#include "util.h"
#include "game.h"

internal void
ProcessOSMessages(game_state* gameState)
{
    OS_event event;
    
    event = GetNextOSEvent(&gameState->eventList);
    
    while(event.eventType)
    {
        switch(event.eventType)
        {
            case WINDOW_CLOSE:
            {
                gameState->isRunning = 0;
            } break;
            case KEY_PRESS:
            {
                switch(event.keyCode)
                {
                    case KEY_W:
                    {
                        gameState->keyData[KEY_W].isDown = 1;
                    } break;
                    case KEY_A:
                    {
                        gameState->keyData[KEY_A].isDown = 1;
                    } break;
                    case KEY_S:
                    {
                        gameState->keyData[KEY_S].isDown = 1;
                    } break;
                    case KEY_D:
                    {
                        gameState->keyData[KEY_D].isDown = 1;
                    } break;
                    case KEY_UP:
                    {
                        gameState->keyData[KEY_UP].isDown = 1;
                    } break;
                    case KEY_LEFT:
                    {
                        gameState->keyData[KEY_LEFT].isDown = 1;
                    } break;
                    case KEY_RIGHT:
                    {
                        gameState->keyData[KEY_RIGHT].isDown = 1;
                    } break;
                    case KEY_DOWN:
                    {
                        gameState->keyData[KEY_DOWN].isDown = 1;
                    } break;
                }
            } break;
            case KEY_RELEASE:
            {
                switch(event.keyCode)
                {
                    case KEY_W:
                    {
                        gameState->keyData[KEY_W].isDown = 0;
                    } break;
                    case KEY_A:
                    {
                        gameState->keyData[KEY_A].isDown = 0;
                    } break;
                    case KEY_S:
                    {
                        gameState->keyData[KEY_S].isDown = 0;
                    } break;
                    case KEY_D:
                    {
                        gameState->keyData[KEY_D].isDown = 0;
                    } break;
                    case KEY_UP:
                    {
                        gameState->keyData[KEY_UP].isDown = 0;
                    } break;
                    case KEY_LEFT:
                    {
                        gameState->keyData[KEY_LEFT].isDown = 0;
                    } break;
                    case KEY_RIGHT:
                    {
                        gameState->keyData[KEY_RIGHT].isDown = 0;
                    } break;
                    case KEY_DOWN:
                    {
                        gameState->keyData[KEY_DOWN].isDown = 0;
                    } break;
                }
            } break;
            default:
            {
                // TODO(winston): logging
            } break;
        };
        event = GetNextOSEvent(&gameState->eventList);
    }
    
    ClearEventList(&gameState->eventList);
}

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
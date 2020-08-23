#include <math.h>

#include "renderer.c"

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
                globalGameState.isRunning = 0;
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

internal void
Game_UpdateAndRender(game_state* gameState,
                     back_buffer* backBuffer)
{
    local_persist u16 x = 0;
    local_persist u16 y = 0;
    
#define SPEED 2
    if(gameState->keyData[KEY_W].isDown)
    {
        y -= SPEED;
    }
    
    if(gameState->keyData[KEY_A].isDown)
    {
        x -= SPEED;
    }
    
    if(gameState->keyData[KEY_S].isDown)
    {
        y += SPEED;
    }
    
    if(gameState->keyData[KEY_D].isDown)
    {
        x += SPEED;
    }
    
    DrawRectangle(backBuffer,
                  0, 0,
                  backBuffer->width, backBuffer->height,
                  (v3){.r = 0, .g = 0, .b = 0});
    DrawRectangle(backBuffer,
                  x, y,
                  100, 100,
                  (v3){.r = 0, .g = 188, .b = 255});
}

//whatever events you want to handle
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
                    case KEY_SPACE:
                    {
                        gameState->keyData[KEY_SPACE].isDown = 1;
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
                    default:
                    {
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
                    case KEY_SPACE:
                    {
                        gameState->keyData[KEY_SPACE].isDown = 0;
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
                    default:
                    {
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

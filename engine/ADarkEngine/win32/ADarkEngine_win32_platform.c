#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>

#define ERROR_LOG_PATH "../data/platform_error.txt"

#include "ADarkEngine/ADarkEngine_platform.h"

#ifndef WIDTH
#define WIDTH 1280
#endif

#ifndef HEIGHT
#define HEIGHT 720
#endif

#ifndef BIG_BOI_ALLOC_SIZE
#define BIG_BOI_ALLOC_SIZE Gigabytes(1)
#endif 

global game_state globalGameState;
global win32_back_buffer globalWin32BackBuffer;
global WINDOWPLACEMENT g_wpPrev = {0};
global memory_arena arena = {0};
global worker_thread_queue workerThreadQueue = {0};

internal void 
ToggleFullscreen(HWND window)
{
    g_wpPrev.length = sizeof(g_wpPrev);
    
    DWORD style = GetWindowLong(window, GWL_STYLE);
    if((style & WS_OVERLAPPEDWINDOW) && !globalGameState.isFullscreen) {
        MONITORINFO mi = {0};
        mi.cbSize = sizeof(mi);
        
        if (GetWindowPlacement(window, &g_wpPrev) &&
            GetMonitorInfo(MonitorFromWindow(window,
                                             MONITOR_DEFAULTTOPRIMARY), &mi)) 
        {
            SetWindowLong(window, GWL_STYLE,
                          style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(window, HWND_TOP,
                         mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            globalGameState.isFullscreen = 1;
        }
    } 
    else 
    {
        SetWindowLong(window, GWL_STYLE,
                      style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(window, &g_wpPrev);
        SetWindowPos(window, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        globalGameState.isFullscreen = 0;
    }
}

internal FILETIME 
Win32_GetFileLastModifiedTime(char* filename)
{
    HANDLE fileHandle = CreateFileA(filename,
                                    GENERIC_READ,
                                    0,
                                    0,
                                    OPEN_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    0);
    FILETIME result = {0};
    GetFileTime(fileHandle,
                0,
                0,
                &result);
    
    CloseHandle(fileHandle);
    
    return result;
}

internal game_code
Win32_LoadGameCode(memory_arena* localArena, 
                   back_buffer* backBuffer, 
                   char* dllName,
                   worker_thread_queue* queue)
{
    game_code gameCodeLoad;
    
    char tempDLL[] = "ADarkAdventure_temp.dll";
    
    CopyFile(dllName,
             tempDLL,
             0);
    
    gameCodeLoad.gameCode = LoadLibraryA(tempDLL);
    gameCodeLoad.Game_UpdateAndRender = 
        (game_update_and_render*)GetProcAddress(gameCodeLoad.gameCode,
                                                "Game_UpdateAndRender");
    gameCodeLoad.Game_Start = 
        (start_game*)GetProcAddress(gameCodeLoad.gameCode,
                                    "Game_Start");
    gameCodeLoad.Game_End = 
        (end_game*)GetProcAddress(gameCodeLoad.gameCode,
                                  "Game_End");
    
    
    if(!gameCodeLoad.Game_UpdateAndRender)
    {
        gameCodeLoad.Game_UpdateAndRender = Game_UpdateAndRenderStub;
    }
    
    if(!gameCodeLoad.Game_Start)
    {
        gameCodeLoad.Game_Start = Game_StartStub;
    }
    
    if(!gameCodeLoad.Game_End)
    {
        gameCodeLoad.Game_End = Game_EndStub;
    }
    
    gameCodeLoad.Game_Start(&globalGameState,
                            backBuffer,
                            localArena,
                            queue);
    
    gameCodeLoad.lastWriteTime = Win32_GetFileLastModifiedTime(dllName);
    
    return gameCodeLoad;
}

internal void
Win32_UnloadGameCode(game_code* gameCode,
                     memory_arena* localArena,
                     back_buffer* backBuffer,
                     worker_thread_queue* queue)
{
    gameCode->Game_End(&globalGameState,
                       backBuffer,
                       localArena,
                       queue);
    
    FreeLibrary(gameCode->gameCode);
    gameCode->gameCode = 0;
    gameCode->Game_UpdateAndRender = Game_UpdateAndRenderStub;
    gameCode->Game_Start = Game_StartStub;
    gameCode->Game_End = Game_EndStub;
    
}

internal window_dimensions
Win32_GetWindowDimensions(HWND window)
{
    RECT dataRect = {0};
    GetClientRect(window,
                  &dataRect);
    
    window_dimensions dimension = {0};
    dimension.width = (u16)(dataRect.right - dataRect.left);
    dimension.height = (u16)(dataRect.bottom - dataRect.top);
    
    return dimension;
}

internal win32_back_buffer
new_back_buffer(memory_arena* localArena,
                u16 width, 
                u16 height)
{
    win32_back_buffer backBuffer = {0};
    
    backBuffer.width = width;
    backBuffer.height = height;
    
    backBuffer.bitmapInfo.bmiHeader.biSize = sizeof(backBuffer.bitmapInfo.bmiHeader);
    backBuffer.bitmapInfo.bmiHeader.biWidth = backBuffer.width;
    backBuffer.bitmapInfo.bmiHeader.biHeight = -backBuffer.height;
    backBuffer.bitmapInfo.bmiHeader.biPlanes = 1;
    backBuffer.bitmapInfo.bmiHeader.biBitCount = 32;
    backBuffer.bitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    u32 sizeToAlloc = width * height * BYTES_PER_PIXEL;
    backBuffer.memory = ArenaAlloc(localArena, sizeToAlloc);
    backBuffer.pitch = width * BYTES_PER_PIXEL;
    
    return backBuffer;
}

internal void*
Win32_UpdateWindow(void* temp)
{
    window_update_group* updateGroup = (window_update_group*)temp;
    
    StretchDIBits(updateGroup->hdc,
                  0, 0,
                  updateGroup->windowWidth,
                  updateGroup->windowHeight,
                  0, 0,
                  updateGroup->backBuffer->width,
                  updateGroup->backBuffer->height,
                  updateGroup->backBuffer->memory,
                  &updateGroup->backBuffer->bitmapInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
    return 0;
}

internal LRESULT 
Win32_DefaultWindowCallback(HWND window,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam)
{
    LRESULT result = {0};
    
    switch(message)
    {
        case WM_QUIT:
        {
            PushOSEvent(&globalGameState.eventList,
                        WINDOW_CLOSE);
        } break;
        case WM_CLOSE:
        {
            PushOSEvent(&globalGameState.eventList,
                        WINDOW_CLOSE);
        } break;
        case WM_DESTROY:
        {
            PushOSEvent(&globalGameState.eventList,
                        WINDOW_CLOSE);
        } break;
#ifdef HIDE_CURSOR
        case WM_SETCURSOR:
        {
            if(LOWORD(lParam) == HTCLIENT)
                SetCursor(0);
            else
                SetCursor(LoadCursor(0, IDC_ARROW));
            return TRUE;
        } break;
#endif
        case WM_PAINT:
        {
            window_dimensions dimension = Win32_GetWindowDimensions(window);
            
            PAINTSTRUCT paintStruct = {0};
            HDC paintDC = BeginPaint(window,
                                     &paintStruct);
            
            window_update_group* updateGroup = (window_update_group*)malloc(sizeof(window_update_group));
            
            updateGroup->hdc = paintDC;
            updateGroup->backBuffer = &globalWin32BackBuffer;
            updateGroup->windowWidth = dimension.width;
            updateGroup->windowHeight = dimension.height;
            
            PushWorkQueue(&arena, 
                          &workerThreadQueue,
                          Win32_UpdateWindow,
                          updateGroup);
            
            EndPaint(window,
                     &paintStruct);
        } break;
        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            u16 VKcode = (u16)wParam;
            b32 isDown = ((lParam) & (1 << 31)) == 0;
            b32 wasDown = ((lParam) & (1 << 30)) != 0;
            if(isDown != wasDown)
            {
                key_code keyCode = KEY_NULL;
                
                switch(VKcode)
                {
                    case 'Q':
                    {
                        keyCode = KEY_Q;
                    } break;
                    case 'W':
                    {
                        keyCode = KEY_W;
                    } break;
                    case 'E':
                    {
                        keyCode = KEY_E;
                    } break;
                    case 'R':
                    {
                        keyCode = KEY_R;
                    } break;
                    case 'T':
                    {
                        keyCode = KEY_T;
                    } break;
                    case 'Y':
                    {
                        keyCode = KEY_Y;
                    } break;
                    case 'U':
                    {
                        keyCode = KEY_U;
                    } break;
                    case 'I':
                    {
                        keyCode = KEY_I;
                    } break;
                    case 'O':
                    {
                        keyCode = KEY_O;
                    } break;
                    case 'P':
                    {
                        keyCode = KEY_P;
                    } break;
                    case 'A':
                    {
                        keyCode = KEY_A;
                    } break;
                    case 'S':
                    {
                        keyCode = KEY_S;
                    } break;
                    case 'D':
                    {
                        keyCode = KEY_D;
                    } break;
                    case 'F':
                    {
                        keyCode = KEY_F;
                    } break;
                    case 'G':
                    {
                        keyCode = KEY_G;
                    } break;
                    case 'H':
                    {
                        keyCode = KEY_H;
                    } break;
                    case 'J':
                    {
                        keyCode = KEY_J;
                    } break;
                    case 'K':
                    {
                        keyCode = KEY_K;
                    } break;
                    case 'L':
                    {
                        keyCode = KEY_L;
                    } break;
                    case 'Z':
                    {
                        keyCode = KEY_Z;
                    } break;
                    case 'X':
                    {
                        keyCode = KEY_X;
                    } break;
                    case 'C':
                    {
                        keyCode = KEY_C;
                    } break;
                    case 'V':
                    {
                        keyCode = KEY_V;
                    } break;
                    case 'B':
                    {
                        keyCode = KEY_B;
                    } break;
                    case 'N':
                    {
                        keyCode = KEY_N;
                    } break;
                    case 'M':
                    {
                        keyCode = KEY_M;
                    } break;
                    case '1':
                    {
                        keyCode = KEY_1;
                    } break;
                    case '2':
                    {
                        keyCode = KEY_2;
                    } break;
                    case '3':
                    {
                        keyCode = KEY_3;
                    } break;
                    case '4':
                    {
                        keyCode = KEY_4;
                    } break;
                    case '5':
                    {
                        keyCode = KEY_5;
                    } break;
                    case '6':
                    {
                        keyCode = KEY_6;
                    } break;
                    case '7':
                    {
                        keyCode = KEY_7;
                    } break;
                    case '8':
                    {
                        keyCode = KEY_8;
                    } break;
                    case '9':
                    {
                        keyCode = KEY_9;
                    } break;
                    case '0':
                    {
                        keyCode = KEY_0;
                    } break;
                    case VK_UP:
                    {
                        keyCode = KEY_UP;
                    } break;
                    case VK_LEFT:
                    {
                        keyCode = KEY_LEFT;
                    } break;
                    case VK_RIGHT:
                    {
                        keyCode = KEY_RIGHT;
                    } break;
                    case VK_DOWN:
                    {
                        keyCode = KEY_DOWN;
                    } break;
                    case VK_ESCAPE:
                    {
                        keyCode = KEY_ESC;
                    } break;
                    case VK_SPACE:
                    {
                        keyCode = KEY_SPACE;
                    } break;
                    case VK_F11:
                    {
#ifdef FULLSCREEN
                        if(isDown)
                            ToggleFullscreen(window);
#endif
                    } break;
                }
                
                if(isDown)
                {
                    PushOSKeyEvent(&globalGameState.eventList,
                                   KEY_PRESS,
                                   keyCode);
                }
                else
                {
                    PushOSKeyEvent(&globalGameState.eventList,
                                   KEY_RELEASE,
                                   keyCode);
                }
                
                // NOTE(winston): maybe have an #ifndef F4_CLOSES_WINDOW here?
                b32 altKeyWasDown = ((lParam) & (1 << 29));
                if(altKeyWasDown && (VKcode == VK_F4))
                {
                    PushOSEvent(&globalGameState.eventList, 
                                WINDOW_CLOSE);
                }
            }
        } break;
        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }
    
    return result;
}

internal void
Win32_ProcessMessageQueue(HWND window)
{
    MSG message;
    while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

global LARGE_INTEGER performanceFrequency = {0};

internal f32 
GetTime_MS()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    
    f64 MS_elapsed = 
        ((f64)time.QuadPart / (f64)performanceFrequency.QuadPart) * 1000;
    
    return (f32)MS_elapsed;
}

internal void 
ThreadSleep(i32 numOfMS)
{
    Sleep((DWORD)numOfMS);
}

#ifdef QUEUE_TEST
internal void*
PrintWinston(void* placeholder)
{
    printf("Hi Winston!\n");
    return 0;
}
#endif 

internal OS_call
GenerateOSCalls()
{
    OS_call result = {0};
    
    result.GetTime_MS = GetTime_MS;
    result.ThreadSleep = ThreadSleep;
    
    return result;
}


//~ main function
int
WinMain(HINSTANCE hInstance, 
        HINSTANCE prevInstance,
        LPSTR CMDLine, 
        INT cmdShow)
{
    DisableBuffering(stdout);
    DisableBuffering(stderr);
    
    // NOTE(winston): Big boi memory alloc
    void* memory = VirtualAlloc(0,
                                BIG_BOI_ALLOC_SIZE,
                                MEM_COMMIT,
                                PAGE_READWRITE);
    
    DE_ClearFile(ERROR_LOG_PATH);
    
    if(!memory)
    {
        DE_LogError("Failed allocate memory.");
        return -1;
    }
    
    arena = new_arena(memory, BIG_BOI_ALLOC_SIZE);
    
    if(!arena.size)
    {
        DE_LogError("Failed to create memory arena.");
        return -1;
    }
    
    globalWin32BackBuffer = new_back_buffer(&arena, WIDTH, HEIGHT);
    
    WNDCLASSA windowClass = {0};
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Win32_DefaultWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.lpszClassName = "Window Class";
    
    QueryPerformanceFrequency(&performanceFrequency);
    
    if(RegisterClass(&windowClass))
    {
        DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | 
                         WS_SYSMENU | WS_MINIMIZEBOX | 
#ifdef MAXIMIZE
                         WS_MAXIMIZEBOX | 
#endif
                         WS_VISIBLE);
        
        HWND window = 
            CreateWindowExA(0,
                            "Window Class",
                            "A Dark Adventure",
                            dwStyle,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            WIDTH,
                            HEIGHT,
                            0,
                            0,
                            0,
                            0);
        
        if(window)
        {
            DWORD threadContext = 0;
            
            OS_call osCall = GenerateOSCalls();
            
            workerThreadQueue = 
                new_worker_thread_queue(&arena);
            
            workerThreadQueue.osCall = osCall;
            
#define WIN32_THREAD_FUNC_TEMPLATE DWORD (__cdecl*)(void*)
            HANDLE workerThread = CreateThread(0,
                                               0,
                                               (WIN32_THREAD_FUNC_TEMPLATE)ProcessWorkQueue,
                                               (void*)&workerThreadQueue,
                                               0,
                                               &threadContext);
#undef WIN32_THREAD_FUNC_TEMPLATE
            
            globalGameState.isRunning = 1;
            globalGameState.eventList = GenerateEventList(&arena, 64);
            
            f32 lastTime = GetTime_MS();
            
            back_buffer backBuffer = {0};
            backBuffer.memory = globalWin32BackBuffer.memory;
            backBuffer.height = globalWin32BackBuffer.height;
            backBuffer.width = globalWin32BackBuffer.width;
            backBuffer.pitch = globalWin32BackBuffer.pitch;
            backBuffer.bytesPerPixel = 4;
            
            HDC hdc = GetDC(window);
            char dllName[] = "game.dll";
            
            globalGameState.fpsCap = 60;
            
            game_code gameCode = Win32_LoadGameCode(&arena,
                                                    &backBuffer,
                                                    dllName,
                                                    &workerThreadQueue);
            
            gameCode.lastWriteTime = Win32_GetFileLastModifiedTime(dllName);
            f32 msCap = (1000.0f / (f32)globalGameState.fpsCap);
            
            //Win32_InitOpenGL(window);
            
            while(globalGameState.isRunning)
            {
                FILETIME currentWriteTime = Win32_GetFileLastModifiedTime(dllName);
                
                if(CompareFileTime(&gameCode.lastWriteTime,
                                   &currentWriteTime))
                {
                    Win32_UnloadGameCode(&gameCode,
                                         &arena,
                                         &backBuffer,
                                         &workerThreadQueue);
                    gameCode = Win32_LoadGameCode(&arena,
                                                  &backBuffer,
                                                  dllName,
                                                  &workerThreadQueue);
                }
                
                Win32_ProcessMessageQueue(window);
                
                gameCode.Game_UpdateAndRender(&globalGameState,
                                              &backBuffer,
                                              &arena,
                                              &workerThreadQueue);
                
                window_dimensions dimension = Win32_GetWindowDimensions(window);
                
                window_update_group* updateGroup = (window_update_group*)malloc(sizeof(window_update_group));
                
                updateGroup->hdc = hdc;
                updateGroup->backBuffer = &globalWin32BackBuffer;
                updateGroup->windowWidth = dimension.width;
                updateGroup->windowHeight = dimension.height;
                
                PushWorkQueue(&arena, 
                              &workerThreadQueue,
                              Win32_UpdateWindow,
                              updateGroup);
                
                f32 currentTime = GetTime_MS();
                f32 timeElapsed = currentTime - lastTime;
                
#ifdef FPS_CAP
                if(timeElapsed < msCap)
                {
                    Sleep((DWORD)(msCap - timeElapsed));
                }
#endif
                
#if 1
                char title[64];
                
                _snprintf_s(title, 64, 64, "A Dark Adventure - %.02f ms", timeElapsed);
                
                SetWindowTextA(window,
                               title);
#endif
                lastTime = currentTime;
            }
            
            DestroyWindow(window);
            
            workerThreadQueue.breakCommand = 1;
            WaitForSingleObject(workerThread, INFINITE);
            
            
            Win32_UnloadGameCode(&gameCode,
                                 &arena,
                                 &backBuffer,
                                 &workerThreadQueue);
            
            ReleaseDC(window, hdc);
        }
        else
        {
            DE_LogError("Failed to create window.");
        }
    }
    else
    {
        DE_LogError("Failed to register window class.");
    }
    
    VirtualFree(arena.memory, 0, MEM_RELEASE);
    
    return 0;
}
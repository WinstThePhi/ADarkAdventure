//#define ERROR_LOG_PATH "../data/platform_error.txt"
#define INFO_TO_STDOUT

#include "win32/win32_platform_include.h"

#ifndef WIDTH
#error "WIDTH is not defined.  Check game_options.h for define."
#endif

#ifndef HEIGHT
#error "HEIGHT is not defined.  Check game_options.h for define."
#endif

#ifndef BIG_BOI_ALLOC_SIZE
#error "BIG_BOI_ALLOC_SIZE is not defined.  Check game_options.h for define."
#endif 

#ifndef WINDOW_TITLE
#error "WINDOW_TITLE is not defined.  Check game_options.h for define."
#endif 

#ifndef FRAME_CAP
#error "Define FRAME_CAP in game_options.h"
#endif 

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib") 

global game_state globalGameState = {};
global WINDOWPLACEMENT g_wpPrev = {};
global os_call os = {};

internal void 
Win32_ToggleFullscreen(HWND window)
{
    g_wpPrev.length = sizeof(g_wpPrev);
    
    DWORD style = GetWindowLong(window, GWL_STYLE);
    if((style & WS_OVERLAPPEDWINDOW) && !globalGameState.isFullscreen) {
        MONITORINFO mi = {};
        mi.cbSize = sizeof(mi);
        
        if (GetWindowPlacement(window, &g_wpPrev) &&
            GetMonitorInfo(MonitorFromWindow(window,
                                             MONITOR_DEFAULTTOPRIMARY), 
                           &mi)) 
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
    FILETIME result = {};
    GetFileTime(fileHandle,
                0,
                0,
                &result);
    
    CloseHandle(fileHandle);
    
    return result;
}

internal game_code
Win32_LoadGameCode(memory_arena& localArena, 
                   char* dllName)
{
    game_code gameCodeLoad;
    
    char tempDLL[] = "temp.dll";
    
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
    
    gameCodeLoad.Game_Start(globalGameState,
                            localArena);
    
    gameCodeLoad.lastWriteTime = Win32_GetFileLastModifiedTime(dllName);
    
    return gameCodeLoad;
}

internal void
Win32_UnloadGameCode(game_code& gameCode,
                     memory_arena& localArena)
{
    gameCode.Game_End(globalGameState,
                      localArena);
    
    FreeLibrary(gameCode.gameCode);
    gameCode.gameCode = 0;
    gameCode.Game_UpdateAndRender = Game_UpdateAndRenderStub;
    gameCode.Game_Start = Game_StartStub;
    gameCode.Game_End = Game_EndStub;
}

internal window_dimensions
Win32_GetWindowDimensions(HWND window)
{
    RECT dataRect = {};
    GetClientRect(window,
                  &dataRect);
    
    window_dimensions dimension = {};
    dimension.width = (u16)(dataRect.right - dataRect.left);
    dimension.height = (u16)(dataRect.bottom - dataRect.top);
    
    return dimension;
}

internal void*
Win32_ClearWindow()
{
    return 0;
}

internal void*
Win32_UpdateWindow(void* temp)
{
    return 0;
}

internal LRESULT 
Win32_DefaultWindowCallback(HWND window,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam)
{
    LRESULT result = {};
    
    switch(message)
    {
        case WM_CLOSE:
        {
            PushOSEvent(&globalGameState.eventList,
                        OSEvent(WINDOW_CLOSE));
        } break;
        case WM_DESTROY:
        {
            PushOSEvent(&globalGameState.eventList,
                        OSEvent(WINDOW_CLOSE));
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
            window_dimensions dimension = 
                Win32_GetWindowDimensions(window);
            
            PAINTSTRUCT paintStruct = {};
            HDC paintDC = BeginPaint(window,
                                     &paintStruct);
            
            Win32_UpdateWindow(&paintDC);
            EndPaint(window,
                     &paintStruct);
        } break;
        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            Assert(0 && "Key input went through filter.");
        } break;
        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }
    
    return result;
}

internal void*
Win32_ProcessMessageQueue(void* temp)
{
    HWND window = ((window_group*)temp)->window;
    MSG message;
    while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
                PushOSEvent(&globalGameState.eventList,
                            OSEvent(WINDOW_CLOSE));
            } break;
            case WM_SYSKEYUP:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_KEYDOWN:
            {
                u16 VKcode = (u16)message.wParam;
                b32 isDown = ((message.lParam) & (1 << 31)) == 0;
                b32 wasDown = ((message.lParam) & (1 << 30)) != 0;
                if(isDown != wasDown)
                {
                    key_code keyCode = KEY_NULL;
                    
                    switch(VKcode)
                    {
                        
#define Key(keyID) \
case VK_##keyID: \
{ \
keyCode = KEY_##keyID; \
} break;
                        
#include "key_list.inc"
#undef Key
                    }
                    
                    if(isDown)
                    {
                        if(keyCode == KEY_F11)
                        {
                            Win32_ToggleFullscreen(window);
                        }
                        
                        PushOSEvent(&globalGameState.eventList,
                                    KeyEvent(keyCode, KEY_PRESS));
                    }
                    else
                    {
                        PushOSEvent(&globalGameState.eventList,
                                    KeyEvent(keyCode, KEY_RELEASE));
                    }
                    
                    // NOTE(winston): maybe have an #ifndef F4_CLOSES_WINDOW here?
                    b32 altKeyWasDown = ((message.lParam) & (1 << 29));
                    if(altKeyWasDown && (VKcode == VK_F4))
                    {
                        PushOSEvent(&globalGameState.eventList, 
                                    OSEvent(WINDOW_CLOSE));
                    }
                }
            } break;
            default:
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            } break;
        }
    }
    return 0;
}

internal void 
ThreadSleep(f32 numOfMS)
{
    Sleep((DWORD)numOfMS);
}

internal os_call
GenerateOSCalls()
{
    os_call result = {};
    
    result.GetTime_MS = GetTime_MS;
    result.ThreadSleep = ThreadSleep;
    
    return result;
}

//~ NOTE(winston): main function

i32 WinMain(HINSTANCE hInstance, 
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
    
    //DE_ClearFile(ERROR_LOG_PATH);
    
    if(!memory)
    {
        DE_LogError("Failed allocate memory.");
        QUIT();
    }
    
    memory_arena arena(memory, BIG_BOI_ALLOC_SIZE);
    
    WNDCLASSA windowClass = {};
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Win32_DefaultWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.lpszClassName = "Window Class";
    
    QueryPerformanceFrequency(&performanceFrequency);
    
    if(RegisterClass(&windowClass))
    {
        DWORD dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_THICKFRAME);
        
        RECT windowRect = {0, 0, WIDTH, HEIGHT};
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_THICKFRAME, FALSE);
        
        HWND window = 
            CreateWindowExA(0,
                            windowClass.lpszClassName,
                            WINDOW_TITLE,
                            dwStyle,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            windowRect.right - windowRect.left,
                            windowRect.bottom - windowRect.top,
                            0,
                            0,
                            0,
                            0);
        
        if(window)
        {
            DWORD threadContext = 0;
#if 0
            worker_thread_queue* queue = Win32_NewWorkerThreadQueue(&arena);
            Win32_DestroyWorkerThreadQueue(queue);
#endif 
            os = GenerateOSCalls();
            
            globalGameState.isRunning = 1;
            globalGameState.eventList = GenerateEventList(arena, 64);
            
            // NOTE(winston): backBuffer member fill out
            
            HDC hdc = GetDC(window);
            
            d3d_group d3d(window);
            
            char dllName[] = "game.dll";
            
            game_code gameCode = Win32_LoadGameCode(arena,
                                                    dllName);
            
            gameCode.lastWriteTime = Win32_GetFileLastModifiedTime(dllName);
            f32 msCap = (1000.0f / (f32)FRAME_CAP);
            
            window_group* windowGroup = 
                Arena_PushStruct(arena, window_group);
            
            windowGroup->window = window;
            
            timer_info timer = new_timer_info();
            
            globalGameState.fpsCap = FRAME_CAP;
            
            while(globalGameState.isRunning)
            {
                Win32_BeginFrame(&timer);
                
                FILETIME currentWriteTime = 
                    Win32_GetFileLastModifiedTime(dllName);
                
                if(CompareFileTime(&gameCode.lastWriteTime,
                                   &currentWriteTime))
                {
                    Win32_UnloadGameCode(gameCode,
                                         arena);
                    gameCode = Win32_LoadGameCode(arena,
                                                  dllName);
                }
                
                window_dimensions dimension = 
                    Win32_GetWindowDimensions(window);
                
                //Win32_ClearWindow();
                d3d.Clear(v3_color(0, 168, 255));
                
                Win32_ProcessMessageQueue((void*)windowGroup);
                
                ProcessOSMessages(&globalGameState);
                
                D3D11_VIEWPORT viewport = {0.0f, 0.0f, (FLOAT)dimension.width, (FLOAT)dimension.height, 0.0f, 1.0f};
                d3d.deviceContext->RSSetViewports(1, &viewport);
                
                d3d.deviceContext->OMSetRenderTargets(1, &d3d.renderTargetView, 0);
                
                d3d.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                d3d.deviceContext->IASetInputLayout(d3d.inputLayout);
                d3d.deviceContext->IASetVertexBuffers(0, 1, &d3d.vertexBuffer, &d3d.vertexStride, &d3d.vertexOffset);
                
                d3d.deviceContext->VSSetShader(d3d.vertexShader, 0, 0);
                d3d.deviceContext->PSSetShader(d3d.pixelShader, 0, 0);
                
                d3d.deviceContext->Draw(d3d.vertexCount, 0);
                
                gameCode.Game_UpdateAndRender(globalGameState,
                                              arena);
                
                //Win32_UpdateWindow(&hdc);
                d3d.UpdateWindow();
                
                Win32_EndFrame(&timer, msCap);
            }
            
            DestroyWindow(window);
            
#if 0
            WaitForSingleObject(workerThread, INFINITE);
#endif
            Win32_UnloadGameCode(gameCode,
                                 arena);
            
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
    
    DE_LogInfo("All memory has been freed and program ended without a crash.");
    
    return 0;
}
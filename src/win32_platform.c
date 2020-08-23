#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "layer.h"
#include "platform_interface.h"
#include "game.h"
#include "memory.h"
#include "win32_platform.h"
#include "util.h"

global game_state globalGameState;
global win32_back_buffer globalWin32BackBuffer;

#include "memory.c"
#include "platform_interface.c"
#include "game.c"

#define BIG_BOI_ALLOC_SIZE Megabytes(20)

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
new_back_buffer(MemoryArena* arena,
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
    backBuffer.memory = ArenaAlloc(arena, sizeToAlloc);
    backBuffer.pitch = width * BYTES_PER_PIXEL;
    
    return backBuffer;
}

internal void
Win32_UpdateWindow(HDC hdc,
                   win32_back_buffer* backBuffer,
                   u16 windowWidth,
                   u16 windowHeight)
{
    StretchDIBits(hdc,
                  0, 0,
                  windowWidth,
                  windowHeight,
                  0, 0,
                  backBuffer->width,
                  backBuffer->height,
                  backBuffer->memory,
                  &backBuffer->bitmapInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
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
        case WM_PAINT:
        {
            window_dimensions dimension = Win32_GetWindowDimensions(window);
            
            PAINTSTRUCT paintStruct = {0};
            HDC paintDC = BeginPaint(window,
                                     &paintStruct);
            Win32_UpdateWindow(paintDC,
                               &globalWin32BackBuffer,
                               dimension.width,
                               dimension.height);
            
            EndPaint(window,
                     &paintStruct);
        } break;
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
                    case 'W':
                    {
                        keyCode = KEY_W;
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
                    case VK_UP:
                    {
                        keyCode = KEY_UP;
                    } break;
                    case VK_LEFT:
                    {
                        keyCode = KEY_LEFT;
                    } break;
                    case VK_DOWN:
                    {
                        keyCode = KEY_DOWN;
                    } break;
                    case VK_RIGHT:
                    {
                        keyCode = KEY_RIGHT;
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

inline f32 
GetTime_MS(LARGE_INTEGER performanceFrequency)
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    
    f64 MS_elapsed = 
        ((f64)time.QuadPart / (f64)performanceFrequency.QuadPart) * 1000;
    
    return (f32)MS_elapsed;
}

int
WinMain(HINSTANCE hInstance, 
        HINSTANCE prevInstance,
        LPSTR CMDLine, 
        INT cmdShow)
{
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
    
    // NOTE(winston): Big boi memory alloc
    void* memory = VirtualAlloc(0,
                                BIG_BOI_ALLOC_SIZE,
                                MEM_COMMIT,
                                PAGE_READWRITE);
    
    MemoryArena arena = new_arena(memory, BIG_BOI_ALLOC_SIZE);
    
    globalWin32BackBuffer = new_back_buffer(&arena, 1280, 720);
    
    WNDCLASSA windowClass = {0};
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Win32_DefaultWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.lpszClassName = "Window Class";
    
    
    LARGE_INTEGER performanceFrequency;
    QueryPerformanceFrequency(&performanceFrequency);
    
    if(RegisterClass(&windowClass))
    {
        HWND window = 
            CreateWindowExA(0,
                            "Window Class",
                            "A Dark Adventure",
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            1280,
                            720,
                            0,
                            0,
                            0,
                            0);
        if(window)
        {
            globalGameState.isRunning = 1;
            globalGameState.eventList = GenerateEventList(&arena, 64);
            
            f32 lastTime = GetTime_MS(performanceFrequency);
            
            back_buffer backBuffer = {0};
            backBuffer.memory = globalWin32BackBuffer.memory;
            backBuffer.height = globalWin32BackBuffer.height;
            backBuffer.width = globalWin32BackBuffer.width;
            backBuffer.pitch = globalWin32BackBuffer.pitch;
            
            HDC hdc = GetDC(window);
            
            while(globalGameState.isRunning)
            {
                Win32_ProcessMessageQueue(window);
                ProcessOSMessages(&globalGameState);
                
                Game_UpdateAndRender(&globalGameState,
                                     &backBuffer);
                
                window_dimensions dimension = Win32_GetWindowDimensions(window);
                
#if 1
                Win32_UpdateWindow(hdc,
                                   &globalWin32BackBuffer,
                                   dimension.width,
                                   dimension.height);
#endif 
                //UpdateWindow(window);
                
                f32 currentTime = GetTime_MS(performanceFrequency);
                f32 timeElapsed = currentTime - lastTime;
                
                //printf("%f\n", timeElapsed);
                if(timeElapsed < 16.667f)
                {
                    Sleep((DWORD)(16.667f - timeElapsed));
                }
                
                lastTime = currentTime;
            }
            
            DestroyWindow(window);
        }
        else
        {
            // TODO(winston): logging
            fprintf(stderr, "Failed to create window.\n");
        }
    }
    else
    {
        // TODO(winston): logging
        fprintf(stderr, "Failed to register window class.\n");
    }
    
    VirtualFree(arena.memory, 0, MEM_RELEASE);
    
    return 0;
}
#include <windows.h>
#include <stdio.h>

#include "layer.h"

global b8 isRunning;

internal LRESULT 
Win32DefaultWindowCallback(HWND window,
                           UINT message,
                           WPARAM wParam,
                           LPARAM lParam)
{
    LRESULT result;
    
    switch(message)
    {
        case WM_QUIT:
        {
            isRunning = 0;
        } break;
        case WM_CLOSE:
        {
            isRunning = 0;
        } break;
        case WM_DESTROY:
        {
            isRunning = 0;
        } break;
        case WM_PAINT:
        {
            HDC hdc = GetDC(window);
            RECT clientRect;
            GetClientRect(window, &clientRect);
            
            PAINTSTRUCT paintStruct = {0};
            paintStruct.hdc = hdc;
            paintStruct.fErase = 1;
            paintStruct.rcPaint = clientRect;
            HDC paintDC = BeginPaint(window,
                                     &paintStruct);
            
            u16 width = clientRect.right - clientRect.left;
            u16 height = clientRect.bottom - clientRect.top;
            
            PatBlt(paintDC, 0, 0, width, height, BLACKNESS);
            
            EndPaint(window,
                     &paintStruct);
            
            ReleaseDC(window, hdc);
        } break;
        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }
    
    return result;
}

internal void
Win32ProcessMessageQueue(HWND window)
{
    MSG message;
    while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            default:
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            } break;
        }
    }
}

int 
WinMain(HINSTANCE hInstance, 
        HINSTANCE prevInstance,
        LPSTR CMDLine, 
        INT cmdShow)
{
    WNDCLASSA windowClass = {0};
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Win32DefaultWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.lpszClassName = "Window Class";
    
    if(RegisterClass(&windowClass))
    {
        HWND window = 
            CreateWindowExA(0,
                            "Window Class",
                            "A Dark Adventure",
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            0,
                            0,
                            0,
                            0);
        if(window)
        {
            isRunning = 1;
            
            while(isRunning)
            {
                Win32ProcessMessageQueue(window);
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
    
    return 0;
}
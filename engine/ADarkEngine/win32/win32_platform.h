#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#define VK_Q 'Q'
#define VK_W 'W'
#define VK_E 'E'
#define VK_R 'R'
#define VK_T 'T'
#define VK_Y 'Y'
#define VK_U 'U'
#define VK_I 'I'
#define VK_O 'O'
#define VK_P 'P'
#define VK_A 'A'
#define VK_S 'S'
#define VK_D 'D'
#define VK_F 'F'
#define VK_G 'G'
#define VK_H 'H'
#define VK_J 'J'
#define VK_K 'K'
#define VK_L 'L'
#define VK_Z 'Z'
#define VK_X 'X'
#define VK_C 'C'
#define VK_V 'V'
#define VK_B 'B'
#define VK_N 'N'
#define VK_M 'M'

#define VK_1 '1'
#define VK_2 '2'
#define VK_3 '3'
#define VK_4 '4'
#define VK_5 '5'
#define VK_6 '6'
#define VK_7 '7'
#define VK_8 '8'
#define VK_9 '9'
#define VK_0 '0'

#define QUIT() \
return -1

#define Win32_LargeToI64(num) num##.QuadPart

#define BYTES_PER_PIXEL 4

typedef struct win32_back_buffer
{
    BITMAPINFO bitmapInfo;
    
    void* memory;
    
    u16 height;
    u16 width;
    
    u16 pitch;
} win32_back_buffer;

typedef struct window_dimensions
{
    u16 width;
    u16 height;
} window_dimensions;

typedef struct game_code
{
    HMODULE gameCode;
    FILETIME lastWriteTime;
    
    game_update_and_render* Game_UpdateAndRender;
    start_game* Game_Start;
    end_game* Game_End;
    
} game_code;

typedef struct window_update_group
{
    HDC hdc;
    win32_back_buffer* backBuffer;
    u16 windowWidth;
    u16 windowHeight;
} window_update_group;

typedef struct window_group
{
    HWND window;
} window_group;


internal void ToggleFullscreen(HWND window);

internal FILETIME Win32_GetFileLastModifiedTime(char* filename);

internal game_code Win32_LoadGameCode(memory_arena* localArena, 
                                      char* dllName,
                                      worker_thread_queue* queue);

internal void Win32_UnloadGameCode(game_code* gameCode,
                                   memory_arena* localArena,
                                   worker_thread_queue* queue);

internal window_dimensions Win32_GetWindowDimensions(HWND window);

internal win32_back_buffer new_back_buffer(memory_arena* localArena,
                                           u16 width, 
                                           u16 height);

internal void* Win32_UpdateWindow(void* temp);

internal LRESULT Win32_DefaultWindowCallback(HWND window,
                                             UINT message,
                                             WPARAM wParam,
                                             LPARAM lParam);

internal void* Win32_ProcessMessageQueue(void* temp);

internal void ThreadSleep(f32 numOfMS);

internal os_call GenerateOSCalls();

#endif

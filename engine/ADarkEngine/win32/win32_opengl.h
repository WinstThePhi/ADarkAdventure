#ifndef WIN32_OPENGL_H
#define WIN32_OPENGL_H

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

#define WGL_CHOOSE_PIXEL_FORMAT_ARB(name) \
BOOL name(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats)
#define WGL_CREATE_CONTEXT_ATTRIBS_ARB(name) \
HGLRC name(HDC hDC, HGLRC hShareContext, const int *attribList)
#define WGL_MAKE_CONTEXT_CURRENT_ARB(name) \
BOOL name(HDC hDrawDC, HDC hReadDC, HGLRC hglrc)
#define WGL_SWAP_INTERVAL_EXT(name) \
BOOL name(int interval)

typedef WGL_CHOOSE_PIXEL_FORMAT_ARB(wgl_choose_pixel_format_arb);
typedef WGL_CREATE_CONTEXT_ATTRIBS_ARB(wgl_create_context_attribs_arb);
typedef WGL_MAKE_CONTEXT_CURRENT_ARB(wgl_make_context_current_arb);
typedef WGL_SWAP_INTERVAL_EXT(wgl_swap_interval_ext);

global HGLRC globalRenderingContext;
global HMODULE openglModule;
global gl_procedures gl;
global os_call os = {};

global wgl_choose_pixel_format_arb*    wglChoosePixelFormatARB;
global wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
global wgl_make_context_current_arb*   wglMakeContextCurrentARB;
global wgl_swap_interval_ext*          wglSwapIntervalEXT;

#define IS_NOT_LOADED(condition)  \
condition == 0          ||    \
condition == (void*)0x1 ||    \
condition == (void*)0x2 ||    \
condition == (void*)0x3 ||    \
condition == (void*)-1

#define LOAD_WGL_FUNCTION(type, name) \
name = (type*)Win32_LoadOpenGLProcedure(#name); \
if(!name) \
return 0

internal void* Win32_LoadOpenGLProcedure(char* procedureName);

internal b32 Win32_LoadNecessaryWGLFunctions();

internal b32 Win32_InitOpenGL(HWND window, HINSTANCE hInstance);

internal void Win32_EndOpenGL();

#endif

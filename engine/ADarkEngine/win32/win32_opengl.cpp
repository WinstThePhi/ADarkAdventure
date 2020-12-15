
#include "win32/win32_opengl.h"

internal void*
Win32_LoadOpenGLProcedure(char* procedureName)
{
    void* result = (void*)wglGetProcAddress(procedureName);
    char buffer[64];
    
    if(IS_NOT_LOADED(result)) 
    {
        result = GetProcAddress(openglModule, procedureName);
        
        if(IS_NOT_LOADED(result))
        {
            sprintf_s(buffer, 64,
                      "OpenGL failed to load function: %s", procedureName);
            
            DE_LogError(buffer);
        }
        else
        {
            goto LOADED;
        }
        
        return 0;
    }
    else
    {
        LOADED:;
#if 0
        sprintf_s(buffer, 64,
                  "OpenGL loaded function: %s", procedureName);
        DE_LogInfo(buffer);
#endif
        return result;
    }
}

internal b32 
Win32_LoadNecessaryWGLFunctions()
{
    LOAD_WGL_FUNCTION(wgl_choose_pixel_format_arb, wglChoosePixelFormatARB);
    LOAD_WGL_FUNCTION(wgl_create_context_attribs_arb, wglCreateContextAttribsARB);
    LOAD_WGL_FUNCTION(wgl_make_context_current_arb, wglMakeContextCurrentARB);
    LOAD_WGL_FUNCTION(wgl_swap_interval_ext, wglSwapIntervalEXT);
    return 1;
}

internal b32
Win32_InitOpenGL(HWND window, HINSTANCE hInstance)
{
    b32 returnCode = 0;
    
    openglModule = LoadLibraryA("opengl32.dll");
    
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0, 0, 0,
        0,
        0, 0, 0, 0,
        24, 
        8, 
        0, 
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    HDC hdc = GetDC(window);
    
    i32 pixelFormat = 
        ChoosePixelFormat(hdc, &pixelFormatDescriptor);
    
    if(pixelFormat)
    {
        SetPixelFormat(hdc,
                       pixelFormat,
                       &pixelFormatDescriptor);
        
        HGLRC tempRenderingContext = wglCreateContext(hdc);
        wglMakeCurrent(hdc, tempRenderingContext);
        
        if(Win32_LoadNecessaryWGLFunctions())
        {
            {
                i32 attribs[] =
                {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                    WGL_COLOR_BITS_ARB, 32,
                    WGL_DEPTH_BITS_ARB, 24,
                    WGL_STENCIL_BITS_ARB, 8,
                    0
                };
                
                u32 numFormat = 0;
                wglChoosePixelFormatARB(hdc, 
                                        attribs,
                                        0,
                                        1,
                                        &pixelFormat,
                                        &numFormat);
            }
            if(pixelFormat)
            {
                const i32 contextAttribs[] =
                {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                    0
                };
                
                globalRenderingContext = 
                    wglCreateContextAttribsARB(hdc, 
                                               tempRenderingContext,
                                               contextAttribs);
                
                if(globalRenderingContext)
                {
                    wglMakeCurrent(hdc, 0);
                    wglDeleteContext(tempRenderingContext);
                    wglMakeCurrent(hdc, globalRenderingContext);
                    wglSwapIntervalEXT(0);
                }
                
                if(DE_LoadAllOpenGLProcedures(&os, &gl))
                {
                    returnCode = 1;
                }
            }
        }
        else
        {
            DE_LogError("Failed to load WGL functions.");
        }
    }
    else
    {
        DE_LogError("Pixel format not chosen.");
    }
    
    ReleaseDC(window, hdc);
    
    return returnCode;
}


internal void 
Win32_CleanUpOpenGL(HWND window)
{
    HDC hdc = GetDC(window);
    
    wglMakeCurrent(hdc, 0);
    wglDeleteContext(globalRenderingContext);
    
    ReleaseDC(window, hdc);
    
    gl_procedures emptyGL = {};
    gl = emptyGL;
    
    FreeLibrary(openglModule);
}


internal void
Win32_InitOpenGL(HWND window)
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    HDC hdc = GetDC(window);
    
    i32 pixelFormat = ChoosePixelFormat(hdc, &pfd); 
    
    SetPixelFormat(hdc, pixelFormat, &pfd);
    
    HGLRC context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, context);
    
    MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
}
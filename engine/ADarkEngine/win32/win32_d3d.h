#ifndef WIN32_D3D_H
#define WIN32_D3D_H

struct d3d_group
{
    ID3D11Device* device                     = 0;
    ID3D11DeviceContext* deviceContext       = 0;
    IDXGISwapChain* swapChain                = 0;
    ID3D11RenderTargetView* renderTargetView = 0;
    
    ID3D11Buffer* vertexBuffer = 0;
    ID3D11InputLayout* inputLayout = 0;
    ID3D11VertexShader* vertexShader = 0;
    ID3D11PixelShader* pixelShader = 0;
    
    u32 vertexStride = 3 * sizeof(f32);
    u32 vertexOffset = 0;
    u32 vertexCount = 3;
    
    d3d_group(HWND window);
    ~d3d_group();
    void Clear(v3 color);
    void UpdateWindow();
};

#endif

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#include "win32_d3d.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


d3d_group::d3d_group(HWND window)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.Windowed = 0;
    
    D3D_FEATURE_LEVEL featureLevel;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
    
    HRESULT result = 
        D3D11CreateDeviceAndSwapChain(0, 
                                      D3D_DRIVER_TYPE_HARDWARE,
                                      0, 
                                      flags,
                                      0,
                                      0,
                                      D3D11_SDK_VERSION,
                                      &swapChainDesc,
                                      &swapChain,
                                      &device, 
                                      &featureLevel,
                                      &deviceContext);
    
    Assert(result == S_OK && swapChain && device && deviceContext);
    
    ID3D11Texture2D* frameBuffer;
    result = 
        swapChain->GetBuffer(0, 
                             __uuidof(ID3D11Texture2D),
                             (void**)&frameBuffer);
    Assert(SUCCEEDED(result));
    
    result = device->CreateRenderTargetView(frameBuffer, 0, &renderTargetView);
    Assert(SUCCEEDED(result));
    
    frameBuffer->Release();
    
    UINT flag = D3DCOMPILE_ENABLE_STRICTNESS;
    
    ID3DBlob* vsBlob = 0;
    ID3DBlob* psBlob = 0;
    ID3DBlob* errorBlob = 0;
    
    result = D3DCompileFromFile(L"../shaders/shaders.hlsl",
                                nullptr,
                                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                "vs_main",
                                "vs_5_0",
                                flag, 
                                0,
                                &vsBlob,
                                &errorBlob);
    
    if(FAILED(result))
    {
        if(errorBlob)
        {
            DE_LogError((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        
        if(vsBlob)
            vsBlob->Release();
        
        Assert(0);
    }
    
    result = D3DCompileFromFile(L"../shaders/shaders.hlsl",
                                nullptr,
                                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                "ps_main",
                                "ps_5_0",
                                flag, 
                                0,
                                &psBlob,
                                &errorBlob);
    
    if(FAILED(result))
    {
        if(errorBlob)
        {
            DE_LogError((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        
        if(psBlob)
            psBlob->Release();
        
        Assert(0);
    }
    
    result = device->CreateVertexShader(vsBlob->GetBufferPointer(), 
                                        vsBlob->GetBufferSize(), 
                                        0, 
                                        &vertexShader);
    result = device->CreatePixelShader(psBlob->GetBufferPointer(), 
                                       psBlob->GetBufferSize(), 
                                       0, 
                                       &pixelShader);
    Assert(SUCCEEDED(result));
    
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
    {
        {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    
    result = device->CreateInputLayout(inputElementDesc, 
                                       ARRAYSIZE(inputElementDesc),
                                       vsBlob->GetBufferPointer(),
                                       vsBlob->GetBufferSize(), 
                                       &inputLayout);
    
    vsBlob->Release();
    psBlob->Release();
    
    Assert(SUCCEEDED(result));
    
    f32 vertexData[] =
    {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    {
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(vertexData);
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA srData = {};
        srData.pSysMem = vertexData;
        result = device->CreateBuffer(&vertexBufferDesc, 
                                      &srData, 
                                      &vertexBuffer);
        
        Assert(SUCCEEDED(result));
    }
}

void d3d_group::Clear(v3 color)
{
    float backgroundColor[4] = {(f32)color.r / 255.0f, (f32)color.g / 255.0f, (f32)color.b / 255.0f, 1.0f};
    deviceContext->ClearRenderTargetView(renderTargetView, backgroundColor);
}

void d3d_group::UpdateWindow()
{
    swapChain->Present(1, 0);
}

d3d_group::~d3d_group()
{
    device->Release();
    deviceContext->Release();
    swapChain->Release();
    renderTargetView->Release();
    vertexBuffer->Release();
    inputLayout->Release();
    vertexShader->Release();
    pixelShader->Release();
}
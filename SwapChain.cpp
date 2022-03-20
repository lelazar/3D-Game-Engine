#include "SwapChain.h"
#include "GraphicsEngine.h"

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
    // The device interface represents a virtual adapter; it is used to create resources.
    ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;

    // Describing the swap chain
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    // We need a Front Buffer and a Back Buffer
    desc.BufferCount = 1;  // A value that describes the number of buffers in the swap chain. When you call IDXGIFactory::CreateSwapChain to create a full-screen swap chain, 
                           // you typically include the front buffer in this value.
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // The pixel format of the frame buffers of our swap chain
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // Treating the buffers as 'render target'
    desc.OutputWindow = hwnd;  // An HWND handle to the output window. This member must not be NULL.
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;  // A Boolean value that specifies whether the output is in windowed mode. TRUE if the output is in windowed mode; otherwise, FALSE.

    // Creating the swap chain for the window, indicated by HWND parameter
    HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

    if (FAILED(hr))
        return false;

    ID3D11Texture2D* buffer = NULL;
    hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

    if (FAILED(hr))
        return false;

    hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
    buffer->Release();

    if (FAILED(hr))
        return false;

    return true;
}

bool SwapChain::present(bool vsync)
{
    m_swap_chain->Present(vsync, NULL);

    return true;
}

bool SwapChain::release()
{
    m_swap_chain->Release();
    delete this;
    return true;
}

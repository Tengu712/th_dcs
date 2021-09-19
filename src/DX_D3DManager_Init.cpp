#include "../include/HeaderDX11.hpp"
#include "../include/pshader.h"
#include "../include/vshader.h"

LRESULT WINAPI WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool D3DManager::init(HINSTANCE hInst, int cmdShow, LPCWSTR nameWnd, LPCWSTR nameWndClass, unsigned int width,
    unsigned int height, bool windowed) {
    try {

        // Create Window
        {
            const DWORD kDwStyle = windowed ? WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX : WS_POPUP;
            const DWORD kDwExStyle = windowed ? 0 : WS_EX_TOPMOST;
            const int kCmdShow = windowed ? SW_SHOW : SW_SHOWMAXIMIZED;

            WNDCLASSEXW wcex = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInst, nullptr, nullptr, nullptr,
                nullptr, nameWndClass, nullptr};
            if (!RegisterClassExW(&wcex))
                throw "Error: Failed to register window class.";

            RECT rect;
            rect = {0, 0, (long)width, (long)height};
            AdjustWindowRectEx(&rect, kDwStyle, false, kDwExStyle);

            hWnd = CreateWindowExW(kDwExStyle, nameWndClass, nameWnd, kDwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInst, nullptr);
            if (!hWnd)
                throw "Error: Failed to create window handle.";

            ShowWindow(hWnd, kCmdShow);
            UpdateWindow(hWnd);
            ShowCursor(windowed);
        }

        // Create Factory
        ComPtr<IDXGIFactory> pFactory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
            throw "Error: Failed to create factory.";

        // Create Device
        {
            D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
            };
            if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                    featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, inf.pDevice.GetAddressOf(), nullptr,
                    inf.pImContext.GetAddressOf())))
                throw "Error: Failed to create Direct3D11 device.";
        }

        // Create SwapChain
        {
            DXGI_SWAP_CHAIN_DESC descSwapChain = {
                {width, height, {60U, 1U}, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                    DXGI_MODE_SCALING_UNSPECIFIED},
                {1, 0}, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1U, hWnd, true, DXGI_SWAP_EFFECT_DISCARD,
                DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH};

            if (FAILED(pFactory->CreateSwapChain(inf.pDevice.Get(), &descSwapChain, inf.pSwapChain.GetAddressOf())))
                throw "Error: Failed to create swapchain.";
        }

        // Create Render Target
        {
            ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
            if (FAILED(inf.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf())))
                throw "Error: Failed to get back buffer.";

            if (FAILED(inf.pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, inf.pRTView.GetAddressOf())))
                throw "Error: Failed to create render target view.";
        }

        // Create Depth Stencil Buffer
        {
            D3D11_DEPTH_STENCIL_DESC descDS = {true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false,
                D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, D3D11_STENCIL_OP_KEEP,
                D3D11_STENCIL_OP_KEEP};

            ComPtr<ID3D11DepthStencilState> pStateDS = nullptr;
            if (FAILED(inf.pDevice->CreateDepthStencilState(&descDS, pStateDS.GetAddressOf())))
                throw "Error: Failed to create depth stancil stencil.";
            inf.pImContext->OMSetDepthStencilState(pStateDS.Get(), 0U);

            D3D11_TEXTURE2D_DESC descDSTex = {width, height, 1U, 1U, DXGI_FORMAT_R24G8_TYPELESS, {1, 0},
                D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0U, 0U};

            ComPtr<ID3D11Texture2D> pDSTex = nullptr;
            if (FAILED(inf.pDevice->CreateTexture2D(&descDSTex, nullptr, pDSTex.GetAddressOf())))
                throw "Error: Failed to create depth stencil buffer texture.";

            D3D11_DEPTH_STENCIL_VIEW_DESC descView = {
                DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_DSV_DIMENSION_TEXTURE2D, 0U, {0U}};
            if (FAILED(inf.pDevice->CreateDepthStencilView(pDSTex.Get(), &descView, inf.pDSView.GetAddressOf())))
                throw "Error: Failed to create depth stencil buffer view.";
        }

        // Create Viewport
        inf.viewport = {0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f};

        // Create Shader
        if (FAILED(inf.pDevice->CreateVertexShader(g_vs_main, sizeof(g_vs_main), nullptr, inf.pVShader.GetAddressOf())))
            throw "Error: Failed to create vertex shader.";
        if (FAILED(inf.pDevice->CreatePixelShader(g_ps_main, sizeof(g_ps_main), nullptr, inf.pPShader.GetAddressOf())))
            throw "Error: Failed to create pixel shader.";

        // Create input element desc
        {
            D3D11_INPUT_ELEMENT_DESC descElem[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                    D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0},
            };
            if (FAILED(inf.pDevice->CreateInputLayout(
                    descElem, ARRAYSIZE(descElem), g_vs_main, sizeof(g_vs_main), inf.pILayout.GetAddressOf())))
                return false;
        }

        // Create constant buffer
        {
            D3D11_BUFFER_DESC descCB = {
                sizeof(ConstantBuffer), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0U, 0U, 0U};
            if (FAILED(inf.pDevice->CreateBuffer(&descCB, nullptr, inf.pCBuffer.GetAddressOf())))
                throw "Error: Failed to create constant buffer.";

            DirectX::XMStoreFloat4x4(&inf.cbuffer.matScl, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f)));
            DirectX::XMStoreFloat4x4(&inf.cbuffer.matRot, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)));
            DirectX::XMStoreFloat4x4(&inf.cbuffer.matTrs, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)));   
            DirectX::XMStoreFloat4x4(&inf.cbuffer.matView, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixLookToLH(
                    DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
                    DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                    DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
            DirectX::XMStoreFloat4x4(&inf.cbuffer.matProj, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)width / (float)height, 1.0f, 2000.0f)));
            DirectX::XMStoreFloat4(&inf.cbuffer.vecColor, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
            DirectX::XMStoreFloat4(&inf.cbuffer.vecLight, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
            DirectX::XMStoreFloat4(&inf.cbuffer.params, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

            inf.pImContext->UpdateSubresource(inf.pCBuffer.Get(), 0U, nullptr, &inf.cbuffer, 0U, 0U);
            inf.pImContext->VSSetConstantBuffers(0U, 1U, inf.pCBuffer.GetAddressOf());
        }

        // Set blend mode
        {
            D3D11_BLEND_DESC descBlend;
            ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
            descBlend.AlphaToCoverageEnable = false;
            descBlend.IndependentBlendEnable = false;
            descBlend.RenderTarget[0].BlendEnable = true;
            descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

            ComPtr<ID3D11BlendState> pBlendState = nullptr;
            if (FAILED(inf.pDevice->CreateBlendState(&descBlend, pBlendState.GetAddressOf())))
                throw "Failed to create blend state.";
            float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
            inf.pImContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);
        }

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Warning", MB_OK);
        return false;
    }

    return true;
}

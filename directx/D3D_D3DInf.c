#include "../include/HeaderD3D11.h"
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

BOOL CreateD3DInf(struct D3DInf* pDinf, HINSTANCE hInst, LPCWSTR nameWnd, LPCWSTR nameWndClass,
        unsigned int width, unsigned int height, BOOL windowed)
{
    memset(pDinf, 0, sizeof(struct D3DInf));

    // Create Window
    {
        const DWORD kDwStyle = windowed ? WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX : WS_POPUP;
        const DWORD kDwExStyle = windowed ? 0 : WS_EX_TOPMOST;
        const int kCmdShow = windowed ? SW_SHOW : SW_SHOWMAXIMIZED;

        WNDCLASSEXW wcex = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInst, NULL, NULL, NULL,
            NULL, nameWndClass, NULL};
        if (!RegisterClassExW(&wcex))
            return ThrowError("Error: Failed to register window class.");

        RECT rect = { 0, 0, (long)width, (long)height };
        AdjustWindowRectEx(&rect, kDwStyle, FALSE, kDwExStyle);

        pDinf->hWnd = CreateWindowExW(kDwExStyle, nameWndClass, nameWnd, kDwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInst, NULL);
        if (!pDinf->hWnd)
            return ThrowError("Error: Failed to create window handle.");

        ShowWindow(pDinf->hWnd, kCmdShow);
        UpdateWindow(pDinf->hWnd);
        ShowCursor(windowed);
    }

    // Create Factory
    IDXGIFactory* pFactory = NULL;
    if (FAILED(CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory)))
        return ThrowError("Error: Failed to create factory.");

    // Create Device
    {
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };
        if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pDinf->pDevice, NULL,
                        &pDinf->pImContext)))
            return ThrowError("Error: Failed to create Direct3D11 device.");
    }

    // Create SwapChain
    {
        DXGI_SWAP_CHAIN_DESC descSwapChain = {
            {width, height, {60U, 1U}, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                DXGI_MODE_SCALING_UNSPECIFIED},
            {1, 0}, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1U, pDinf->hWnd, TRUE, DXGI_SWAP_EFFECT_DISCARD,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH};

        IUnknown* pUnknown = NULL;
        if (FAILED(pDinf->pDevice->lpVtbl->QueryInterface(pDinf->pDevice, &IID_IUnknown, &pUnknown)))
            return ThrowError("Error: Failed to down to IUnknown from ID3D11Device.");

        if (FAILED(pFactory->lpVtbl->CreateSwapChain(pFactory, pUnknown, &descSwapChain, &pDinf->pSwapChain)))
            return ThrowError("Error: Failed to create swapchain.");

        COMRelease(pUnknown);
    }

    // Create Render Target
    {
        ID3D11Texture2D* pBackBuffer = NULL;
        if (FAILED(pDinf->pSwapChain->lpVtbl->GetBuffer(
                        pDinf->pSwapChain, 0, &IID_ID3D11Texture2D, (void**)&pBackBuffer)))
            return ThrowError("Error: Failed to get back buffer.");

        ID3D11Resource* pRes = NULL;
        if (FAILED(pBackBuffer->lpVtbl->QueryInterface(pBackBuffer, &IID_ID3D11Resource, &pRes)))
            return ThrowError("Failed to down to ID3D11Resource from ID3D11Texture2D");

        if (FAILED(pDinf->pDevice->lpVtbl->CreateRenderTargetView(pDinf->pDevice, pRes, NULL, &pDinf->pRTView)))
            return ThrowError("Error: Failed to create render target view.");

        COMRelease(pBackBuffer);
        COMRelease(pRes);
    }

    // Create Depth Stencil Buffer
    {
        D3D11_DEPTH_STENCIL_DESC descDS = {TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, FALSE,
            D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, D3D11_STENCIL_OP_KEEP,
            D3D11_STENCIL_OP_KEEP};

        ID3D11DepthStencilState* pStateDS = NULL;
        if (FAILED(pDinf->pDevice->lpVtbl->CreateDepthStencilState(pDinf->pDevice, &descDS, &pStateDS)))
            return ThrowError("Error: Failed to create depth stancil stencil.");
        pDinf->pImContext->lpVtbl->OMSetDepthStencilState(pDinf->pImContext, pStateDS, 0U);

        D3D11_TEXTURE2D_DESC descDSTex = {width, height, 1U, 1U, DXGI_FORMAT_R24G8_TYPELESS, {1, 0},
            D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0U, 0U};

        ID3D11Texture2D* pDSTex = NULL;
        if (FAILED(pDinf->pDevice->lpVtbl->CreateTexture2D(pDinf->pDevice, &descDSTex, NULL, &pDSTex)))
            return ThrowError("Error: Failed to create depth stencil buffer texture.");

        ID3D11Resource* pRes = NULL;
        if (FAILED(pDSTex->lpVtbl->QueryInterface(pDSTex, &IID_ID3D11Resource, &pRes)))
            return ThrowError("Failed to down to ID3D11Resource from ID3D11Texture2D");

        D3D11_DEPTH_STENCIL_VIEW_DESC descView = {
            DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_DSV_DIMENSION_TEXTURE2D, 0U, {0U}};
        if (FAILED(pDinf->pDevice->lpVtbl->CreateDepthStencilView(pDinf->pDevice, pRes, &descView, &pDinf->pDSView)))
            return ThrowError("Error: Failed to create depth stencil buffer view.");

        COMRelease(pStateDS);
        COMRelease(pDSTex);
    }

    // Create Viewport
    {
        D3D11_VIEWPORT tmp = {0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f};
        pDinf->viewport = tmp;
    }

    // Create Shader
    if (FAILED(pDinf->pDevice->lpVtbl->CreateVertexShader(
                    pDinf->pDevice, g_vs_main, sizeof(g_vs_main), NULL, &pDinf->pVShader)))
        return ThrowError("Error: Failed to create vertex shader.");
    if (FAILED(pDinf->pDevice->lpVtbl->CreatePixelShader(
                    pDinf->pDevice, g_ps_main, sizeof(g_ps_main), NULL, &pDinf->pPShader)))
        return ThrowError("Error: Failed to create pixel shader.");

    // Create input element desc
    {
        D3D11_INPUT_ELEMENT_DESC descElem[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        if (FAILED(pDinf->pDevice->lpVtbl->CreateInputLayout(pDinf->pDevice,
                        descElem, ARRAYSIZE(descElem), g_vs_main, sizeof(g_vs_main), &pDinf->pILayout)))
            return ThrowError("Error: Failed to create input element desc.");
    }

    // Create constant buffer
    {
        D3D11_BUFFER_DESC descCB = {
            sizeof(struct ConstantBuffer), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0U, 0U, 0U
        };
        if (FAILED(pDinf->pDevice->lpVtbl->CreateBuffer(pDinf->pDevice, &descCB, NULL, &pDinf->pCBuffer)))
            return ThrowError("Error: Failed to create constant buffer.");

        if (FAILED(pDinf->pCBuffer->lpVtbl->QueryInterface(pDinf->pCBuffer, &IID_ID3D11Resource, &pDinf->pResCBuffer)))
            return ThrowError("Error: Failed to down to ID3D11Resource from ID3D11Buffer.");

        memset(&pDinf->cbuffer, 0, sizeof(struct ConstantBuffer));

        struct MDFLOAT4x4 matE = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        struct MDFLOAT4x4 matProj = {
            (float)height / (float)width * 2.414213562f, 0.0f, 0.0f, 0.0f,
            0.0f, 2.414213562f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.00050025f, -1.00050025f,
            0.0f, 0.0f, 1.0f, 0.0f
        };
        struct MDFLOAT4 vecColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        struct MDFLOAT4 vecLight = { 0.0f, 0.0f, 1.0f, 0.0f };
        struct MDFLOAT4 params = { 0.0f, 0.0f, 0.0f, 0.0f };

        pDinf->cbuffer.matScl = matE;
        pDinf->cbuffer.matRot = matE;
        pDinf->cbuffer.matTrs = matE;
        pDinf->cbuffer.matView = matE;
        pDinf->cbuffer.matProj = matProj;
        pDinf->cbuffer.vecColor = vecColor;
        pDinf->cbuffer.vecLight = vecLight;
        pDinf->cbuffer.params = params;

        pDinf->pImContext->lpVtbl->UpdateSubresource(
            pDinf->pImContext, pDinf->pResCBuffer, 0U, NULL, &pDinf->cbuffer, 0U, 0U);
        pDinf->pImContext->lpVtbl->VSSetConstantBuffers(pDinf->pImContext, 0U, 1U, &pDinf->pCBuffer);
    }

    // Set blend mode
    ID3D11BlendState* pBState = NULL;
    {
        D3D11_BLEND_DESC descBlend;
        memset(&descBlend, 0, sizeof(D3D11_BLEND_DESC));
        descBlend.AlphaToCoverageEnable = FALSE;
        descBlend.IndependentBlendEnable = FALSE;
        descBlend.RenderTarget[0].BlendEnable = TRUE;
        descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        if (FAILED(pDinf->pDevice->lpVtbl->CreateBlendState(pDinf->pDevice, &descBlend, &pBState)))
            return ThrowError("Failed to create blend state.");
    }

    // Initialize default context
    {
        pDinf->pImContext->lpVtbl->RSSetViewports(pDinf->pImContext, 1U, &pDinf->viewport);

        float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
        pDinf->pImContext->lpVtbl->OMSetBlendState(pDinf->pImContext, pBState, blendFactor, 0xffffffff);

        pDinf->pImContext->lpVtbl->IASetPrimitiveTopology(pDinf->pImContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pDinf->pImContext->lpVtbl->IASetInputLayout(pDinf->pImContext, pDinf->pILayout);

        pDinf->pImContext->lpVtbl->VSSetShader(pDinf->pImContext, pDinf->pVShader, NULL, 0U);
        pDinf->pImContext->lpVtbl->PSSetShader(pDinf->pImContext, pDinf->pPShader, NULL, 0U);
    }

    COMRelease(pFactory);
    COMRelease(pBState);
    return TRUE;
}

void FreeD3DInf(struct D3DInf* pDinf) {
    COMRelease(pDinf->pDevice);
    COMRelease(pDinf->pImContext);
    COMRelease(pDinf->pSwapChain);
    COMRelease(pDinf->pRTView);
    COMRelease(pDinf->pDSView);
    COMRelease(pDinf->pVShader);
    COMRelease(pDinf->pPShader);
    COMRelease(pDinf->pILayout);
    COMRelease(pDinf->pCBuffer);
    COMRelease(pDinf->pResCBuffer);
}

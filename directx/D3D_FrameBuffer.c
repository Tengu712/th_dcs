#include "../include/HeaderD3D11.h"

BOOL CreateFrameBuffer(struct D3DInf* pDinf, struct FrameBuffer* pFBuffer, unsigned int width, unsigned int height) {

    memset(pFBuffer, 0, sizeof(struct FrameBuffer));

    D3D11_TEXTURE2D_DESC descTex = {
        width, // Wdith
        height, // Height
        1U, // MipLevels
        1U, // ArraySize
        DXGI_FORMAT_R8G8B8A8_UNORM, // Format
        {1U, 0U}, // SampleDesc
        D3D11_USAGE_DEFAULT, // Usage
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, // BindFlags
        0U, // CPUAccessFlags
        0U // MiscFlags
    };

    ID3D11Texture2D* pTex = NULL;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateTexture2D(pDinf->pDevice, &descTex, NULL, &pTex)))
        return ThrowError("Failed to create texture2D for frame buffer.");
    
    ID3D11Resource* pRes = NULL;
    if (FAILED(pTex->lpVtbl->QueryInterface(pTex, &IID_ID3D11Resource, &pRes)))
        return ThrowError("Failed to down to ID3D11Resource from ID3D11Texture2D.");

    D3D11_RENDER_TARGET_VIEW_DESC descRTView;
    ZeroMemory(&descRTView, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    descRTView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descRTView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateRenderTargetView(
                    pDinf->pDevice, pRes, &descRTView, &pFBuffer->pRTView)))
        return ThrowError("Failed to create render target view for frame buffer.");

    D3D11_SHADER_RESOURCE_VIEW_DESC descSRView;
    ZeroMemory(&descSRView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    descSRView.Format = descRTView.Format;
    descSRView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRView.Texture2D.MipLevels = 1U;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateShaderResourceView(
                    pDinf->pDevice, pRes, &descSRView, &pFBuffer->image.pSRView)))
        return ThrowError("Failed to create shader resource view for frame buffer.");

    COMRelease(pTex);
    COMRelease(pRes);
    return TRUE;
}

void FreeFrameBuffer(struct FrameBuffer* pFBuffer) {
    if (pFBuffer->pRTView != NULL)
        COMRelease(pFBuffer->pRTView);
    FreeImage(&pFBuffer->image);
    memset(pFBuffer, 0, sizeof(struct FrameBuffer));
}
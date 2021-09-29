#include "../include/HeaderDX11.hpp"

bool D3DManager::createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* pFBuffer) {
    try {
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
        ComPtr<ID3D11Texture2D> pTex = nullptr;
        if (FAILED(inf.pDevice->CreateTexture2D(&descTex, nullptr, pTex.GetAddressOf())))
            throw "Failed to create texture2D for frame buffer.";

        D3D11_RENDER_TARGET_VIEW_DESC descRTView;
        ZeroMemory(&descRTView, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        descRTView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        descRTView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        if (FAILED(inf.pDevice->CreateRenderTargetView(pTex.Get(), &descRTView, pFBuffer->pRTView.GetAddressOf())))
            throw "Failed to create render target view for frame buffer.";

        D3D11_SHADER_RESOURCE_VIEW_DESC descSRView;
        ZeroMemory(&descSRView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        descSRView.Format = descRTView.Format;
        descSRView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        descSRView.Texture2D.MipLevels = 1U;
        if (FAILED(inf.pDevice->CreateShaderResourceView(pTex.Get(), &descSRView, pFBuffer->texture.pSRView.GetAddressOf())))
            throw "Failed to create shader resource view for frame buffer.";

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        return false;
    }
    return true;
}

void D3DManager::drawBeginWithFrameBuffer(FrameBuffer* pFBuffer, bool depth) {
    inf.pImContext->OMSetRenderTargets(1U, pFBuffer->pRTView.GetAddressOf(), depth ? inf.pDSView.Get() : nullptr);
    inf.pImContext->RSSetViewports(1U, &inf.viewport);

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    inf.pImContext->ClearRenderTargetView(pFBuffer->pRTView.Get(), clearColor);
    if (depth)
        inf.pImContext->ClearDepthStencilView(inf.pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);

    float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
    inf.pImContext->OMSetBlendState(inf.pBState.Get(), blendFactor, 0xffffffff);

    inf.pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    inf.pImContext->IASetInputLayout(inf.pILayout.Get());

    inf.pImContext->VSSetShader(inf.pVShader.Get(), nullptr, 0U);
    inf.pImContext->PSSetShader(inf.pPShader.Get(), nullptr, 0U);
}

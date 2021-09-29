#include "../include/HeaderDX11.hpp"

void D3DManager::drawBegin(bool depth) {
    inf.pImContext->OMSetRenderTargets(1U, inf.pRTView.GetAddressOf(), depth ? inf.pDSView.Get() : nullptr);
    inf.pImContext->RSSetViewports(1U, &inf.viewport);

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    inf.pImContext->ClearRenderTargetView(inf.pRTView.Get(), clearColor);
    if (depth)
        inf.pImContext->ClearDepthStencilView(inf.pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);

    float blendFactor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
    inf.pImContext->OMSetBlendState(inf.pBState.Get(), blendFactor, 0xffffffff);

    inf.pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    inf.pImContext->IASetInputLayout(inf.pILayout.Get());

    inf.pImContext->VSSetShader(inf.pVShader.Get(), nullptr, 0U);
    inf.pImContext->PSSetShader(inf.pPShader.Get(), nullptr, 0U);
}

void D3DManager::drawEnd() {
    inf.pSwapChain->Present(1U, 0U);
}


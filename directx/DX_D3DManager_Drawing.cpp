#include "../include/HeaderDX11.hpp"

void D3DManager::clearDepthStencil() {
    inf.pImContext->ClearDepthStencilView(inf.pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);
}

void D3DManager::drawBegin() {
    inf.pImContext->OMSetRenderTargets(1U, inf.pRTView.GetAddressOf(), inf.pDSView.Get());
    inf.pImContext->RSSetViewports(1U, &inf.viewport);

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    inf.pImContext->ClearRenderTargetView(inf.pRTView.Get(), clearColor);
    inf.pImContext->ClearDepthStencilView(inf.pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);

    inf.pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    inf.pImContext->IASetInputLayout(inf.pILayout.Get());

    inf.pImContext->VSSetShader(inf.pVShader.Get(), nullptr, 0U);
    inf.pImContext->PSSetShader(inf.pPShader.Get(), nullptr, 0U);
}

void D3DManager::drawEnd() {
    inf.pSwapChain->Present(1U, 0U);
}


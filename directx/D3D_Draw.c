#include "../include/HeaderD3D11.h"

void DrawBegin(struct D3DInf* pDinf, struct FrameBuffer* pFBuffer, BOOL depth) {
    pDinf->pImContext->lpVtbl->OMSetRenderTargets(
        pDinf->pImContext, 1U, pFBuffer == NULL ? &pDinf->pRTView : &pFBuffer->pRTView, depth ? pDinf->pDSView : NULL);

    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    pDinf->pImContext->lpVtbl->ClearRenderTargetView(
        pDinf->pImContext, pFBuffer == NULL ? pDinf->pRTView : pFBuffer->pRTView, clearColor);
    if (depth)
        pDinf->pImContext->lpVtbl->ClearDepthStencilView(
            pDinf->pImContext, pDinf->pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);
}

void DrawEnd(struct D3DInf* pDinf) {
    pDinf->pSwapChain->lpVtbl->Present(pDinf->pSwapChain, 1U, 0U);
}

void DrawModel(struct D3DInf* pDinf, struct ModelInf* pMinf) {
    unsigned int strides = sizeof(struct Vertex);
    unsigned int offsets = 0U;
    pDinf->pImContext->lpVtbl->IASetVertexBuffers(pDinf->pImContext, 0U, 1U, &pMinf->pVBuffer, &strides, &offsets);
    pDinf->pImContext->lpVtbl->IASetIndexBuffer(pDinf->pImContext, pMinf->pIBuffer, DXGI_FORMAT_R32_UINT, 0U);

    struct MDFLOAT4x4 matScl = {
        pMinf->sclX, 0.0f, 0.0f, 0.0f,
        0.0f, pMinf->sclY, 0.0f, 0.0f,
        0.0f, 0.0f, pMinf->sclZ, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    struct MDFLOAT4x4 matRot = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    struct MDFLOAT4x4 matTrs = {
        1.0f, 0.0f, 0.0f, pMinf->posX,
        0.0f, 1.0f, 0.0f, pMinf->posY,
        0.0f, 0.0f, 1.0f, pMinf->posZ,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    struct MDFLOAT4 vecColor = {
        pMinf->colR, pMinf->colG, pMinf->colB, pMinf->colA
    };
    pDinf->cbuffer.matScl = matScl;
    pDinf->cbuffer.matRot = matRot;
    pDinf->cbuffer.matTrs = matTrs;
    pDinf->cbuffer.vecColor = vecColor;

    pDinf->pImContext->lpVtbl->UpdateSubresource(pDinf->pImContext, pDinf->pResCBuffer, 0U, NULL, &pDinf->cbuffer, 0U, 0U);
    pDinf->pImContext->lpVtbl->VSSetConstantBuffers(pDinf->pImContext, 0U, 1U, &pDinf->pCBuffer);

    pDinf->pImContext->lpVtbl->DrawIndexed(pDinf->pImContext, pMinf->numIdx, 0U, 0U);
}
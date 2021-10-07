#include "../include/HeaderD3D11.h"

BOOL CreateFontImage(struct D3DInf* pDinf, struct Image* pImage, LOGFONTA* pFont, unsigned int code) {

    memset(pImage, 0, sizeof(struct Image));

    // create font handle
    HFONT hFont = CreateFontIndirectA(pFont);
    HDC hdc = GetDC(NULL);
    HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

    // get metrics
    TEXTMETRIC metText;
    GetTextMetrics(hdc, &metText);
    GLYPHMETRICS metGlyph;
    const MAT2 mat = {{0,1}, {0,0}, {0,0}, {0,1}};
    DWORD dwRes = GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &metGlyph, 0, NULL, &mat);

    // get bmp data
    unsigned char* pMono = (unsigned char*)malloc(dwRes);
    GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &metGlyph, dwRes, pMono, &mat);
    
    // release
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);
    ReleaseDC(NULL, hdc);

    // set kWidth and kHeight
    const unsigned int kWidth = (unsigned int)metGlyph.gmCellIncX;
    const unsigned int kHeight = (unsigned int)metText.tmHeight;
    const int kOfsX = metGlyph.gmptGlyphOrigin.x;
    const int kOfsY = metText.tmAscent - metGlyph.gmptGlyphOrigin.y;
    const int kWidthBmp = metGlyph.gmBlackBoxX + (4 - (metGlyph.gmBlackBoxX % 4)) % 4;
    const int kHeightBmp = metGlyph.gmBlackBoxY;

    // create texture
    D3D11_TEXTURE2D_DESC descTex = {
        kWidth, // Width
        kHeight, // Height
        1, // MipLevels
        1, // ArraySize
        DXGI_FORMAT_R8G8B8A8_UNORM, // Format
        {1, 0}, // SampleDesc
        D3D11_USAGE_DYNAMIC, // Usage
        D3D11_BIND_SHADER_RESOURCE, // BindFlags
        D3D11_CPU_ACCESS_WRITE, // CPUAccessFlags
        0, // MiscFlags
    };
    ID3D11Texture2D* pBufLayer = NULL;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateTexture2D(pDinf->pDevice, &descTex, NULL, &pBufLayer))) {
        free(pMono);
        return ThrowError("Failed to create font texture.");
    }

    ID3D11Resource* pRes = NULL;
    if (FAILED(pBufLayer->lpVtbl->QueryInterface(pBufLayer, &IID_ID3D11Resource, &pRes)))
        return ThrowError("Failed to down to ID3D11Resource from ID3D11Texture2D.");

    // create subresource
    D3D11_MAPPED_SUBRESOURCE sres;
    pDinf->pImContext->lpVtbl->Map(pDinf->pImContext, pRes, 0, D3D11_MAP_WRITE_DISCARD, 0, &sres);
    unsigned char* pBits = (unsigned char*)sres.pData;
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            if (x < kOfsX || y < kOfsY || x >= kOfsX + kWidthBmp || y >= kOfsY + kHeightBmp) {
                DWORD col = 0x00000000;
                memcpy(pBits + sres.RowPitch * y + 4 * x, &col, sizeof(DWORD));
            }
            else {
                DWORD alp = (255 * pMono[x - kOfsX + kWidthBmp * (y - kOfsY)]) / 16;
                DWORD col = 0x00ffffff | (alp << 24);
                memcpy(pBits + sres.RowPitch * y + 4 * x, &col, sizeof(DWORD));
            }
        }
    }
    pDinf->pImContext->lpVtbl->Unmap(pDinf->pImContext, pRes, 0);

    free(pMono);

    // create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC descSRView;
    ZeroMemory(&descSRView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    descSRView.Format = descTex.Format;
    descSRView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRView.Texture2D.MostDetailedMip = 0;
    descSRView.Texture2D.MipLevels = descTex.MipLevels;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateShaderResourceView(pDinf->pDevice, pRes, &descSRView, &pImage->pSRView))) 
        return ThrowError("Failed to create shader resource view.");

    COMRelease(pRes);
    COMRelease(pBufLayer);
    return TRUE;
}

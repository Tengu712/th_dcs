#include "../include/HeaderDX11.hpp"

#include <Shlwapi.h>

bool D3DManager::createFont(unsigned int code, Texture* pFont) {
    if (pFont == nullptr) {
        MessageBoxA(nullptr, "Cannot create nullptr font.", "Error", MB_OK);
        return false;
    }

    // create font handle
    LOGFONT logfont = {
        64, 0, 0, 0,
        0, 0, 0, 0,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH | FF_MODERN,
        "ＭＳ ゴシック",
    };
    HFONT hFont = CreateFontIndirectA(&logfont);
    HDC hdc = GetDC(nullptr);
    HFONT hFontOld = static_cast<HFONT>(SelectObject(hdc, hFont));

    // get metrics
    TEXTMETRIC metText;
    GetTextMetrics(hdc, &metText);
    GLYPHMETRICS metGlyph;
    const MAT2 mat = {{0,1}, {0,0}, {0,0}, {0,1}};
    DWORD dwRes = GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &metGlyph, 0, nullptr, &mat);

    // get bmp data
    unsigned char* pMono = new unsigned char[dwRes];
    GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &metGlyph, dwRes, pMono, &mat);
    
    // release
    SelectObject(hdc, hFontOld);
    DeleteObject(hFont);
    ReleaseDC(nullptr, hdc);

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
    ComPtr<ID3D11Texture2D> bufLayer = nullptr;
    if (FAILED(inf.pDevice->CreateTexture2D(&descTex, nullptr, bufLayer.GetAddressOf()))) {
        MessageBoxA(nullptr, "Failed to create font texture.", "Error", MB_OK);
        delete[] pMono;
        return false;
    }

    // create subresource
    D3D11_MAPPED_SUBRESOURCE sres;
    inf.pImContext->Map(bufLayer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sres);
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
    inf.pImContext->Unmap(bufLayer.Get(), 0);
    delete[] pMono;

    // create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC descSRView;
    ZeroMemory(&descSRView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    descSRView.Format = descTex.Format;
    descSRView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRView.Texture2D.MostDetailedMip = 0;
    descSRView.Texture2D.MipLevels = descTex.MipLevels;
    if (FAILED(inf.pDevice->CreateShaderResourceView(bufLayer.Get(), &descSRView, pFont->pSRView.GetAddressOf()))) {
        MessageBoxA(nullptr, "Failed to create shader resource view.", "Error", MB_OK);
        return false;
    }

    return true;
}

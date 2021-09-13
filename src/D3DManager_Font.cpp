#include "../include/HeaderDX11.hpp"

#include <Shlwapi.h>

unsigned int D3DManager::str2uint(LPCSTR c) {
    if (IsDBCSLeadByte(*c))
        return (unsigned char)c[0] << 8 | (unsigned char)c [1];    
    else
        return c[0];
}

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

    // set width and height
    pFont->width = (unsigned int)metGlyph.gmCellIncX;
    pFont->height = (unsigned int)metText.tmHeight;
    const int ofsX = metGlyph.gmptGlyphOrigin.x;
    const int ofsY = metText.tmAscent - metGlyph.gmptGlyphOrigin.y;
    const int widthBmp = metGlyph.gmBlackBoxX + (4 - (metGlyph.gmBlackBoxX % 4)) % 4;
    const int heightBmp = metGlyph.gmBlackBoxY;

    // create texture
    D3D11_TEXTURE2D_DESC descTex = {
        pFont->width, // Width
        pFont->height, // Height
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
    for (int y = 0; y < pFont->height; ++y) {
        for (int x = 0; x < pFont->width; ++x) {
            if (x < ofsX || y < ofsY || x >= ofsX + widthBmp || y >= ofsY + heightBmp) {
                DWORD col = 0x00000000;
                memcpy(pBits + sres.RowPitch * y + 4 * x, &col, sizeof(DWORD));
            }
            else {
                DWORD alp = (255 * pMono[x - ofsX + widthBmp * (y - ofsY)]) / 16;
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

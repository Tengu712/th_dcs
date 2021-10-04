#include "../include/HeaderDX11.h"
#include "../include/resource.h"

#include <wincodec.h>
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Ole32.lib")

BOOL CreateImage(struct D3DInf* pDinf, struct Image* pImage, HMODULE hModule, unsigned int id) {

    memset(pImage, 0, sizeof(struct Image));
    pImage->id = id;

    HRSRC hImageRes = FindResourceA(hModule, MAKEINTRESOURCEA(id), "IMAGE");
    if (!hImageRes)
        return ThrowError("Failed to find resource.");

    HGLOBAL hImageData = LoadResource(hModule, hImageRes);
    if (!hImageData)
        return ThrowError("Failed to load resource.");

    void* pLock = LockResource(hImageData);
    if (!pLock)
        return ThrowError("Failed to lock resource.");

    DWORD sizeRes = SizeofResource(hModule, hImageRes);
    if (sizeRes == 0)
        return ThrowError("Failed to get size of resource.");

    IWICImagingFactory* pFactory = NULL;
    if (FAILED(CoCreateInstance(
                    &CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                    &IID_IWICImagingFactory, (LPVOID*)&pFactory)))
        return ThrowError("Failed to create wic factory.");

    IWICStream* pWICStream = NULL;
    if (FAILED(pFactory->lpVtbl->CreateStream(pFactory, &pWICStream)))
        return ThrowError("Failed to create stream.");

    if (FAILED(pWICStream->lpVtbl->InitializeFromMemory(pWICStream, (byte*)pLock, sizeRes)))
        return ThrowError("Failed to initialize stream.");

    IStream* pStream = NULL;
    if (FAILED(pWICStream->lpVtbl->QueryInterface(pWICStream, &IID_IStream, &pStream)))
        return ThrowError("Failed to down to IStream from IWICStream.");

    IWICBitmapDecoder* pDecoder = NULL;
    if (FAILED(pFactory->lpVtbl->CreateDecoderFromStream(
                    pFactory, pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder)))
        return ThrowError("Failed to load file.");

    IWICBitmapFrameDecode* pFrame = NULL;
    if (FAILED(pDecoder->lpVtbl->GetFrame(pDecoder, 0U, &pFrame)))
        return ThrowError("Failed to get frame.");

    IWICFormatConverter* pFormatConverter = NULL;
    if (FAILED(pFactory->lpVtbl->CreateFormatConverter(pFactory, &pFormatConverter)))
        return ThrowError("Failed to create format converter.");

    IWICBitmapSource* pBSource = NULL;
    if (FAILED(pFrame->lpVtbl->QueryInterface(pFrame, &IID_IWICBitmapSource, &pBSource)))
        return ThrowError("Failed to down to IWICBitmapSource from IWICBitmapFrameDecode.");

    if (FAILED(pFormatConverter->lpVtbl->Initialize(
                    pFormatConverter, pBSource, &GUID_WICPixelFormat32bppRGBA, 
                    WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut)))
        return ThrowError("Failed to initialize format.");

    unsigned int width, height;
    if (FAILED(pFormatConverter->lpVtbl->GetSize(pFormatConverter, &width, &height)))
        return ThrowError("Failed to get texture size.");

    D3D11_TEXTURE2D_DESC descTex = {width, height, 1U, 1U, DXGI_FORMAT_R8G8B8A8_UNORM, {1U, 0U},
        D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0U};
    ID3D11Texture2D* pTex = NULL;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateTexture2D(pDinf->pDevice, &descTex, NULL, &pTex)))
        return ThrowError("Failed to create texture resource.");

    ID3D11Resource* pRes = NULL;
    if (FAILED(pTex->lpVtbl->QueryInterface(pTex, &IID_ID3D11Resource, &pRes)))
        return ThrowError("Failed to down to ID3D11Resource from ID3D11Texture2D");

    D3D11_MAPPED_SUBRESOURCE resMapped;
    memset(&resMapped, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
    if (FAILED(pDinf->pImContext->lpVtbl->Map(
                    pDinf->pImContext, pRes, 0U, D3D11_MAP_WRITE_DISCARD, 0U, &resMapped)))
        return ThrowError("Failed to map.");
    if (FAILED(pFormatConverter->lpVtbl->CopyPixels(
                    pFormatConverter, NULL, width * 4U, width * height * 4U, (BYTE*)resMapped.pData)))
        return ThrowError("Failed to copy pixels.");
    pDinf->pImContext->lpVtbl->Unmap(pDinf->pImContext, pRes, 0U);

    D3D11_SHADER_RESOURCE_VIEW_DESC descShader;
    ZeroMemory(&descShader, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    descShader.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descShader.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descShader.Texture2D.MipLevels = 1U;
    if (FAILED(pDinf->pDevice->lpVtbl->CreateShaderResourceView(
                    pDinf->pDevice, pRes, &descShader, &pImage->pSRView)))
        return ThrowError("Failed to create shader resource view.");

    COMRelease(pFactory);
    COMRelease(pWICStream);
    COMRelease(pStream);
    COMRelease(pDecoder);
    COMRelease(pFrame);
    COMRelease(pBSource);
    COMRelease(pFormatConverter);
    COMRelease(pTex);
    COMRelease(pRes);
    return TRUE;
}

void ApplyImage(struct D3DInf* pDinf, struct Image* pImage) {
    if (pImage == NULL || pImage->pSRView == NULL) {
        pDinf->cbuffer.params.x = 0.0f;
    } else {
        pDinf->pImContext->lpVtbl->PSSetShaderResources(pDinf->pImContext, 0U, 1U, &pImage->pSRView);
        pDinf->cbuffer.params.x = 1.0f;
    }
}

void FreeImage(struct Image* pImage) {
    if (pImage->pSRView != NULL)
        COMRelease(pImage->pSRView);
    memset(pImage, 0, sizeof(struct Image));
}
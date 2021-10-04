#pragma once
#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <d3d11.h>
#include <windows.h>
#include <xinput.h>
#include <xaudio2.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")
#pragma comment (lib, "xaudio2.lib")

// ================================================================================================================= //
//                                          Constants
// ================================================================================================================= //

#define MD_PIDIV4 0.785398163f

// ================================================================================================================= //
//                                          Structs
// ================================================================================================================= //

struct Vertex {
    float pcnu[12];
};

struct MDFLOAT4 {
    float x, y, z, w;
};

struct MDFLOAT4x4 {
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
};

struct ConstantBuffer {
    struct MDFLOAT4x4 matScl;
    struct MDFLOAT4x4 matRot;
    struct MDFLOAT4x4 matTrs;
    struct MDFLOAT4x4 matView;
    struct MDFLOAT4x4 matProj;
    struct MDFLOAT4 vecColor;
    struct MDFLOAT4 vecLight;
    struct MDFLOAT4 params;
};

struct Image {
    unsigned int id;
    ID3D11ShaderResourceView* pSRView;
};

struct FrameBuffer {
    ID3D11RenderTargetView* pRTView;
    struct Image image;
};

struct Camera {
    float width, height;
    float angle, nearZ, farZ;
    float posX, posY, posZ;
    float rghX, rghY, rghZ;
    float uppX, uppY, uppZ;
    float dirX, dirY, dirZ;
};

struct ModelInf {
    unsigned int numIdx;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    ID3D11Buffer* pVBuffer;
    ID3D11Buffer* pIBuffer;
};

struct D3DInf {
    HWND hWnd;
    ID3D11Device* pDevice;
    ID3D11DeviceContext* pImContext;
    IDXGISwapChain* pSwapChain;
    ID3D11RenderTargetView* pRTView;
    ID3D11DepthStencilView* pDSView;
    D3D11_VIEWPORT viewport;
    ID3D11VertexShader* pVShader;
    ID3D11PixelShader* pPShader;
    ID3D11InputLayout* pILayout;
    ID3D11Buffer* pCBuffer;
    ID3D11Resource* pResCBuffer;
    struct ConstantBuffer cbuffer;
};

// ================================================================================================================= //
//                                          Functions
// ================================================================================================================= //

#define COMRelease(c) {(c)->lpVtbl->Release((c)); (c) = NULL;}

// General
inline unsigned int Lpcstr2uint(LPCSTR c) {
    if (IsDBCSLeadByte(*c))
        return (unsigned char)c[0] << 8 | (unsigned char)c [1];
    else
        return c[0];
}
inline BOOL ThrowError(LPCSTR error) {
    MessageBoxA(NULL, error, "Error", MB_OK);
    return FALSE;
}

BOOL CreateD3DInf(
    struct D3DInf* pDinf, HINSTANCE hInst, LPCWSTR nameWnd, LPCWSTR nameWndClass,
    unsigned int width, unsigned int height, BOOL windowed);
void FreeD3DInf(struct D3DInf* pDinf);

void DrawBegin(struct D3DInf* pDinf, struct FrameBuffer* pFBuffer, BOOL depth);
void DrawEnd(struct D3DInf* pDinf);
void DrawModel(struct D3DInf* pDinf, struct ModelInf* pMinf);

BOOL CreateImage(struct D3DInf* pDinf, struct Image* pImage, HMODULE hModule, unsigned int id);
void ApplyImage(struct D3DInf* pDinf, struct Image* pImage);
void FreeImage(struct Image* pImage);

BOOL CreateFontImage(struct D3DInf* pDinf, struct Image* pImage, unsigned int code);

BOOL CreateFrameBuffer(struct D3DInf* pDinf, struct FrameBuffer* pFBuffer, unsigned int width, unsigned int height);
void FreeFrameBuffer(struct FrameBuffer* pFBuffer);

void CreateCamera(float width, float height, struct Camera* pCamera);
void ApplyCamera(struct D3DInf* pDinf, struct Camera* pCamera, BOOL parse);

void InitModelInf(struct ModelInf* pMinf);
BOOL CreateModelBuffer(struct D3DInf* pDinf, struct ModelInf* pMinf, unsigned int numVtx, struct Vertex* dataPCU, unsigned int* dataIdx);










/*
enum struct GAMEPAD_KEYTYPE : char {
    Buttons, LTrigger, RTrigger,
    ThumbLL, ThumbLR, ThumbLU, ThumbLD,
    ThumbRL, ThumbRR, ThumbRU, ThumbRD,
};

struct KeyInf {
    char state;
    char codeKey;
    char codeKeyboard;
    GAMEPAD_KEYTYPE typeGamepadKey;
    short codeGamepad;
};

class InputManager {
    private:
        int numRegistered;
        char statePrev[64];
        KeyInf inf[64];
    public:
        InputManager();
        bool addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad);
        void inspect();
        char getKey(char codeKey);
};

class VoiceCallback : public IXAudio2VoiceCallback
{
    public:
        HANDLE event;
        VoiceCallback() : event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
        ~VoiceCallback() { CloseHandle(event); }
        void STDMETHODCALLTYPE OnStreamEnd() {}
        void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
        void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
        void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) { SetEvent(event); }
        void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {}
        void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {}
        void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) {}
};

struct AudioParam {
    int nABPS;
    HMMIO hMmio;
    IXAudio2SourceVoice* pSVoice;
    VoiceCallback callback;
};

class Audio {
    public:
        IXAudio2SourceVoice* pSVoice;
        XAUDIO2_BUFFER buffer;
        Audio();
        ~Audio();
        void play();
        void stop();
        void restart();
};

class AudioManager {
    private:
        IXAudio2* pXAudio;
        IXAudio2MasteringVoice* pMVoice;
    public:
        AudioManager();
        ~AudioManager();
        bool init();
        bool createAudio(unsigned int id, Audio* pAudio);
};

*/

#endif


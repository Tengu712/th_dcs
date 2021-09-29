#pragma once
#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <DirectXMath.h>
#include <d3d11.h>
#include <windows.h>
#include <wrl/client.h>
#include <xinput.h>
#include <xaudio2.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment (lib, "xinput.lib")
#pragma comment (lib, "xaudio2.lib")

using Microsoft::WRL::ComPtr;

void DebugBox(int i, const char* title);
void DebugBox(const char* str, const char* title);
unsigned int Lpcstr2uint(LPCSTR c);

struct Vertex {
    float pcnu[12];
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 matScl;
    DirectX::XMFLOAT4X4 matRot;
    DirectX::XMFLOAT4X4 matTrs;
    DirectX::XMFLOAT4X4 matView;
    DirectX::XMFLOAT4X4 matProj;
    DirectX::XMFLOAT4 vecColor;
    DirectX::XMFLOAT4 vecLight;
    DirectX::XMFLOAT4 params;
};

struct Camera {
    float width, height;
    float angle, nearZ, farZ;
    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    float uppX, uppY, uppZ;
};

struct Texture {
    unsigned int id;
    ComPtr<ID3D11ShaderResourceView> pSRView;
};

struct D3DInf {
    // General
    ComPtr<ID3D11Device> pDevice;
    ComPtr<ID3D11DeviceContext> pImContext;
    ComPtr<IDXGISwapChain> pSwapChain;
    ComPtr<ID3D11RenderTargetView> pRTView;
    ComPtr<ID3D11DepthStencilView> pDSView;
    D3D11_VIEWPORT viewport;

    // Shader
    ComPtr<ID3D11VertexShader> pVShader;
    ComPtr<ID3D11PixelShader> pPShader;

    // Polygon
    ComPtr<ID3D11InputLayout> pILayout;
    ComPtr<ID3D11Buffer> pCBuffer;
    ComPtr<ID3D11Buffer> pCBufPx;
    ConstantBuffer cbuffer;
};

struct ModelInf {
    unsigned int numIdx;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    ComPtr<ID3D11Buffer> pVBuffer;
    ComPtr<ID3D11Buffer> pIBuffer;
};

class D3DManager {
    private:
        HWND hWnd;
        D3DInf inf;

    public:
        D3DManager();

        // General
        bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
                unsigned int height, bool windowed);
        void drawBegin();
        void drawEnd();
        HWND getWindowHandle();
        D3DInf* getD3DInformation();

        // Camera
        void createCamera(float width, float height, Camera* camera);
        void applyCamera(Camera* camera, bool isParse);

        // Texture
        bool createTexture(HMODULE hModule, unsigned int id, Texture* pTexture);
        void applyTexture(Texture* pTexture);

        // Font
        bool createFont(unsigned int code, Texture* pFont);

        // Model
        bool createModelBuffers(unsigned int numVtx, Vertex* data, unsigned int* dataIdx, ModelInf* minf);
        void drawModel(ModelInf* minf);

        // Buffer
        void clearDepthStencil();
};


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
        ComPtr<IXAudio2> pXAudio;
        IXAudio2MasteringVoice* pMVoice;
    public:
        AudioManager();
        ~AudioManager();
        bool init();
        bool createAudio(unsigned int id, Audio* pAudio);
};

#endif


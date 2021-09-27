#include "../include/HeaderApp.hpp"

GameInf::GameInf() :
    // Manager
    dmanager(D3DManager()),
    imanager(InputManager()),
    amanager(AudioManager()),
    // System
    sceCur(SCE_ID::Title),
    sceNex(SCE_ID::Title),
    texs(nullptr),
    fonts(nullptr),
    ideaSquare(ModelInf()),
    // Queue
    queUI(nullptr),
    queFont(nullptr),
    // Fps
    cntFps(0),
    fps(0.0f),
    startTime(timeGetTime()),
    lastTime(timeGetTime()),
    // Camera
    cameraUI(Camera())
{}

GameInf::~GameInf() {
    if (texs != nullptr)
        delete texs;
    if (fonts != nullptr)
        delete fonts;
    if (queUI != nullptr)
        delete queUI;
    if (queFont != nullptr)
        delete queFont;
}

bool GameInf::init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName,
        unsigned int width, unsigned int height, bool windowed, HMODULE hModule) 
{
    try {
        // DirectX11
        if (!dmanager.init(hInst, cmdShow, wndName, wndClassName, width, height, windowed))
            throw "Failed to initialize Direct3D.";
        if (!amanager.init())
            throw "Failed to initialize AudioManager.";

        // Resource
        texs = new Texture[MAX_TEX];
        fonts = new Texture[MAX_FNT];
        for (int i = 0; i < MAX_TEX; ++i) {
            texs[i] = Texture();
        }
        for (int i = 0; i < MAX_FNT; ++i) {
            fonts[i] = Texture();
        }

        // Queue
        queUI = new Fact*[MAX_QUE_UI];
        queFont = new Fact*[MAX_QUE_FNT];
        for (int i = 0; i < MAX_QUE_UI; ++i) {
            queUI[i] = nullptr;
        }
        for (int i = 0; i < MAX_QUE_FNT; ++i) {
            queFont[i] = nullptr;
        }

        // Idea
        const unsigned int kNumVtx = 4U;
        const unsigned int kNumIdx = 6U;
        ideaSquare.numIdx = kNumIdx;
        struct Vertex dataPCNU[kNumVtx] = {
            {-50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
            {-50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
            {+50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
            {+50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        };
        unsigned int dataIdx[kNumIdx] = {0, 1, 2, 0, 2, 3};
        if (!dmanager.createModelBuffers(kNumVtx, dataPCNU, dataIdx, &ideaSquare))
            throw "Failed to create idea of square.";

        // Camera
        dmanager.createCamera(width, height, &cameraUI);
        cameraUI.posZ = -1200.0f;

        // Load screen
        if (!addTexture(hModule, TEX_BG_LOAD))
            throw "Failed to load image for load screen.";
        Fact bgLoad = Fact();
        bgLoad.posZ = -900.0f;
        bgLoad.sclX = 12.8f;
        bgLoad.sclY = 9.6f;
        applyFact(&bgLoad);
        dmanager.drawBegin();
        dmanager.applyCamera(&cameraUI, false);
        dmanager.applyTexture(getTexture(TEX_BG_LOAD));
        dmanager.drawModel(&ideaSquare);
        dmanager.drawEnd();

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        return false;
    }

    return true;
}

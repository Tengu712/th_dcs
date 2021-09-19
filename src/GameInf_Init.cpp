#include "../include/HeaderApp.hpp"
#include "../include/resource.hpp"

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
    // Fps
    cntFps(0),
    fps(0.0f),
    startTime(timeGetTime()),
    lastTime(timeGetTime()),
    modelsFps(nullptr),
    // Camera
    cameraUI(Camera()),
    // Queue
    queUI(ModelQueue())
{}

GameInf::~GameInf() {
    if (texs != nullptr)
        delete texs;
    if (fonts != nullptr)
        delete fonts;
    if (modelsFps != nullptr)
        delete modelsFps;
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
        texs = new GameTexture[MAX_TEX];
        fonts = new GameTexture[MAX_FNT];
        for (int i = 0; i < MAX_TEX; ++i) {
            texs[i] = GameTexture();
        }
        for (int i = 0; i < MAX_FNT; ++i) {
            fonts[i] = GameTexture();
        }

        // Camera
        dmanager.createCamera(width, height, &cameraUI);
        cameraUI.posZ = -1200.0f;

        // Queue
        queUI.init(MAX_UI);

        // Fps
        modelsFps = new ModelSquare[7];
        for (int i = 0; i < 7; ++i) {
            modelsFps[i] = ModelSquare();
            if (!modelsFps[i].init(&dmanager))
                throw "Failed to create model for FPS viewer.";
            modelsFps[i].posZ = -1000.0f;
            modelsFps[i].posX = 15.0f * (float)i + 532.0f;
            modelsFps[i].posY = -457.0f;
            modelsFps[i].sclX = 0.15f;
            modelsFps[i].sclY = 0.2f;
        }

        // Load screen
        if (!addTexture(hModule, TEX_LOAD))
            throw "Failed to load image for load screen.";
        ModelSquare bgLoad = ModelSquare();
        if (!bgLoad.init(&dmanager))
            throw "Failed to create model for load screen.";
        bgLoad.posZ = -900.0f;
        bgLoad.sclX = 12.8f;
        bgLoad.sclY = 9.6f;
        dmanager.drawBegin();
        dmanager.applyCamera(&cameraUI, false);
        dmanager.applyTexture(getTexture(TEX_LOAD));
        dmanager.drawModel(&bgLoad);
        dmanager.drawEnd();
    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        return false;
    }

    return true;
}

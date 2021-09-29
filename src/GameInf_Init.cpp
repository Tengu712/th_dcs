#include "../include/HeaderApp.hpp"

#include <stdio.h>

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
    fbBG(FrameBuffer()),
    fbGame(FrameBuffer()),
    // Queue
    queBG(nullptr),
    queUI(nullptr),
    queFont(nullptr),
    // Fps
    cntFps(0),
    fps(0.0f),
    startTime(timeGetTime()),
    lastTime(timeGetTime()),
    // Camera
    cameraBG(Camera()),
    cameraGame(Camera()),
    cameraUI(Camera()),
    // Game
    player(Player()),
    mapBullets(nullptr),
    mapBulletsSelf(nullptr),
    bullets(nullptr),
    bulletsSelf(nullptr)
{}

GameInf::~GameInf() {
    if (texs != nullptr)
        delete texs;
    if (fonts != nullptr)
        delete fonts;
    if (queBG != nullptr)
        delete queBG;
    if (queUI != nullptr)
        delete queUI;
    if (queFont != nullptr)
        delete queFont;
    if (mapBullets != nullptr)
        delete mapBullets;
    if (mapBulletsSelf != nullptr)
        delete mapBulletsSelf;
    if (bullets != nullptr) {
        for (int i = 0; i < MAX_KND_BUL; ++i) {
            if (bullets[i] != nullptr)
                delete bullets[i];
        }
        delete bullets;
    }
    if (bulletsSelf != nullptr) {
        for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
            if (bulletsSelf[i] != nullptr)
                delete bulletsSelf[i];
        }
        delete bullets;
    }
}

bool GameInf::init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName,
        unsigned int width, unsigned int height, bool windowed) 
{
    // Load dll
    HMODULE hModule = LoadLibrary("./resource.dll");
    if (!hModule) {
        MessageBoxA(nullptr, "resource.dll not found.", "Error", MB_OK);
        return false;
    }

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
        queBG = new Fact*[MAX_QUE_BG];
        queUI = new Fact*[MAX_QUE_UI];
        queFont = new Fact*[MAX_QUE_FNT];
        for (int i = 0; i < MAX_QUE_BG; ++i) {
            queBG[i] = nullptr;
        }
        for (int i = 0; i < MAX_QUE_UI; ++i) {
            queUI[i] = nullptr;
        }
        for (int i = 0; i < MAX_QUE_FNT; ++i) {
            queFont[i] = nullptr;
        }

        // Bullets map
        mapBullets = new int[MAX_KND_BUL];
        mapBulletsSelf = new int[MAX_KND_BUL_SELF];
        for (int i = 0; i < MAX_KND_BUL; ++i) {
            mapBullets[i] = 0;
        }
        for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
            mapBulletsSelf[i] = 0;
        }

        // Bullets
        bullets = new Bullet*[MAX_KND_BUL];
        bulletsSelf = new Bullet*[MAX_KND_BUL_SELF];
        for (int i = 0; i < MAX_KND_BUL; ++i) {
            bullets[i] = new Bullet[MAX_NUM_BUL];
        }
        for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
            bulletsSelf[i] = new Bullet[MAX_NUM_BUL_SELF];
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
        dmanager.createCamera(width, height, &cameraBG);
        dmanager.createCamera(width * 10000.0f, height * 10000.0f, &cameraGame);
        dmanager.createCamera(width, height, &cameraUI);
        cameraGame.posZ = -100.0f;
        cameraUI.posZ = -100.0f;
        
        // Frame buffers
        dmanager.createFrameBuffer(width, height, &fbBG);
        dmanager.createFrameBuffer(width, height, &fbGame);

        // Load screen
        if (!addTexture(hModule, TEX_BG_LOAD))
            throw "Failed to load image for load screen.";
        Fact bgLoad = Fact();
        bgLoad.posZ = 1.5f;
        bgLoad.sclX = 12.8f;
        bgLoad.sclY = 9.6f;
        applyFact(&bgLoad);
        dmanager.drawBegin(false);
        dmanager.applyCamera(&cameraUI, false);
        dmanager.applyTexture(getTexture(TEX_BG_LOAD));
        dmanager.drawModel(&ideaSquare);
        dmanager.drawEnd();

        // Load beginning
        bool flg = true;
        flg = flg && addTexture(hModule, TEX_BG_TUTORIAL);
        flg = flg && addTexture(hModule, TEX_UI_FRAME);
        flg = flg && addTexture(hModule, TEX_BU_SELF0);
        flg = flg && addTexture(hModule, TEX_CH_ATARI);
        flg = flg && addTexture(hModule, TEX_CH_SLOWCIRCLE);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_B0);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_B1);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_B2);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_B3);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_R0);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_R1);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_L0);
        flg = flg && addTexture(hModule, TEX_CH_MARISA_L1);
        flg = flg && addFont(Lpcstr2uint("0"));
        flg = flg && addFont(Lpcstr2uint("1"));
        flg = flg && addFont(Lpcstr2uint("2"));
        flg = flg && addFont(Lpcstr2uint("3"));
        flg = flg && addFont(Lpcstr2uint("4"));
        flg = flg && addFont(Lpcstr2uint("5"));
        flg = flg && addFont(Lpcstr2uint("6"));
        flg = flg && addFont(Lpcstr2uint("7"));
        flg = flg && addFont(Lpcstr2uint("8"));
        flg = flg && addFont(Lpcstr2uint("9"));
        flg = flg && addFont(Lpcstr2uint("."));
        flg = flg && addFont(Lpcstr2uint("f"));
        flg = flg && addFont(Lpcstr2uint("p"));
        flg = flg && addFont(Lpcstr2uint("s"));
        flg = flg && setKeyConfig();

        // Load ending
        if (!flg)
            throw "Failed to Load.";

        // Load data
        FILE* pFile = fopen("./savedata.dat", "r");
        if (!pFile) 
            throw "Failed to open savedata.dat.";

        // Set player
        player.r = 10;
        player.spdNorm = 800;
        player.spdSlow = 400;
        player.interval = 8;
        player.rGrz = 100;

        fclose(pFile);
        FreeLibrary(hModule);

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        FreeLibrary(hModule);
        return false;
    }

    return true;
}

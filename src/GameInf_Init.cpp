#include "../include/HeaderApp.hpp"

#include <stdio.h>

bool GameInf::init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName,
        unsigned int width, unsigned int height, bool windowed) 
{
    // Load dll
    HMODULE hModule = LoadLibraryA("./resource.dll");
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
        fontsTmp = new Texture[MAX_FNT_TMP];
        for (int i = 0; i < MAX_TEX; ++i) {
            texs[i] = Texture();
        }
        for (int i = 0; i < MAX_FNT; ++i) {
            fonts[i] = Texture();
        }
        for (int i = 0; i < MAX_FNT_TMP; ++i) {
            fontsTmp[i] = Texture();
        }

        // Storage
        queBG = new Fact[MAX_QUE_BG];
        queUI = new Fact[MAX_QUE_UI];
        queFont = new Fact[MAX_QUE_FNT];
        bulsE = new Bullet[MAX_BUL_E];
        bulsP = new Bullet[MAX_BUL_P];
        texidsLog = new unsigned int[MAX_LOGUE];
        enemies = new Enemy[MAX_ENEMY];

        // Idea
        const unsigned int kNumVtx = 4U;
        const unsigned int kNumIdx = 6U;
        idea.numIdx = kNumIdx;
        struct Vertex dataPCNU[kNumVtx] = {
            {-50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
            {-50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
            {+50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
            {+50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        };
        unsigned int dataIdx[kNumIdx] = {0, 1, 2, 0, 2, 3};
        if (!dmanager.createModelBuffers(kNumVtx, dataPCNU, dataIdx, &idea))
            throw "Failed to create idea of square.";

        // Camera
        dmanager.createCamera(width, height, &cameraBG);
        dmanager.createCamera(width * 10000.0f, height * 10000.0f, &cameraGame);
        dmanager.createCamera(width, height, &cameraUI);
        cameraGame.posZ = -100.0f;
        cameraUI.posZ = -100.0f;
        
        // Frame buffers
        dmanager.createFrameBuffer(width, height, &fb0);
        dmanager.createFrameBuffer(width, height, &fb1);

        // Load screen
        if (!loadTexture(hModule, TEX_BG_LOAD))
            throw "Failed to load image for load screen.";
        Fact bgLoad = Fact();
        bgLoad.posZ = 1.5f;
        bgLoad.sclX = 12.8f;
        bgLoad.sclY = 9.6f;
        applyFact(&bgLoad);
        dmanager.drawBegin(false);
        dmanager.applyCamera(&cameraUI, false);
        dmanager.applyTexture(getTexture(TEX_BG_LOAD));
        dmanager.drawModel(&idea);
        dmanager.drawEnd();

        // Load beginning
        bool flg = true;
        flg = flg && loadTexture(hModule, TEX_BG_TUTORIAL);
        flg = flg && loadTexture(hModule, TEX_BG_KEIDAI);
        flg = flg && loadTexture(hModule, TEX_BG_CLOUD0);
        flg = flg && loadTexture(hModule, TEX_BG_CLOUD1);
        flg = flg && loadTexture(hModule, TEX_UI_FRAME);
        flg = flg && loadTexture(hModule, TEX_BU_SELF0);
        flg = flg && loadTexture(hModule, TEX_CH_ATARI);
        flg = flg && loadTexture(hModule, TEX_CH_SLOWCIRCLE);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_B0);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_B1);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_B2);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_B3);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_R0);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_R1);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_L0);
        flg = flg && loadTexture(hModule, TEX_CH_MARISA_L1);
        flg = flg && loadTexture(hModule, TEX_CH_FAIRY_R0);
        flg = flg && loadTexture(hModule, TEX_CH_FAIRY_R1);
        flg = flg && loadTexture(hModule, TEX_TC_MARISA0);
        flg = flg && loadTexture(hModule, TEX_TC_MARISA1);
        flg = flg && loadTexture(hModule, TEX_TC_MARISA2);
        flg = flg && loadFont(Lpcstr2uint("0"));
        flg = flg && loadFont(Lpcstr2uint("1"));
        flg = flg && loadFont(Lpcstr2uint("2"));
        flg = flg && loadFont(Lpcstr2uint("3"));
        flg = flg && loadFont(Lpcstr2uint("4"));
        flg = flg && loadFont(Lpcstr2uint("5"));
        flg = flg && loadFont(Lpcstr2uint("6"));
        flg = flg && loadFont(Lpcstr2uint("7"));
        flg = flg && loadFont(Lpcstr2uint("8"));
        flg = flg && loadFont(Lpcstr2uint("9"));
        flg = flg && loadFont(Lpcstr2uint("."));
        flg = flg && loadFont(Lpcstr2uint("f"));
        flg = flg && loadFont(Lpcstr2uint("p"));
        flg = flg && loadFont(Lpcstr2uint("s"));
        flg = flg && setKeyConfig();

        // Load ending
        if (!flg)
            throw "Failed to Load.";

        // Load data
        FILE* pFile = fopen("./savedata.dat", "r");
        if (!pFile) 
            throw "Failed to open savedata.dat.";

        // Load data
        data.cntPlay = 0;
        data.cntWorldRound = 0;
        data.scoreTotalGot = 0;
        data.scoreTotal = 0;
        data.spdNorm = 800;
        data.spdSlow = 400;
        data.r = 1500;
        data.rGrz = 5000;
        data.atk = 100;
        data.interval = 8;
        data.numOpt = 0;
        data.widthShot = 0;
        data.kndShot = 0;
        data.kndSkill = 0;

        // Other
        player.fact.sclX = 8600.0f;
        player.fact.sclY = 8600.0f;

        fclose(pFile);
        FreeLibrary(hModule);

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        FreeLibrary(hModule);
        return false;
    }

    return true;
}

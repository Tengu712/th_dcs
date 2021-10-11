#include "../include/HeaderApp.h"

#include <stdio.h>

char CreateGameInf(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf,
        unsigned int width, unsigned int height)
{
    memset(pGinf, 0, sizeof(struct GameInf));

    HMODULE hModule = LoadLibraryA("./resource.dll");
    if (!hModule)
        return ThrowError("resource.dll not found.");

    pGinf->fps = 0.0f;

    // sce
    pGinf->sceCur = -1;
    pGinf->sceNex = 0;

    // idea
    CreateModelInf(&pGinf->idea);
    pGinf->idea.numIdx = 6U;
    struct Vertex dataPCNU[4U] = {
        {-50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        {-50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        {+50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
        {+50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    };
    unsigned int dataIdx[6U] = {0, 1, 2, 0, 2, 3};
    if (!CreateModelBuffer(pDinf, &pGinf->idea, 4U, dataPCNU, dataIdx)) 
        return ThrowError("Failed to create idea.");

    // frame buffers
    CreateFrameBuffer(pDinf, &pGinf->fb0, width, height);
    CreateFrameBuffer(pDinf, &pGinf->fb1, width, height);

    // cameras
    CreateCamera((float)width, (float)height, &pGinf->cameraBG);
    CreateCamera((float)width * 10000.0f, (float)height * 10000.0f, &pGinf->cameraGame);
    CreateCamera((float)width, (float)height, &pGinf->cameraUI);
    pGinf->cameraGame.posZ = -100.0f;
    pGinf->cameraUI.posZ = -100.0f;

    // storages
    pGinf->imgs = (struct Image*)malloc(sizeof(struct Image) * MAX_IMG);
    pGinf->imgsTmp = (struct Image*)malloc(sizeof(struct Image) * MAX_IMG_TMP);
    pGinf->bulsE = (struct Bullet*)malloc(sizeof(struct Bullet) * MAX_BUL_E);
    pGinf->bulsP = (struct Bullet*)malloc(sizeof(struct Bullet) * MAX_BUL_P);
    pGinf->imgidsLog = (unsigned int*)malloc(sizeof(struct Logue) * MAX_LOGUE);
    pGinf->cntSce = (int*)malloc(sizeof(int) * MAX_PARAM);
    memset(pGinf->imgs, 0, sizeof(struct Image) * MAX_IMG);
    memset(pGinf->imgsTmp, 0, sizeof(struct Image) * MAX_IMG_TMP);
    memset(pGinf->bulsE, 0, sizeof(struct Bullet) * MAX_BUL_E);
    memset(pGinf->bulsP, 0, sizeof(struct Bullet) * MAX_BUL_P);
    memset(pGinf->imgidsLog, 0, sizeof(struct Logue) * MAX_LOGUE);
    memset(pGinf->cntSce, 0, sizeof(int) * MAX_PARAM);

    // Load
    if (!LoadAddImage(pGinf, pDinf, hModule, IMG_BG_LOAD))
        return ThrowError("Failed to load image for loading.");
    pGinf->idea.sclX = 12.8f;
    pGinf->idea.sclY = 9.6f;
    DrawBegin(pDinf, NULL, FALSE);
    ApplyCamera(pDinf, &pGinf->cameraBG, FALSE);
    ApplyImage(pDinf, GetImage(pGinf, IMG_BG_LOAD));
    DrawModel(pDinf, &pGinf->idea);
    DrawEnd(pDinf);

    // font
    {
        HRSRC hFontRes = FindResourceA(hModule, MAKEINTRESOURCEA(FNT_NOTO_SANS), "IMAGE");
        if (!hFontRes)
            return ThrowError("Failed to find font resource.");
        HGLOBAL hFontData = LoadResource(hModule, hFontRes);
        if (!hFontData)
            return ThrowError("Failed to load font resource.");
        void* pLock = LockResource(hFontData);
        if (!pLock)
            return ThrowError("Failed to lock font resource.");
        DWORD sizeRes = SizeofResource(hModule, hFontRes);
        if (sizeRes == 0)
            return ThrowError("Failed to get size of font resource.");
        DWORD cntFont = 0;
        HANDLE hFont = AddFontMemResourceEx(pLock, sizeRes, NULL, &cntFont);
        if (!hFont)
            return ThrowError("Failed to add font resource.");
        LOGFONTA fonttmp = {
            64, 0, 0, 0,
            0, 0, 0, 0,
            SHIFTJIS_CHARSET,
            OUT_TT_ONLY_PRECIS,
            CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY,
            DEFAULT_PITCH | FF_MODERN,
            "Noto Sans Mono",
        };
        pGinf->font = fonttmp;
        if (!RemoveFontMemResourceEx(hFont))
            return ThrowError("Failed to remove font resource.");
    }

    // resource
    char flg = 1;
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_MAINMENU);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_MAINMENU_OVER);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_TUTORIAL);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_KEIDAI);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_CLOUD0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_CLOUD1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_FRAME);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_BOX);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_BOX_DEST);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_BOX_DETAIL);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_MAINMENU);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BU_SELF0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BU_HUDA);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_ATARI);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_SLOWCIRCLE);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_B0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_B1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_B2);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_B3);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_R0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_R1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_L0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_MARISA_L1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_FAIRY_R0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_CH_FAIRY_R1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_TC_MARISA0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_TC_MARISA1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_TC_MARISA2);
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("0"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("1"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("2"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("3"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("4"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("5"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("6"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("7"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("8"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("9"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("."));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("f"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("p"));
    flg = flg && LoadAddFont(pGinf, pDinf, Lpcstr2uint("s"));
    if (!flg) 
        return ThrowError("Failed to load.");

    // keyconfig
    {
        FILE* pKC = fopen("./keyconfig.cfg", "r");
        if (!pKC) 
            return ThrowError("Failed to open keyconfig.cfg.");
        int cntKey = 0;
        int cntBuf = 0;
        int buf = 0;
        int bufs[2] = {0,0};
        char mapKey[7] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0x5A, VK_SHIFT, VK_ESCAPE};
        while ((buf = fgetc(pKC)) != EOF) {
            if (buf != 'u' && buf != 'd' && buf != 'l' && buf != 'r' 
                    && buf != 's' && buf != 't' && buf != 'b' && buf != 'c'
                    && buf != '1' && buf != '2' && buf != '3'
                    && buf != 'a' && buf != 'b' && buf != 'x' && buf != 'y' && buf != ',')
                continue;
            if (cntKey >= 7 || cntBuf > 2)
                return ThrowError("Invalid keyconfig.");
            if (buf == ',') {
                char t = 0;
                short c = 0;
                if (bufs[0] == 'l' && bufs[1] == '2') {
                    t = MD_GAMEPAD_KEYTYPE_LTRIGGER;
                    c = 128;
                } else if (bufs[0] == 'r' && bufs[1] == '2') {
                    t = MD_GAMEPAD_KEYTYPE_RTRIGGER;
                    c = 128;
                } else if (bufs[0] == 'l' && bufs[1] == 'l') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBLL;
                    c = 128;
                } else if (bufs[0] == 'l' && bufs[1] == 'r') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBLR;
                    c = 128;
                } else if (bufs[0] == 'l' && bufs[1] == 'u') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBLU;
                    c = 128;
                } else if (bufs[0] == 'l' && bufs[1] == 'd') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBLD;
                    c = 128;
                } else if (bufs[0] == 'r' && bufs[1] == 'l') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBRL;
                    c = 128;
                } else if (bufs[0] == 'r' && bufs[1] == 'r') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBRR;
                    c = 128;
                } else if (bufs[0] == 'r' && bufs[1] == 'u') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBRU;
                    c = 128;
                } else if (bufs[0] == 'r' && bufs[1] == 'd') {
                    t = MD_GAMEPAD_KEYTYPE_THUMBRD;
                    c = 128;
                } else if (bufs[0] == 's' && bufs[1] == 't') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_START;
                } else if (bufs[0] == 'b' && bufs[1] == 'c') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_BACK;
                } else if (bufs[0] == 'l' && bufs[1] == '1') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_LEFT_SHOULDER;
                } else if (bufs[0] == 'r' && bufs[1] == '1') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_RIGHT_SHOULDER;
                } else if (bufs[0] == 'l' && bufs[1] == '3') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_LEFT_THUMB;
                } else if (bufs[0] == 'r' && bufs[1] == '3') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_RIGHT_THUMB;
                } else if (bufs[0] == 'u') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_DPAD_UP;
                } else if (bufs[0] == 'd') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_DPAD_DOWN;
                } else if (bufs[0] == 'l') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_DPAD_LEFT;
                } else if (bufs[0] == 'r') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_DPAD_RIGHT;
                } else if (bufs[0] == 'a') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_A;
                } else if (bufs[0] == 'b') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_B;
                } else if (bufs[0] == 'x') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_X;
                } else if (bufs[0] == 'y') {
                    t = MD_GAMEPAD_KEYTYPE_BUTTONS;
                    c = XINPUT_GAMEPAD_Y;
                } else
                    return ThrowError("Invalid keycode.");
                if (!AddKeycode(pIinf, cntKey + 1, mapKey[cntKey], t, c))
                    return ThrowError("Failed to add key.");
                cntKey++;
                cntBuf = 0;
                bufs[0] = 0;
                bufs[1] = 0;
                continue;
            }
            bufs[cntBuf] = buf;
            cntBuf++;
        }
        fclose(pKC);
    }

    //! data
    {
        FILE* pSD = fopen("./savedata.dat", "r");
        if (!pSD) 
            return ThrowError("Failed to open savedata.dat.");
        memset(&pGinf->data, 0, sizeof(struct SaveData));
        pGinf->data.cntPlay = 0;
        pGinf->data.cntWorldRound = 1;
        pGinf->data.scoreTotalGot = 10000000LL;
        pGinf->data.scoreTotal = 10000000LL;
        pGinf->data.scoreInit = 10000000LL;
        pGinf->data.spdNorm = 800;
        pGinf->data.spdSlow = 400;
        pGinf->data.r = 100000;
        pGinf->data.rGrz = 500000;
        pGinf->data.atk = 100;
        pGinf->data.interval = 8;
        pGinf->data.invtime = 240;
        pGinf->data.numOpt = 0;
        pGinf->data.widthShot = 0;
        pGinf->data.kndShot = 0;
        pGinf->data.kndSkill = 0;
        fclose(pSD);
    }

    FreeLibrary(hModule);
    return 1;
}

void FreeGameInf(struct GameInf* pGinf) {
    FreeModelInf(&pGinf->idea);
    FreeFrameBuffer(&pGinf->fb0);
    FreeFrameBuffer(&pGinf->fb1);
    if (pGinf->imgs != NULL)
        free(pGinf->imgs);
    if (pGinf->imgsTmp != NULL)
        free(pGinf->imgsTmp);
    if (pGinf->bulsE != NULL)
        free(pGinf->bulsE);
    if (pGinf->bulsP != NULL)
        free(pGinf->bulsP);
    if (pGinf->imgidsLog != NULL)
        free(pGinf->imgidsLog);
    if (pGinf->cntSce != NULL)
        free(pGinf->cntSce);
    memset(pGinf, 0, sizeof(struct GameInf));
}

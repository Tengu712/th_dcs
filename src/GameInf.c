#include "../include/HeaderApp.h"

#include <stdio.h>

char CreateGameInf(struct GameInf* pGinf, struct D3DInf* pDinf, unsigned int width, unsigned int height) {

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

    //! data
    FILE* pFile = fopen("./savedata.dat", "r");
    if (!pFile) 
        return ThrowError("Failed to open savedata.dat.");
    memset(&pGinf->data, 0, sizeof(struct SaveData));
    pGinf->data.cntPlay = 0;
    pGinf->data.cntWorldRound = 0;
    pGinf->data.scoreTotalGot = 0;
    pGinf->data.scoreTotal = 0;
    pGinf->data.spdNorm = 800;
    pGinf->data.spdSlow = 400;
    pGinf->data.r = 1500;
    pGinf->data.rGrz = 5000;
    pGinf->data.atk = 100;
    pGinf->data.interval = 8;
    pGinf->data.numOpt = 0;
    pGinf->data.widthShot = 0;
    pGinf->data.kndShot = 0;
    pGinf->data.kndSkill = 0;

    // cameras
    CreateCamera((float)width, (float)height, &pGinf->cameraBG);
    CreateCamera((float)width * 10000.0f, (float)height * 10000.0f, &pGinf->cameraGame);
    CreateCamera((float)width, (float)height, &pGinf->cameraUI);
    pGinf->cameraGame.posZ = -100.0f;
    pGinf->cameraUI.posZ = -100.0f;

    // storages
    pGinf->imgs = (struct Image*)malloc(sizeof(struct Image) * MAX_IMG);
    pGinf->imgsTmp = (struct Image*)malloc(sizeof(struct Image) * MAX_IMG_TMP);
    pGinf->queBG = (struct Fact*)malloc(sizeof(struct Fact) * MAX_QUE_BG);
    pGinf->queUI = (struct Fact*)malloc(sizeof(struct Fact) * MAX_QUE_UI);
    pGinf->bulsE = (struct Bullet*)malloc(sizeof(struct Bullet) * MAX_BUL_E);
    pGinf->bulsP = (struct Bullet*)malloc(sizeof(struct Bullet) * MAX_BUL_P);
    pGinf->binfs = (struct BulletInf*)malloc(sizeof(struct BulletInf) * MAX_KND_BUL);
    pGinf->imgidsLog = (unsigned int*)malloc(sizeof(struct Logue) * MAX_LOGUE);

    CreatePlayer(&pGinf->player);
    memset(&pGinf->log, 0, sizeof(struct Logue));

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

    char flg = 1;
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_TUTORIAL);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_KEIDAI);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_CLOUD0);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BG_CLOUD1);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_UI_FRAME);
    flg = flg && LoadAddImage(pGinf, pDinf, hModule, IMG_BU_SELF0);
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
    //flg = flg && setKeyConfig();

    if (!flg) 
        return ThrowError("Failed to load.");

    struct BulletInf binf0 = {
            50000, 100000,
            5000.0f, 5000.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
    pGinf->binfs[0] = binf0;

    fclose(pFile);
    FreeLibrary(hModule);
    return 1;
}

void UpdateGameInf(struct GameInf* pGinf) {
    for (int i = 0; i < MAX_QUE_BG; ++i) {
        pGinf->queBG[i].enable = 0;
    }
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        pGinf->queUI[i].enable = 0;
    }
    pGinf->data.cntPlay++;
}

void FreeGameInf(struct GameInf* pGinf) {
    FreeModelInf(&pGinf->idea);
    FreeFrameBuffer(&pGinf->fb0);
    FreeFrameBuffer(&pGinf->fb1);
    if (pGinf->imgs != NULL)
        free(pGinf->imgs);
    if (pGinf->imgsTmp != NULL)
        free(pGinf->imgsTmp);
    if (pGinf->queBG != NULL)
        free(pGinf->queBG);
    if (pGinf->queUI != NULL)
        free(pGinf->queUI);
    if (pGinf->bulsE != NULL)
        free(pGinf->bulsE);
    if (pGinf->bulsP != NULL)
        free(pGinf->bulsP);
    if (pGinf->binfs != NULL)
        free(pGinf->binfs);
    if (pGinf->imgidsLog != NULL)
        free(pGinf->imgidsLog);
    memset(pGinf, 0, sizeof(struct GameInf));
}

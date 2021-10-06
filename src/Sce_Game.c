#include "../include/HeaderScene.h"


void InitGame(struct GameInf* pGinf) {
    CreatePlayer(&pGinf->player);
}

void UpdateGame(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
        UpdatePlayer(pGinf, pIinf, &pGinf->player);
        for (int i = 0; i < MAX_BUL_P; ++i) {
            if (pGinf->bulsP[i].flg == 0)
                continue;
            UpdateBullet(pGinf, &pGinf->bulsP[i]);
            //! hit
        }
        for (int i = 0; i < MAX_BUL_E; ++i) {
            if (pGinf->bulsE[i].flg == 0)
                continue;
            UpdateBullet(pGinf, &pGinf->bulsE[i]);
            //! hit
        }
}

inline void DrawImage(struct GameInf* pGinf, struct D3DInf* pDinf, struct Fact* pFact, struct Image* pImage) {
    ApplyFact(pGinf, pFact);
    ApplyImage(pDinf, pImage);
    DrawModel(pDinf, &pGinf->idea);
}

void DrawGame(struct GameInf* pGinf, struct D3DInf* pDinf) {

    struct Fact fact;
    CreateFact(&fact);

    // Player Bullets : game 1 f

    DrawBegin(pDinf, &pGinf->fb1, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);
    DrawImage(pGinf, pDinf, &fact, &pGinf->fb0.image);

    ApplyCamera(pDinf, &pGinf->cameraGame, FALSE);

    {
        CreateFact(&fact);
        fact.posX = (float)pGinf->player.x;
        fact.posY = (float)pGinf->player.y;
        fact.sclX = 8600.0f;
        fact.sclY = 8600.0f;
        int imgid = IMG_CH_MARISA_B0;
        if (pGinf->player.dx == 1)
            imgid = IMG_CH_MARISA_R0 + ((pGinf->player.cnt / 8) % 2);
        else if (pGinf->player.dx == -1)
            imgid = IMG_CH_MARISA_L0 + ((pGinf->player.cnt / 8) % 2);
        else
            imgid = IMG_CH_MARISA_B0 + ((pGinf->player.cnt / 8) % 4);
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, imgid));
    }

    int texCur = 0;
    CreateFact(&fact);
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (pGinf->bulsP[i].flg == 0)
            continue;
        if (texCur != pGinf->binfs[pGinf->bulsP[i].knd].imgid) {
            texCur = pGinf->binfs[pGinf->bulsP[i].knd].imgid;
            ApplyImage(pDinf, GetImage(pGinf, texCur));
        }
        fact.posX = (float)pGinf->bulsP[i].x;
        fact.posY = (float)pGinf->bulsP[i].y;
        fact.degZ = (float)pGinf->bulsP[i].deg - 90.0f;
        fact.sclX = pGinf->binfs[pGinf->bulsP[i].knd].sclX;
        fact.sclY = pGinf->binfs[pGinf->bulsP[i].knd].sclY;
        fact.colR = pGinf->binfs[pGinf->bulsP[i].knd].colR;
        fact.colG = pGinf->binfs[pGinf->bulsP[i].knd].colG;
        fact.colB = pGinf->binfs[pGinf->bulsP[i].knd].colB;
        fact.colA = pGinf->binfs[pGinf->bulsP[i].knd].colA;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (pGinf->bulsE[i].flg == 0)
            continue;
        if (texCur != pGinf->binfs[pGinf->bulsE[i].knd].imgid) {
            texCur = pGinf->binfs[pGinf->bulsE[i].knd].imgid;
            ApplyImage(pDinf, GetImage(pGinf, texCur));
        }
        fact.posX = (float)pGinf->bulsE[i].x;
        fact.posY = (float)pGinf->bulsE[i].y;
        fact.degZ = (float)pGinf->bulsE[i].deg - 90.0f;
        fact.sclX = pGinf->binfs[pGinf->bulsE[i].knd].sclX;
        fact.sclY = pGinf->binfs[pGinf->bulsE[i].knd].sclY;
        fact.colR = pGinf->binfs[pGinf->bulsE[i].knd].colR;
        fact.colG = pGinf->binfs[pGinf->bulsE[i].knd].colG;
        fact.colB = pGinf->binfs[pGinf->bulsE[i].knd].colB;
        fact.colA = pGinf->binfs[pGinf->bulsE[i].knd].colA;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    if (pGinf->player.cntSlow > 0U) {
        CreateFact(&fact);
        fact.posX = (float)pGinf->player.x;
        fact.posY = (float)pGinf->player.y;
        // Atari
        fact.sclX = (float)pGinf->data.r + 300.0f;
        fact.sclY = (float)pGinf->data.r + 300.0f;
        if (pGinf->player.cntSlow < 10U) {
            fact.sclX += 500.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
            fact.sclY += 500.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
        } else 
            fact.degZ = (float)pGinf->player.cntSlow * 4.0f;
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_CH_ATARI));
        // Circle
        fact.sclX = 13000.0f;
        fact.sclY = 13000.0f;
        if (pGinf->player.cntSlow < 10U) {
            fact.sclX += 5000.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
            fact.sclY += 5000.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
        }
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_CH_SLOWCIRCLE));
        fact.degZ *= -1.0f;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    // UI Font Frame Logue : ui 1 f

    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_FRAME));

    if (pGinf->log.flg) {
        // Tachie
        if (pGinf->log.imgidL != 0) {
            CreateFact(&fact);
            fact.posX = -390.0f;
            fact.posY = -160.0f;
            fact.sclX = 6.5f;
            fact.sclY = 6.5f;
            DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, pGinf->log.imgidL));
        }
        // Box
        CreateFact(&fact);
        fact.posY = -350.0f;
        fact.sclX = 6.0f;
        fact.colR = 0.0f;
        fact.colG = 0.0f;
        fact.colB = 0.0f;
        fact.colA = 0.5f;
        DrawImage(pGinf, pDinf, &fact, NULL);
        // Logue
        CreateFact(&fact);
        fact.posX = -260.0f;
        fact.posY = -320.0f;
        fact.sclX = 0.27f;
        fact.sclY = 0.27f;
        for (int i = 0; i < MAX_LOGUE; ++i) {
            if (pGinf->imgidsLog[i] == 0)
                break;
            fact.posX += 27.0f;
            DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, pGinf->imgidsLog[i]));
        }
    }

    // Fps : ui - f

    DrawBegin(pDinf, NULL, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);
    DrawImage(pGinf, pDinf, &fact, &pGinf->fb1.image);

    DrawFps(pDinf, pGinf);


    DrawEnd(pDinf);
}

#include "../include/HeaderScene.h"

void InitGame(struct GameInf* pGinf) {
    pGinf->cntAll = 0U;
    pGinf->cntSce = 0U;
    pGinf->mode = 0U;
    pGinf->score = pGinf->data.scoreInit;
    pGinf->grz = 0;
    memset(&pGinf->player, 0, sizeof(struct Player));
    pGinf->player.y = -2400000;
    memset(&pGinf->enemy, 0, sizeof(struct Enemy));
    pGinf->enemy.y = 2400000;
    memset(&pGinf->log, 0, sizeof(struct Logue));
    ClearFontTmp(pGinf);
}

inline char IsHit(int x1, int y1, int r1, int x2, int y2, int r2) {
    const unsigned long long dis = 
        (unsigned long long)(x1 - x2) * (unsigned long long)(x1 - x2) + 
        (unsigned long long)(y1 - y2) * (unsigned long long)(y1 - y2);
    const unsigned long long r =
        (unsigned long long)(r1 + r2) * (unsigned long long)(r1 + r2);
    return dis < r;
}

inline void Died(struct GameInf* pGinf) {
    pGinf->score -= 10000000LL * min(8, pGinf->data.cntWorldRound);
    memset(&pGinf->player, 0, sizeof(struct Player));
    pGinf->player.y = -2400000;
}

void UpdateGame(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
    UpdatePlayer(pGinf, pIinf, &pGinf->player);
    MoveEntity(&pGinf->enemy.x, &pGinf->enemy.y, pGinf->enemy.deg, pGinf->enemy.spd);
    pGinf->enemy.cnt++;
    if (IsHit(pGinf->player.x, pGinf->player.y, pGinf->data.r,
                pGinf->enemy.x, pGinf->enemy.y, 600000)) {
        Died(pGinf); //#Score
    }
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (pGinf->bulsP[i].flg == 0)
            continue;
        UpdateBullet(pGinf, &pGinf->bulsP[i]);
        if (IsHit(pGinf->bulsP[i].x, pGinf->bulsP[i].y, pGinf->bulsP[i].r,
                    pGinf->enemy.x, pGinf->enemy.y, 600000)) {
            pGinf->enemy.hp -= pGinf->bulsP[i].atk;
            pGinf->score += 10; //#Score
            pGinf->bulsP[i].flg = 0;
        }
    }
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (pGinf->bulsE[i].flg == 0)
            continue;
        UpdateBullet(pGinf, &pGinf->bulsE[i]);
        if (IsHit(pGinf->bulsE[i].x, pGinf->bulsE[i].y, pGinf->bulsE[i].r,
                    pGinf->player.x, pGinf->player.y, pGinf->data.rGrz)) {
            pGinf->score += 1; //#Score
            pGinf->bulsE[i].flg = 2;
        }
        if (IsHit(pGinf->bulsE[i].x, pGinf->bulsE[i].y, pGinf->bulsE[i].r,
                    pGinf->player.x, pGinf->player.y, pGinf->data.r)) {
            Died(pGinf); //#Score
            pGinf->bulsE[i].flg = 0;
        }
    }
}

void DrawGame(struct GameInf* pGinf, struct D3DInf* pDinf) {

    struct Fact fact;
    CreateFact(&fact);

    // ============================================================================================================= //

    DrawBegin(pDinf, &pGinf->fb1, FALSE);

    // Draw frame buffer
    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);
    DrawImage(pGinf, pDinf, &fact, &pGinf->fb0.image);

    // Camera for game
    ApplyCamera(pDinf, &pGinf->cameraGame, FALSE);

    // Player
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

    // Enemy
    CreateFact(&fact);
    fact.posX = (float)pGinf->enemy.x;
    fact.posY = (float)pGinf->enemy.y;
    fact.sclX = 12000.0f;
    fact.sclY = 12000.0f;
    DrawImage(pGinf, pDinf, &fact, NULL);

    // Bullet player's
    int texCur = 0;
    CreateFact(&fact);
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (!pGinf->bulsP[i].flg)
            continue;
        if (texCur != pGinf->bulsP[i].imgid) {
            texCur = pGinf->bulsP[i].imgid;
            ApplyImage(pDinf, GetImage(pGinf, texCur));
        }
        fact.posX = (float)pGinf->bulsP[i].x;
        fact.posY = (float)pGinf->bulsP[i].y;
        fact.degZ = (float)pGinf->bulsP[i].deg - 90.0f;
        fact.sclX = pGinf->bulsP[i].sclX;
        fact.sclY = pGinf->bulsP[i].sclY;
        fact.colR = pGinf->bulsP[i].colR;
        fact.colG = pGinf->bulsP[i].colG;
        fact.colB = pGinf->bulsP[i].colB;
        fact.colA = pGinf->bulsP[i].colA;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    // Bullet enemy's
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (!pGinf->bulsE[i].flg)
            continue;
        if (texCur != pGinf->bulsE[i].imgid) {
            texCur = pGinf->bulsE[i].imgid;
            ApplyImage(pDinf, GetImage(pGinf, texCur));
        }
        fact.posX = (float)pGinf->bulsE[i].x;
        fact.posY = (float)pGinf->bulsE[i].y;
        fact.degZ = (float)pGinf->bulsE[i].deg - 90.0f;
        fact.sclX = pGinf->bulsE[i].sclX;
        fact.sclY = pGinf->bulsE[i].sclY;
        fact.colR = pGinf->bulsE[i].colR;
        fact.colG = pGinf->bulsE[i].colG;
        fact.colB = pGinf->bulsE[i].colB;
        fact.colA = pGinf->bulsE[i].colA;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    // Slow circle
    if (pGinf->player.cntSlow > 0U) {
        CreateFact(&fact);
        fact.posX = (float)pGinf->player.x;
        fact.posY = (float)pGinf->player.y;
        // Atari
        fact.sclX = (float)pGinf->data.r * 2.0f;
        fact.sclY = (float)pGinf->data.r * 2.0f;
        if (pGinf->player.cntSlow < 10U) {
            fact.sclX += 500.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
            fact.sclY += 500.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
        } else 
            fact.degZ = (float)pGinf->player.cntSlow * 4.0f;
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_CH_ATARI));
        // Circle
        fact.sclX = (float)pGinf->data.rGrz * 2.0f;
        fact.sclY = (float)pGinf->data.rGrz * 2.0f;
        if (pGinf->player.cntSlow < 10U) {
            fact.sclX += 5000.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
            fact.sclY += 5000.0f * (1.0f - (float)pGinf->player.cntSlow / 10.0f);
        }
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_CH_SLOWCIRCLE));
        fact.degZ *= -1.0f;
        ApplyFact(pGinf, &fact);
        DrawModel(pDinf, &pGinf->idea);
    }

    // ============================================================================================================= //

    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);

    // HP bar
    if (pGinf->enemy.hp > 0 && pGinf->enemy.hpMax > 0) {
        CreateFact(&fact);
        fact.posX = -375.0f * (1.0f - (float)pGinf->enemy.hp / (float)pGinf->enemy.hpMax);
        fact.posY = 460.0f;
        fact.sclX = 7.5f - 7.5f * (1.0f - (float)pGinf->enemy.hp / (float)pGinf->enemy.hpMax);
        fact.sclY = 0.08f;
        DrawImage(pGinf, pDinf, &fact, NULL);
    }

    // Score and graze
    {
        CreateFact(&fact);
        fact.posX = -145.0f;
        fact.posY = 435.0f;
        fact.sclX = 0.22f;
        fact.sclY = 0.25f;
        long long dev = 1LL;
        for (int i = 0; i < 10; ++i) {
            fact.posX -= 22.0f;
            DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->score / dev) % 10) + 48U)));
            dev *= 10;
        }
        fact.posX = -293.0f;
        fact.posY = 410.0f;
        fact.sclX = 0.18;
        fact.sclY = 0.20f;
        dev = 1LL;
        for (int i = 0; i < 4; ++i) {
            fact.posX -= 18.0f;
            DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->grz / dev) % 10) + 48U)));
            dev *= 10;
        }
    }

    // Timer
    if (pGinf->enemy.cnt > 0 && pGinf->enemy.timlim > pGinf->enemy.cnt) {
        CreateFact(&fact);
        fact.posX = 340.0f;
        fact.posY = 435.0f;
        fact.sclX = 0.22f;
        fact.sclY = 0.25f;
        DrawImage(pGinf, pDinf, &fact, 
            GetImage(pGinf, ToFontID((((pGinf->enemy.timlim - pGinf->enemy.cnt) / 600) % 10) + 48U)));
        fact.posX += 22.0f;
        DrawImage(pGinf, pDinf, &fact, 
            GetImage(pGinf, ToFontID((((pGinf->enemy.timlim - pGinf->enemy.cnt) / 60) % 10) + 48U)));
    }

    // Frame
    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_FRAME));

    // Logue
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

    // ============================================================================================================= //

    DrawBegin(pDinf, NULL, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);
    DrawImage(pGinf, pDinf, &fact, &pGinf->fb1.image);

    DrawFps(pDinf, pGinf);

    DrawEnd(pDinf);
}

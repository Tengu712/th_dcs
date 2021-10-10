#include "../include/HeaderScene.h"

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
    pGinf->grz = 0;
    memset(&pGinf->player, 0, sizeof(struct Player));
    pGinf->player.y = -2400000;
}

char InitGame(struct Infs* pinfs) {
    pinfs->pGinf->cntSce0 = 0U;
    pinfs->pGinf->cntSce1 = 0U;
    pinfs->pGinf->cntSce2 = 0U;
    pinfs->pGinf->cntSce3 = 0U;
    pinfs->pGinf->score = pinfs->pGinf->data.scoreInit;
    pinfs->pGinf->grz = 0;
    memset(&pinfs->pGinf->player, 0, sizeof(struct Player));
    pinfs->pGinf->player.y = -2400000;
    memset(&pinfs->pGinf->enemy, 0, sizeof(struct Enemy));
    pinfs->pGinf->enemy.y = 2400000;
    memset(&pinfs->pGinf->log, 0, sizeof(struct Logue));
    ClearFontTmp(pinfs->pGinf);
    return 1;
}

void UpdateGame(struct Infs* pinfs, void (*fLog)(struct Infs*),
        void (*fGame)(struct Infs*), void (*fWin)(struct Infs*), void (*fBG)(struct Infs*))
{
    // ============================================================================================================= //
    //                                            Update
    // ============================================================================================================= //

    // Switch pause
    if (GetKey(pinfs->pIinf, KEY_CODE_Esc) & KEY_STA_Down
            && pinfs->pGinf->cntSce1 != SCE_GAME_Result
            && pinfs->pGinf->cntSce1 != SCE_GAME_GameOver) {
        if (pinfs->pGinf->cntSce1 == SCE_GAME_Pause) {
            pinfs->pGinf->cntSce1 = 0; //!
        } else {
            pinfs->pGinf->cntSce1 = SCE_GAME_Pause;
        }
    }

    // Update scene
    if (pinfs->pGinf->cntSce1 == SCE_GAME_Logue)
        fLog(pinfs);
    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Game)
        fGame(pinfs);
    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Win)
        fWin(pinfs);

    // Update entities
    if (pinfs->pGinf->cntSce1 != SCE_GAME_Pause
            && pinfs->pGinf->cntSce1 != SCE_GAME_Result
            && pinfs->pGinf->cntSce1 != SCE_GAME_GameOver) {
        const char isGame = pinfs->pGinf->cntSce1 == SCE_GAME_Game;

        UpdatePlayer(pinfs->pGinf, pinfs->pIinf, &pinfs->pGinf->player, isGame && pinfs->pGinf->score >= 0);

        MoveEntity(&pinfs->pGinf->enemy.x, &pinfs->pGinf->enemy.y, pinfs->pGinf->enemy.deg, pinfs->pGinf->enemy.spd);
        pinfs->pGinf->enemy.cnt++;

        if (isGame
                && IsHit(pinfs->pGinf->player.x, pinfs->pGinf->player.y, pinfs->pGinf->data.r,
                    pinfs->pGinf->enemy.x, pinfs->pGinf->enemy.y, 600000)) {
            Died(pinfs->pGinf); //#Score
        }

        for (int i = 0; i < MAX_BUL_P; ++i) {
            if (pinfs->pGinf->bulsP[i].flg == 0)
                continue;
            UpdateBullet(pinfs->pGinf, &pinfs->pGinf->bulsP[i]);
            if (isGame
                    && IsHit(pinfs->pGinf->bulsP[i].x, pinfs->pGinf->bulsP[i].y, pinfs->pGinf->bulsP[i].r,
                        pinfs->pGinf->enemy.x, pinfs->pGinf->enemy.y, 600000)) {
                pinfs->pGinf->enemy.hp -= pinfs->pGinf->bulsP[i].atk;
                pinfs->pGinf->score += 10; //#Score
                pinfs->pGinf->bulsP[i].flg = 0;
            }
        }
        for (int i = 0; i < MAX_BUL_E; ++i) {
            if (pinfs->pGinf->bulsE[i].flg == 0)
                continue;
            UpdateBullet(pinfs->pGinf, &pinfs->pGinf->bulsE[i]);
            if (isGame
                    && pinfs->pGinf->bulsE[i].flg == 1
                    && IsHit(pinfs->pGinf->bulsE[i].x, pinfs->pGinf->bulsE[i].y, pinfs->pGinf->bulsE[i].r,
                        pinfs->pGinf->player.x, pinfs->pGinf->player.y, pinfs->pGinf->data.rGrz)) {
                pinfs->pGinf->score += min(1000, pinfs->pGinf->grz) * min(1000, pinfs->pGinf->grz) * 0.5; //#Score
                pinfs->pGinf->bulsE[i].flg = 2;
                pinfs->pGinf->grz++;
            }
            if (isGame
                    && IsHit(pinfs->pGinf->bulsE[i].x, pinfs->pGinf->bulsE[i].y, pinfs->pGinf->bulsE[i].r,
                        pinfs->pGinf->player.x, pinfs->pGinf->player.y, pinfs->pGinf->data.r)) {
                Died(pinfs->pGinf); //#Score
                pinfs->pGinf->bulsE[i].flg = 0;
            }
        }
    }

    // ============================================================================================================= //
    //                                            Frame Buffer 0
    // ============================================================================================================= //

    struct Fact fact;
    CreateFact(&fact);

    DrawBegin(pinfs->pDinf, &pinfs->pGinf->fb0, TRUE);

    fBG(pinfs);

    // ============================================================================================================= //
    //                                            Frame Buffer 1
    // ============================================================================================================= //

    DrawBegin(pinfs->pDinf, &pinfs->pGinf->fb1, FALSE);

    // Draw frame buffer
    if (pinfs->pGinf->cntSce1 != SCE_GAME_Result && pinfs->pGinf->cntSce1 != SCE_GAME_GameOver) {
        CreateFact(&fact);
        fact.sclX = 12.8f;
        fact.sclY = 9.6f;
        ApplyCamera(pinfs->pDinf, &pinfs->pGinf->cameraUI, FALSE);
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, &pinfs->pGinf->fb0.image);
    }

    // Camera for game
    ApplyCamera(pinfs->pDinf, &pinfs->pGinf->cameraGame, FALSE);

    // Player
    if (pinfs->pGinf->score >= 0) {
        CreateFact(&fact);
        fact.posX = (float)pinfs->pGinf->player.x;
        fact.posY = (float)pinfs->pGinf->player.y;
        fact.sclX = 8600.0f;
        fact.sclY = 8600.0f;
        int imgid = IMG_CH_MARISA_B0;
        if (pinfs->pGinf->player.dx == 1)
            imgid = IMG_CH_MARISA_R0 + ((pinfs->pGinf->player.cnt / 8) % 2);
        else if (pinfs->pGinf->player.dx == -1)
            imgid = IMG_CH_MARISA_L0 + ((pinfs->pGinf->player.cnt / 8) % 2);
        else
            imgid = IMG_CH_MARISA_B0 + ((pinfs->pGinf->player.cnt / 8) % 4);
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, imgid));
    }

    // Enemy
    CreateFact(&fact);
    fact.posX = (float)pinfs->pGinf->enemy.x;
    fact.posY = (float)pinfs->pGinf->enemy.y;
    fact.sclX = 12000.0f;
    fact.sclY = 12000.0f;
    DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, NULL);

    // change shader pram
    pinfs->pDinf->cbuffer.params.y = 1.0;

    // Bullet player's
    int texCur = 0;
    CreateFact(&fact);
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (!pinfs->pGinf->bulsP[i].flg)
            continue;
        if (texCur != pinfs->pGinf->bulsP[i].imgid) {
            texCur = pinfs->pGinf->bulsP[i].imgid;
            ApplyImage(pinfs->pDinf, GetImage(pinfs->pGinf, texCur));
        }
        fact.posX = (float)pinfs->pGinf->bulsP[i].x;
        fact.posY = (float)pinfs->pGinf->bulsP[i].y;
        fact.degZ = (float)pinfs->pGinf->bulsP[i].deg - 90.0f;
        fact.sclX = pinfs->pGinf->bulsP[i].sclX;
        fact.sclY = pinfs->pGinf->bulsP[i].sclY;
        fact.colR = pinfs->pGinf->bulsP[i].colR;
        fact.colG = pinfs->pGinf->bulsP[i].colG;
        fact.colB = pinfs->pGinf->bulsP[i].colB;
        fact.colA = pinfs->pGinf->bulsP[i].colA;
        ApplyFact(pinfs->pGinf, &fact);
        DrawModel(pinfs->pDinf, &pinfs->pGinf->idea);
    }

    // Bullet enemy's
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (!pinfs->pGinf->bulsE[i].flg)
            continue;
        if (texCur != pinfs->pGinf->bulsE[i].imgid) {
            texCur = pinfs->pGinf->bulsE[i].imgid;
            ApplyImage(pinfs->pDinf, GetImage(pinfs->pGinf, texCur));
        }
        fact.posX = (float)pinfs->pGinf->bulsE[i].x;
        fact.posY = (float)pinfs->pGinf->bulsE[i].y;
        fact.degZ = (float)pinfs->pGinf->bulsE[i].deg - 90.0f;
        fact.sclX = pinfs->pGinf->bulsE[i].sclX;
        fact.sclY = pinfs->pGinf->bulsE[i].sclY;
        fact.colR = pinfs->pGinf->bulsE[i].colR;
        fact.colG = pinfs->pGinf->bulsE[i].colG;
        fact.colB = pinfs->pGinf->bulsE[i].colB;
        fact.colA = pinfs->pGinf->bulsE[i].colA;
        ApplyFact(pinfs->pGinf, &fact);
        DrawModel(pinfs->pDinf, &pinfs->pGinf->idea);
    }

    // change shader pram
    pinfs->pDinf->cbuffer.params.y = 0.0;

    // Slow circle
    if (pinfs->pGinf->score >= 0 && pinfs->pGinf->player.cntSlow > 0U) {
        CreateFact(&fact);
        fact.posX = (float)pinfs->pGinf->player.x;
        fact.posY = (float)pinfs->pGinf->player.y;
        // Atari
        fact.sclX = (float)pinfs->pGinf->data.r * 0.02f;
        fact.sclY = (float)pinfs->pGinf->data.r * 0.02f;
        if (pinfs->pGinf->player.cntSlow < 10U) {
            fact.sclX += 500.0f * (1.0f - (float)pinfs->pGinf->player.cntSlow / 10.0f);
            fact.sclY += 500.0f * (1.0f - (float)pinfs->pGinf->player.cntSlow / 10.0f);
        } else 
            fact.degZ = (float)pinfs->pGinf->player.cntSlow * 4.0f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_CH_ATARI));
        // Circle
        fact.sclX = (float)pinfs->pGinf->data.rGrz * 0.02f;
        fact.sclY = (float)pinfs->pGinf->data.rGrz * 0.02f;
        if (pinfs->pGinf->player.cntSlow < 10U) {
            fact.sclX += 5000.0f * (1.0f - (float)pinfs->pGinf->player.cntSlow / 10.0f);
            fact.sclY += 5000.0f * (1.0f - (float)pinfs->pGinf->player.cntSlow / 10.0f);
        }
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_CH_SLOWCIRCLE));
        fact.degZ *= -1.0f;
        ApplyFact(pinfs->pGinf, &fact);
        DrawModel(pinfs->pDinf, &pinfs->pGinf->idea);
    }

    // ============================================================================================================= //

    ApplyCamera(pinfs->pDinf, &pinfs->pGinf->cameraUI, FALSE);

    // HP bar
    if (pinfs->pGinf->enemy.hp > 0 && pinfs->pGinf->enemy.hpMax > 0) {
        CreateFact(&fact);
        fact.posX = -375.0f * (1.0f - (float)pinfs->pGinf->enemy.hp / (float)pinfs->pGinf->enemy.hpMax);
        fact.posY = 460.0f;
        fact.sclX = 7.5f - 7.5f * (1.0f - (float)pinfs->pGinf->enemy.hp / (float)pinfs->pGinf->enemy.hpMax);
        fact.sclY = 0.08f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, NULL);
    }

    // Score and graze
    if (pinfs->pGinf->score >= 0) {
        CreateFact(&fact);
        fact.posX = -145.0f;
        fact.posY = 435.0f;
        fact.sclX = 0.22f;
        fact.sclY = 0.25f;
        long long dev = 1LL;
        for (int i = 0; i < 10; ++i) {
            fact.posX -= 22.0f;
            DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, ToFontID(((pinfs->pGinf->score / dev) % 10) + 48U)));
            dev *= 10;
        }
        fact.posX = -293.0f;
        fact.posY = 410.0f;
        fact.sclX = 0.18;
        fact.sclY = 0.20f;
        dev = 1LL;
        for (int i = 0; i < 4; ++i) {
            fact.posX -= 18.0f;
            DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, ToFontID(((pinfs->pGinf->grz / dev) % 10) + 48U)));
            dev *= 10;
        }
    }

    // Timer
    if (pinfs->pGinf->enemy.cnt > 0 && pinfs->pGinf->enemy.timlim > pinfs->pGinf->enemy.cnt) {
        CreateFact(&fact);
        fact.posX = 340.0f;
        fact.posY = 435.0f;
        fact.sclX = 0.22f;
        fact.sclY = 0.25f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, 
                GetImage(pinfs->pGinf, ToFontID((((pinfs->pGinf->enemy.timlim - pinfs->pGinf->enemy.cnt) / 600) % 10) + 48U)));
        fact.posX += 22.0f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, 
                GetImage(pinfs->pGinf, ToFontID((((pinfs->pGinf->enemy.timlim - pinfs->pGinf->enemy.cnt) / 60) % 10) + 48U)));
    }

    // ============================================================================================================= //
    //                                            Back Buffer
    // ============================================================================================================= //

    DrawBegin(pinfs->pDinf, NULL, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    ApplyCamera(pinfs->pDinf, &pinfs->pGinf->cameraUI, FALSE);
    ApplyFact(pinfs->pGinf, &fact);
    ApplyImage(pinfs->pDinf, &pinfs->pGinf->fb1.image);
    if (pinfs->pGinf->cntSce1 == SCE_GAME_Pause)
        pinfs->pDinf->cbuffer.params.x = 2.0;
    DrawModel(pinfs->pDinf, &pinfs->pGinf->idea);
    pinfs->pDinf->cbuffer.params.x = 0.0;

    // Frame
    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_UI_FRAME));

    // Logue
    if (pinfs->pGinf->log.flg && pinfs->pGinf->cntSce1 != SCE_GAME_Pause) {
        // Tachie
        if (pinfs->pGinf->log.imgidL != 0) {
            CreateFact(&fact);
            if (pinfs->pGinf->log.isRight) {
                fact.posX = -440.0f;
                fact.posY = -190.0f;
                fact.sclX = 6.5f;
                fact.sclY = 6.5f;
                fact.colR = 0.5f;
                fact.colG = 0.5f;
                fact.colB = 0.5f;
            }
            else {
                fact.posX = -390.0f;
                fact.posY = -160.0f;
                fact.sclX = 6.5f;
                fact.sclY = 6.5f;
            }
            DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, pinfs->pGinf->log.imgidL));
        }
        // Box
        CreateFact(&fact);
        fact.posY = -350.0f;
        fact.sclX = 6.0f;
        fact.colR = 0.0f;
        fact.colG = 0.0f;
        fact.colB = 0.0f;
        fact.colA = 0.5f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, NULL);
        // Logue
        CreateFact(&fact);
        fact.posX = -260.0f;
        fact.posY = -320.0f;
        fact.sclX = 0.27f;
        fact.sclY = 0.27f;
        for (int i = 0; i < MAX_LOGUE; ++i) {
            if (pinfs->pGinf->imgidsLog[i] == 0)
                break;
            fact.posX += 27.0f;
            DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, pinfs->pGinf->imgidsLog[i]));
        }
    }

    DrawFps(pinfs->pDinf, pinfs->pGinf);

    DrawEnd(pinfs->pDinf);
}

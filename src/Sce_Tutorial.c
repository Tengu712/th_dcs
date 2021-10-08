#include "../include/HeaderScene.h"

char InitTutorial(struct GameInf* pGinf, struct D3DInf* pDinf) {
    InitGame(pGinf);
    pGinf->enemy.x -= 3400000;
    pGinf->enemy.y += 3400000;
    char res = 1;
    res = res && LoadSentence(pGinf, pDinf, "やっぱ異変と言えばここだよな");
    res = res && LoadSentence(pGinf, pDinf, "ちょっと！");
    res = res && LoadSentence(pGinf, pDinf, "うちが異変起こしているみたいじゃない");
    return res;
}

void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {

    // Logue
    if (pGinf->mode == 0U) {
        char flg = 0;
        if (GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Down) 
            flg = 1;
        if (pGinf->cntSce == 0U)
            ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA0, 0, "やっぱ異変と言えばここだよな");
        else if (pGinf->cntSce >= 1U && pGinf->cntSce <= 31U) {
            ApplyLogue(pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");
            pGinf->enemy.deg = -45;
            pGinf->enemy.spd = 1600;
            pGinf->cntSce++;
            flg = 0;
        }
        else if (pGinf->cntSce == 32U) {
            ApplyLogue(pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");
            pGinf->enemy.spd = 0;
        }
        else if (pGinf->cntSce == 33U) {
            pGinf->enemy.spd = 0;
            ApplyLogue(pGinf, 1, 1, IMG_TC_MARISA0, 0, "うちが異変起こしているみたいじゃない");
        }
        else {
            ApplyLogue(pGinf, 0, 0, 0, 0, "");
            pGinf->enemy.hp = 10000;
            pGinf->enemy.hpMax = 10000;
            pGinf->enemy.timlim = 1800;
            pGinf->cntSce = 0;
            pGinf->mode = 1U;
        }
        if (flg)
            pGinf->cntSce++;
        UpdateGame(pGinf, pDinf, pIinf);
        pGinf->cntAll++;
    }
    // Game
    else if (pGinf->mode == 1U) {
        if (pGinf->cntSce == 0) {
            if (pGinf->enemy.cnt % 10 == 0) {
                for (int i = 0; i < 72; ++i) {
                    struct Bullet bul;
                    CreateBullet(&bul, BUL_HUDA);
                    bul.spd = 500;
                    bul.deg = i * 360 / 72;
                    bul.colB = 0.0f;
                    bul.colG = 0.0f;
                    pushBulletE(pGinf, &bul);
                }
            }
            if (pGinf->enemy.hp < 0) {
                pGinf->enemy.hp = 10000;
                pGinf->enemy.hpMax = 10000;
                pGinf->enemy.timlim = 1800;
                pGinf->enemy.cnt = 0;
                pGinf->cntSce++;
            }
        }
        else {
            if (pGinf->enemy.hp < 0) {
                pGinf->enemy.hp = 0;
                pGinf->enemy.hpMax = 10000;
                pGinf->enemy.timlim = 1800;
                pGinf->enemy.cnt = 0;
                pGinf->cntSce = 0;
                pGinf->mode = 2U;
            }
        }
        UpdateGame(pGinf, pDinf, pIinf);
        pGinf->cntAll++;
    }
    // Clear
    else if (pGinf->mode == 2U) {
        if (GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Down)
            pGinf->cntSce++;
        if (pGinf->cntSce == 0U)
            ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA1, 0, "1010");
        else {
            ApplyLogue(pGinf, 0, 0, 0, 0, "");
            pGinf->cntSce = 0;
            pGinf->mode = 3U;
        }
        UpdateGame(pGinf, pDinf, pIinf);
        pGinf->cntAll++;
    }
    // Result
    else if (pGinf->mode == 3U) {
        pGinf->sceNex = SCE_MainMenu;
    }

    DrawGame(pGinf, pDinf);
}

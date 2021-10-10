#include "../include/HeaderScene.h"

char InitTutorial(struct Infs* pinfs) {
    InitGame(pinfs);
    pinfs->pGinf->enemy.x -= 3400000;
    pinfs->pGinf->enemy.y += 3400000;
    char res = 1;
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "やっぱ異変と言えばここだよな");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "ちょっと！");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "うちが異変起こしているみたいじゃない");
    return res;
}

void UpdateTutorial(struct Infs* pinfs) {
    SwitchPause(pinfs);

    if (pinfs->pGinf->cntSce1 == SCE_GAME_Logue) {
        char flg = 0;
        if (GetKey(pinfs->pIinf, KEY_CODE_Z) & KEY_STA_Down) 
            flg = 1;
        if (pinfs->pGinf->cntSce2 == 0U)
            ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA0, 0, "やっぱ異変と言えばここだよな");
        else if (pinfs->pGinf->cntSce2 == 1U && pinfs->pGinf->cntSce3 >= 0 && pinfs->pGinf->cntSce3 < 30U) {
            ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");
            pinfs->pGinf->enemy.deg = -45;
            pinfs->pGinf->enemy.spd = 1600;
            pinfs->pGinf->cntSce3++;
            flg = 0;
        }
        else if (pinfs->pGinf->cntSce2 == 1U && pinfs->pGinf->cntSce3 == 30U) {
            ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");
            pinfs->pGinf->enemy.spd = 0;
        }
        else if (pinfs->pGinf->cntSce2 == 2U) {
            ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, 0, "うちが異変起こしているみたいじゃない");
            pinfs->pGinf->cntSce3 = 0U;
        }
        else {
            ApplyLogue(pinfs->pGinf, 0, 0, 0, 0, "");
            pinfs->pGinf->enemy.hp = 10000;
            pinfs->pGinf->enemy.hpMax = 10000;
            pinfs->pGinf->enemy.timlim = 1800;
            pinfs->pGinf->cntSce1 = SCE_GAME_Game;
            pinfs->pGinf->cntSce2 = 0U;
            pinfs->pGinf->cntSce3 = 0U;
        }
        if (flg)
            pinfs->pGinf->cntSce2++;
        UpdateGame(pinfs);
        pinfs->pGinf->cntSce0++;
    }

    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Game) {
        if (pinfs->pGinf->cntSce2 == 0) {
            if (pinfs->pGinf->enemy.cnt % 10 == 0) {
                for (int i = 0; i < 72; ++i) {
                    struct Bullet bul;
                    CreateBullet(&bul, BUL_HUDA);
                    bul.spd = 500;
                    bul.deg = i * 360 / 72;
                    bul.colB = 0.0f;
                    bul.colG = 0.0f;
                    pushBulletE(pinfs->pGinf, &bul);
                }
            }
            if (pinfs->pGinf->enemy.hp < 0) {
                pinfs->pGinf->enemy.hp = 10000;
                pinfs->pGinf->enemy.hpMax = 10000;
                pinfs->pGinf->enemy.timlim = 1800;
                pinfs->pGinf->enemy.cnt = 0;
                pinfs->pGinf->cntSce2++;
            }
        }
        else {
            if (pinfs->pGinf->enemy.hp < 0) {
                pinfs->pGinf->enemy.hp = 0;
                pinfs->pGinf->enemy.hpMax = 10000;
                pinfs->pGinf->enemy.timlim = 1800;
                pinfs->pGinf->enemy.cnt = 0;
                pinfs->pGinf->cntSce1 = SCE_GAME_Win;
                pinfs->pGinf->cntSce2 = 0U;
                pinfs->pGinf->cntSce3 = 0U;
            }
        }
        UpdateGame(pinfs);
        pinfs->pGinf->cntSce0++;
    }

    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Win) {
        if (GetKey(pinfs->pIinf, KEY_CODE_Z) & KEY_STA_Down)
            pinfs->pGinf->cntSce2++;
        if (pinfs->pGinf->cntSce2 == 0U)
            ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA1, 0, "1010");
        else {
            ApplyLogue(pinfs->pGinf, 0, 0, 0, 0, "");
            pinfs->pGinf->cntSce1 = SCE_GAME_Result;
            pinfs->pGinf->cntSce2 = 0U;
            pinfs->pGinf->cntSce3 = 0U;
        }
        UpdateGame(pinfs);
        pinfs->pGinf->cntSce0++;
    }

    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Result) {
        pinfs->pGinf->sceNex = SCE_MainMenu;
    }

    else if (pinfs->pGinf->cntSce1 == SCE_GAME_Pause) {

    }

    DrawGame(pinfs);
}

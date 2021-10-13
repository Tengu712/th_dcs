#include "../include/HeaderScene.h"

void FuncTutorialLogue(struct Infs* pinfs) {
    char flg = GetKey(pinfs->pIinf, KEY_CODE_Z) & KEY_STA_Down;

    if (pinfs->pGinf->cntSce[CNT_STATE] == 0U)
        ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA0, 0, "やっぱ異変と言えばここだよな");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 1U && pinfs->pGinf->cntSce[CNT_FADE] >= 0) {
        ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");
        pinfs->pGinf->enemy.deg = -45;
        pinfs->pGinf->enemy.spd = 1600;
        pinfs->pGinf->cntSce[CNT_FADE]++;
        flg = 0;
        if (pinfs->pGinf->cntSce[CNT_FADE] == 30U) {
            pinfs->pGinf->enemy.spd = 0;
            pinfs->pGinf->cntSce[CNT_STATE]++;
            pinfs->pGinf->cntSce[CNT_FADE] = 0U;
        }
    }

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 2U)
        ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, 0, "ちょっと！");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 3U)
        ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, IMG_TC_REIMU1, "うちが異変起こしているみたいじゃない");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 4U)
        ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA1, IMG_TC_REIMU1, "言葉の綾だよ");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 5U)
        ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA0, IMG_TC_REIMU0, "霊夢にもやっぱり見えているのか？　　「あれ」");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 6U)
        ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA0, IMG_TC_REIMU0, "勿論。そこにあんたが現れた");

    else if (pinfs->pGinf->cntSce[CNT_STATE] == 7U)
        ApplyLogue(pinfs->pGinf, 1, 1, IMG_TC_MARISA1, IMG_TC_REIMU1, "取り敢えず倒れなさい！");

    else {
        ApplyLogue(pinfs->pGinf, 0, 0, 0, 0, "");
        pinfs->pGinf->enemy.hp = 10000;
        pinfs->pGinf->enemy.hpMax = 10000;
        pinfs->pGinf->enemy.timlim = 1800;
        pinfs->pGinf->cntSce[CNT_MODE] = SCE_GAME_Game;
        pinfs->pGinf->cntSce[CNT_STATE] = 0U;
        pinfs->pGinf->cntSce[CNT_FADE] = 0U;
    }

    if (flg)
        pinfs->pGinf->cntSce[CNT_STATE]++;
    pinfs->pGinf->cntSce[CNT_ALL]++;
}

void FuncTutorialGame(struct Infs* pinfs) {
    if (pinfs->pGinf->cntSce[CNT_STATE] == 0) {
        if (pinfs->pGinf->cntSce[CNT_FADE] % 10 == 0) {
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
            pinfs->pGinf->cntSce[CNT_FADE] = 0U;
            pinfs->pGinf->cntSce[CNT_STATE]++;
        }
    }
    else {
        if (pinfs->pGinf->enemy.hp < 0) {
            pinfs->pGinf->enemy.hp = 0;
            pinfs->pGinf->enemy.hpMax = 10000;
            pinfs->pGinf->enemy.timlim = 1800;
            pinfs->pGinf->cntSce[CNT_MODE] = SCE_GAME_Win;
            pinfs->pGinf->cntSce[CNT_STATE] = 0U;
            pinfs->pGinf->cntSce[CNT_FADE] = 0U;
        }
    }
    pinfs->pGinf->cntSce[CNT_FADE]++;
    pinfs->pGinf->cntSce[CNT_ALL]++;
}

void FuncTutorialWin(struct Infs* pinfs) {
    if (GetKey(pinfs->pIinf, KEY_CODE_Z) & KEY_STA_Down)
        pinfs->pGinf->cntSce[CNT_STATE]++;
    if (pinfs->pGinf->cntSce[CNT_STATE] == 0U)
        ApplyLogue(pinfs->pGinf, 1, 0, IMG_TC_MARISA1, 0, "1010");
    else {
        ApplyLogue(pinfs->pGinf, 0, 0, 0, 0, "");
        pinfs->pGinf->cntSce[CNT_MODE] = SCE_GAME_Result;
        pinfs->pGinf->cntSce[CNT_STATE] = 0U;
        pinfs->pGinf->cntSce[CNT_FADE] = 0U;
    }
    pinfs->pGinf->cntSce[CNT_ALL]++;
}

void FuncTutorialBG(struct Infs* pinfs) {

}

char InitTutorial(struct Infs* pinfs) {
    InitGame(pinfs);
    pinfs->pGinf->enemy.x -= 3400000;
    pinfs->pGinf->enemy.y += 3400000;
    char res = 1;
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "やっぱ異変と言えばここだよな");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "ちょっと！");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "うちが異変起こしているみたいじゃない");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "言葉の綾だよ");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "霊夢にもやっぱり見えているのか？　「あれ」");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "勿論。そこにあんたが現れた");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "取り敢えず倒れなさい！");
    return res;
}

void UpdateTutorial(struct Infs* pinfs) {
    UpdateGame(pinfs, FuncTutorialLogue, FuncTutorialGame, FuncTutorialWin, FuncTutorialBG);
}
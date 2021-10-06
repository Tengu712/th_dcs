#include "../include/HeaderScene.h"

void InitTutorial(struct GameInf* pGinf, struct D3DInf* pDinf) {
    InitGame(pGinf);
    LoadSentence(pGinf, pDinf, "なんていうか今回は");
}

void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {

    // Logue
    if (pGinf->mode == 0U) {
        if (GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Down)
            pGinf->cntSce++;
        if (pGinf->cntSce == 0U)
            ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA0, 0, "なんていうか今回は");
        else {
            ApplyLogue(pGinf, 0, 0, 0, 0, "");
            pGinf->enemy.hp = 10000;
            pGinf->enemy.hpMax = 10000;
            pGinf->enemy.timlim = 1800;
            pGinf->cntSce = 0;
            pGinf->mode = 1U;
        }
        pGinf->player.cnt++;
        pGinf->cntAll++;
    }
    // Game
    else if (pGinf->mode == 1U) {
        if (pGinf->cntSce == 0) {
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
    }
    // Clear
    else if (pGinf->mode == 2U) {

    }
    // Result
    else if (pGinf->mode == 3U) {
        
    }

    DrawGame(pGinf, pDinf);
}

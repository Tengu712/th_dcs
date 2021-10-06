#include "../include/HeaderScene.h"

unsigned int cntAll = 0;
unsigned int cntSce = 0;
unsigned int mode = 0;

void InitTutorial(struct GameInf* pGinf, struct D3DInf* pDinf) {
    InitGame(pGinf);
    ClearFontTmp(pGinf);
    LoadSentence(pGinf, pDinf, "なんていうか今回は");
    cntAll = 0U;
    cntSce = 0U;
    mode = 0U;
}

void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {

    // Logue
    if (mode == 0U) {
        if (GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Down)
            cntSce++;
        if (cntSce == 0U)
            ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA0, 0, "なんていうか今回は");
        else {
            ApplyLogue(pGinf, 0, 0, 0, 0, "");
            cntSce = 0;
            mode = 1U;
        }
        pGinf->player.cnt++;
        cntAll++;
    }
    // Game
    else if (mode == 1U) {
        UpdateGame(pGinf, pDinf, pIinf);
    }

    DrawGame(pGinf, pDinf);
}

#include "../include/HeaderScene.h"

int mode = 0;

void InitTutorial(struct GameInf* pGinf) {
    InitGame(pGinf);
    mode = 0;
}

void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
    UpdateGame(pGinf, pDinf, pIinf);

    ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA0, 0, "0110.");

    DrawGame(pGinf, pDinf);
}

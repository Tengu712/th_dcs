#include "../include/HeaderScene.h"

void InitTutorial(struct GameInf* pGinf) {
    InitGame(pGinf);
}

void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
    const char mode = UpdateGame(pGinf, pDinf, pIinf);

    ApplyLogue(pGinf, 1, 0, IMG_TC_MARISA0, 0, "0110.");

    DrawGame(pGinf, pDinf);
}

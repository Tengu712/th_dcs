#include "../include/HeaderScene.h"

char InitTitle(struct GameInf* pGinf, struct D3DInf* pDinf) {
    return 1;
}

void UpdateTitle(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
    pGinf->sceNex = SCE_Tutorial;
}
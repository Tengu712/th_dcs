#include "../include/HeaderApp.h"

void pushBG(struct GameInf* pGinf, struct Fact* pFact) {
    for (int i = 0; i < MAX_QUE_BG; ++i) {
        if (pGinf->queBG[i].enable)
            continue;
        pGinf->queBG[i] = *pFact;
        pGinf->queBG[i].enable = 1;
        break;
    }
}

void pushUI(struct GameInf* pGinf, struct Fact* pFact) {
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        if (pGinf->queUI[i].enable)
            continue;
        pGinf->queUI[i] = *pFact;
        pGinf->queUI[i].enable = 1;
        break;
    }
}

void pushBulletE(struct GameInf* pGinf, struct Bullet* pBul) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (pGinf->bulsE[i].flg != 0)
            continue;
        pGinf->bulsE[i] = *pBul;
        pGinf->bulsE[i].flg = 1;
        return;
    }
}

void pushBulletP(struct GameInf* pGinf, struct Bullet* pBul) {
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (pGinf->bulsP[i].flg != 0)
            continue;
        pGinf->bulsP[i] = *pBul;
        pGinf->bulsP[i].flg = 1;
        return;
    }
}

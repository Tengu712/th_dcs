#include "../include/HeaderApp.h"

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

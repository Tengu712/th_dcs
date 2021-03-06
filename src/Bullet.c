#include "../include/HeaderApp.h"

void CreateBullet(struct Bullet* pBul, unsigned int knd) {
    memset(pBul, 0, sizeof(struct Bullet));
    pBul->colR = 1.0f;
    pBul->colG = 1.0f;
    pBul->colB = 1.0f;
    pBul->colA = 1.0f;
    if (knd == BUL_SELF_0) {
        pBul->imgid = IMG_BU_SELF0;
        pBul->r = 10000;
        pBul->sclX = 5000.0f;
        pBul->sclY = 5000.0f;
    }
    else if (knd == BUL_HUDA) {
        pBul->imgid = IMG_BU_HUDA;
        pBul->r = 10000;
        pBul->sclX = 3000.0f;
        pBul->sclY = 3000.0f;
    }
}

void UpdateBullet(struct GameInf* pGinf, struct Bullet* pBul) {
    //! pattern

    MoveEntity(&pBul->x, &pBul->y, pBul->deg, pBul->spd);

    if (pBul->x > 5000000 || pBul->x < -5000000 || pBul->y > 6000000 || pBul->y < -6000000)
        pBul->flg = 0;
    
    pBul->cnt++;
}

void ClearBulletE(struct GameInf* pGinf) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        //!
        pGinf->bulsE[i].flg = 0;
    }
}

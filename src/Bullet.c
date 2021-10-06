#include "../include/HeaderApp.h"

void CreateBullet(struct Bullet* pBul, unsigned int knd) {
    memset(pBul, 0, sizeof(struct Bullet));
    pBul->colR = 1.0f;
    pBul->colG = 1.0f;
    pBul->colB = 1.0f;
    pBul->colA = 1.0f;
    if (knd == BUL_SELF_0) {
        pBul->imgid = IMG_BU_SELF0;
        pBul->r = 1000;
        pBul->sclX = 5000.0f;
        pBul->sclY = 5000.0f;
    }
}

void UpdateBullet(struct GameInf* pGinf, struct Bullet* pBul) {
    //! pattern

    MoveEntity(&pBul->x, &pBul->y, pBul->deg, pBul->spd);

    if (pBul->x > 5000000 || pBul->x < -5000000 || pBul->y > 6000000 || pBul->y < -6000000)
        pBul->flg = 0;
    
    pBul->cnt++;
}

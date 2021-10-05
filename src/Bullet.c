#include "../include/HeaderApp.h"

#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void CreateBullet(struct Bullet* pBul, unsigned int knd) {
    memset(pBul, 0, sizeof(struct Bullet));
    pBul->knd;
}

void UpdateBullet(struct GameInf* pGinf, struct Bullet* pBul) {
    //! pattern

    const double rad = (double)pBul->deg / 180.0 * M_PI;
    pBul->x += (int)(cos(rad) * 10000.0f) * pBul->spd / 100;
    pBul->y += (int)(sin(rad) * 10000.0f) * pBul->spd / 100;

    if (pBul->x > 5000000 || pBul->x < -5000000 || pBul->y > 6000000 || pBul->y < -6000000)
        pBul->flg = 0;
    
    pBul->cnt++;
}

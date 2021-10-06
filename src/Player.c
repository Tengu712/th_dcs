#include "../include/HeaderApp.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

const int kDegs[3][3] = {
    {225, 180, 135},
    {270, 999, 90},
    {315, 0, 45}
};

void CreatePlayer(struct Player* pPlayer) {
    memset(pPlayer, 0, sizeof(struct Player));
    pPlayer->y = -2400000;
}

void UpdatePlayer(struct GameInf* pGinf, struct InputInf* pIinf, struct Player* pPlayer) {
    if ((GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Pressed) && pPlayer->cnt % pGinf->data.interval == 0) {
        struct Bullet bul;
        // Main shot
        CreateBullet(&bul, BUL_SELF_0);
        bul.x = pPlayer->x - 150000;
        bul.y = pPlayer->y + 100000;
        bul.deg = 90;
        bul.spd = 4000;
        bul.atk = pGinf->data.atk;
        pushBulletP(pGinf, &bul);
        bul.x = pPlayer->x + 150000;
        pushBulletP(pGinf, &bul);
    }

    if (GetKey(pIinf, KEY_CODE_Shift) & KEY_STA_Pressed)
        pPlayer->cntSlow++;
    else
        pPlayer->cntSlow = 0U;

    const int dx = (GetKey(pIinf, KEY_CODE_Right) & KEY_STA_Pressed > 0) - (GetKey(pIinf, KEY_CODE_Left) & KEY_STA_Pressed > 0);
    const int dy = (GetKey(pIinf, KEY_CODE_Up) & KEY_STA_Pressed > 0) - (GetKey(pIinf, KEY_CODE_Down) & KEY_STA_Pressed > 0);

    int spd = pPlayer->cntSlow > 0U ? pGinf->data.spdSlow : pGinf->data.spdNorm;
    int deg = kDegs[dx + 1][dy + 1];
    if (dx == 0 && dy == 0)
        spd = 0;

    const double rad = (double)deg / 180.0 * M_PI;
    pPlayer->x += (int)(cos(rad) * 10000.0f) * spd / 100;
    pPlayer->y += (int)(sin(rad) * 10000.0f) * spd / 100;

    pPlayer->x = max(-3800000, min(3800000, pPlayer->x));
    pPlayer->y = max(-4600000, min(4600000, pPlayer->y));

    pPlayer->dx = dx;

    pPlayer->cnt++;
}

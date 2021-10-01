#include "../include/HeaderApp.hpp"

#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void GameInf::createBullet(Bullet* bul, int knd) {
    bul->moving = true;
    bul->fact.texid = knd;
    if (knd == TEX_BU_SELF0) {
        bul->fact.sclX = 5000.0f;
        bul->fact.sclY = 5000.0f;
    }
    else {
        bul->moving = false;
        bul->fact.texid = 0;
    }
}

void GameInf::pushBulletE(Bullet* bul) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (bulsE[i].moving)
            continue;
        bulsE[i] = *bul;
        return;
    }
}

void GameInf::pushBulletP(Bullet* bul) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (bulsP[i].moving)
            continue;
        bulsP[i] = *bul;
        return;
    }
}

void GameInf::updateEntity(Entity* pEntity) {
    const double rad = (double)pEntity->deg / 360.0 * 2.0 * M_PI;
    pEntity->x += (int)(cos(rad) * 10000.0f) * pEntity->spd / 100;
    pEntity->y += (int)(sin(rad) * 10000.0f) * pEntity->spd / 100;
}

void GameInf::updateBullet(Bullet* pBul){
    updateEntity(pBul);
    if (pBul->x > 5000000 || pBul->x < -5000000 || pBul->y > 6000000 || pBul->y < -6000000)
        pBul->moving = false;
    pBul->fact.posX = (float)pBul->x;
    pBul->fact.posY = (float)pBul->y;
    pBul->cnt++;
}

void GameInf::updateBullets() {
    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (!bulsP[i].moving)
            continue;
        const int flgCur = bulsP[i].flgHit;
        updateBullet(&bulsP[i]);
        if (bulsP[i].flgHit == 2)
            bulsE[i].moving = false;
    }

    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (!bulsE[i].moving)
            continue;
        const int flgCur = bulsE[i].flgHit;
        updateBullet(&bulsE[i]);
        if (bulsE[i].flgHit == 2)
            bulsE[i].moving = false;
    }
}

void GameInf::updatePlayer() {

    if (getKey(KEY_CODE::Z, KEY_STA::Pressed) && player.cnt % data.interval == 0) {
        Bullet bul = Bullet();
        // Main shot
        createBullet(&bul, TEX_BU_SELF0);
        bul.x = player.x - 150000;
        bul.y = player.y + 100000;
        bul.deg = 90;
        bul.spd = 4000;
        bul.atk = data.atk;
        pushBulletP(&bul);
        bul.x = player.x + 150000;
        pushBulletP(&bul);
    }

    if (getKey(KEY_CODE::Shift, KEY_STA::Pressed))
        player.cntSlow++;
    else
        player.cntSlow = 0U;

    const int dx = getKey(KEY_CODE::Right, KEY_STA::Pressed) - getKey(KEY_CODE::Left, KEY_STA::Pressed);
    const int dy = getKey(KEY_CODE::Up, KEY_STA::Pressed) - getKey(KEY_CODE::Down, KEY_STA::Pressed);

    player.spd = player.cntSlow > 0U ? data.spdSlow : data.spdNorm;
    if (dx == 1 && dy == 0)
        player.deg = 0;
    else if (dx == 1 && dy == 1)
        player.deg = 45;
    else if (dx == 0 && dy == 1)
        player.deg = 90;
    else if (dx == -1 && dy == 1)
        player.deg = 135;
    else if (dx == -1 && dy == 0)
        player.deg = 180;
    else if (dx == -1 && dy == -1)
        player.deg = 225;
    else if (dx == 0 && dy == -1)
        player.deg = 270;
    else if (dx == 1 && dy == -1)
        player.deg = 315;
    else
        player.spd = 0;

    updateEntity(&player);
    player.x = max(-3800000, min(3800000, player.x));
    player.y = max(-4600000, min(4600000, player.y));

    player.fact.posX = (float)player.x;
    player.fact.posY = (float)player.y;
    if (dx == 1)
        player.fact.texid = TEX_CH_MARISA_R0 + ((player.cnt / 6) % 2);
    else if (dx == -1)
        player.fact.texid = TEX_CH_MARISA_L0 + ((player.cnt / 6) % 2);
    else
        player.fact.texid = TEX_CH_MARISA_B0 + ((player.cnt / 8) % 4);

    player.cnt++;
}
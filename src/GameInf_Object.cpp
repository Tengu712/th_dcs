#include "../include/HeaderApp.hpp"

void GameInf::createBullet(Bullet* pBul, int knd) {
    pBul->moving = true;
    pBul->fact.texid = knd;
    if (knd == TEX_BU_SELF0) {
        pBul->fact.sclX = 5000.0f;
        pBul->fact.sclY = 5000.0f;
    }
    else
        pBul->moving = false;
}

void GameInf::createEnemy(Enemy* pEnemy, int knd) {
    pEnemy->moving = true;
    pEnemy->fact.texid = knd;
    if (knd == TEX_CH_FAIRY_R0) {
        pEnemy->hp = 100;
        pEnemy->fact.sclX = 5000.0f;
        pEnemy->fact.sclY = 5000.0f;
    }
    else
        pEnemy->moving = false;
}

void GameInf::pushBulletE(Bullet* pBul) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (bulsE[i].moving)
            continue;
        bulsE[i] = *pBul;
        return;
    }
}

void GameInf::pushBulletP(Bullet* pBul) {
    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (bulsP[i].moving)
            continue;
        bulsP[i] = *pBul;
        return;
    }
}

void GameInf::pushEnemy(Enemy* pEnemy) {
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (enemies[i].moving)
            continue;
        enemies[i] = *pEnemy;
        return;
    }
}
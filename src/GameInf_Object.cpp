#include "../include/HeaderApp.hpp"

void GameInf::applyLogue(bool flg, bool right, unsigned int texidLeft, unsigned int texidRight, LPCSTR str) {
    for (int i = 0; i < MAX_LOGUE; ++i) {
        texidsLog[i] = 0;
    }
    log.flg = flg;
    log.right = right;
    log.texidLeft = texidLeft;
    log.texidRight = texidRight;
    int cnt = 0;
    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        unsigned int res = 0;
        if (IsDBCSLeadByte(str[i])) {
            res = (unsigned char)str[i] << 8 | (unsigned char)str[i + 1];
            i++;
        } else
            res = (unsigned int)str[i];
        texidsLog[cnt] = res;
        cnt++;
    }
}

void GameInf::createBullet(Bullet* pBul, int knd) {
    pBul->moving = true;
    pBul->fact.texid = knd;
    if (knd == TEX_BU_SELF0) {
        pBul->fact.sclX = 5000.0f;
        pBul->fact.sclY = 5000.0f;
        pBul->r = 50000;
        pBul->rGrz = 1;
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
        pEnemy->r = 400000;
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

int GameInf::isHit(Bullet* pBul, Entity* pEntity) {
    const unsigned long long dis = 
        (long long)(pBul->x - pEntity->x) * (long long)(pBul->x - pEntity->x)
        + (long long)(pBul->y - pEntity->y) * (long long)(pBul->y - pEntity->y);
    const unsigned long long r = (long long)(pBul->r + pEntity->r) * (long long)(pBul->r + pEntity->r);
    const unsigned long long rGrz = (long long)(pBul->rGrz + pEntity->rGrz) * (long long)(pBul->rGrz + pEntity->rGrz);
    if (dis < r) {
        pBul->flgHit = 2;
        return 2;
    }
    if (dis < rGrz && pBul->flgHit == 0) {
        pBul->flgHit = 1;
        return 1;
    }
    return 0;
}

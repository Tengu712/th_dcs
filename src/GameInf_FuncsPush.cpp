#include "../include/HeaderApp.hpp"

void GameInf::pushBG(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_BG; ++i) {
        if (queBG[i].enable)
            continue;
        queBG[i] = *pFact;
        queBG[i].enable = true;
        break;
    }
}

void GameInf::pushUI(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        if (queUI[i].enable)
            continue;
        queUI[i] = *pFact;
        queUI[i].enable = true;
        break;
    }
}

void GameInf::pushFont(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_FNT; ++i) {
        if (queFont[i].enable)
            continue;
        queFont[i] = *pFact;
        queFont[i].enable = true;
        break;
    }
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
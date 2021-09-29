#include "../include/HeaderApp.hpp"

bool GameInf::addTexture(HMODULE hModule, unsigned int id) {
    for (int i = 0; i < MAX_TEX; ++i) {
        if (texs[i].id != 0)
            continue;
        if (!dmanager.createTexture(hModule, id, &texs[i]))
            return false;
        texs[i].id = id;
        break;
    }
    return true;
}

bool GameInf::addFont(unsigned int code) {
    for (int i = 0; i < MAX_FNT; ++i) {
        if (fonts[i].id != 0)
            continue;
        if (!dmanager.createFont(code, &fonts[i]))
            return false;
        fonts[i].id = code;
        break;
    }
    return true;
}

Texture* GameInf::getTexture(unsigned int id) {
    for (int i = 0; i < MAX_TEX; ++i) {
        if (texs[i].id == id)
            return &texs[i];
    }
    return nullptr;
}

void GameInf::pushBG(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_BG; ++i) {
        if (queBG[i] != nullptr)
            continue;
        queBG[i] = pFact;
        break;
    }
}

Texture* GameInf::getFont(unsigned int code) {
    for (int i = 0; i < MAX_FNT; ++i) {
        if (fonts[i].id == code)
            return &fonts[i];
    }
    return nullptr;
}

void GameInf::pushUI(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        if (queUI[i] != nullptr)
            continue;
        queUI[i] = pFact;
        break;
    }
}

void GameInf::pushFont(Fact* pFact) {
    for (int i = 0; i < MAX_QUE_FNT; ++i) {
        if (queFont[i] != nullptr)
            continue;
        queFont[i] = pFact;
        break;
    }
}

void GameInf::pushBullet(Bullet* bullet) {
    for (int i = 0; i < MAX_KND_BUL; ++i) {
        if (bullet->fact.texid != mapBullets[i])
            continue;
        for (int k = 0; k < MAX_NUM_BUL; ++k) {
            if (!bullets[i][k].moving) {
                bullets[i][k] = *bullet;
                return;
            }
        }
    }
    for (int i = 0; i < MAX_KND_BUL; ++i) {
        if (mapBullets[i] != 0)
            continue;
        bullets[i][0] = *bullet;
        mapBullets[i] = bullet->fact.texid;
        return;
    }
}

void GameInf::pushBulletSelf(Bullet* bullet) {
    for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
        if (bullet->fact.texid != mapBulletsSelf[i])
            continue;
        for (int k = 0; k < MAX_NUM_BUL_SELF; ++k) {
            if (!bulletsSelf[i][k].moving) {
                bulletsSelf[i][k] = *bullet;
                return;
            }
        }
    }
    for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
        if (mapBulletsSelf[i] != 0)
            continue;
        bulletsSelf[i][0] = *bullet;
        mapBulletsSelf[i] = bullet->fact.texid;
        return;
    }
}

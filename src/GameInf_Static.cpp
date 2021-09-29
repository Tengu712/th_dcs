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

void GameInf::applyFact(Fact* pFact) {
    idea.posX = pFact->posX;
    idea.posY = pFact->posY;
    idea.posZ = pFact->posZ;
    idea.degX = pFact->degX;
    idea.degY = pFact->degY;
    idea.degZ = pFact->degZ;
    idea.sclX = pFact->sclX;
    idea.sclY = pFact->sclY;
    idea.sclZ = pFact->sclZ;
    idea.colR = pFact->colR;
    idea.colG = pFact->colG;
    idea.colB = pFact->colB;
    idea.colA = pFact->colA;
}

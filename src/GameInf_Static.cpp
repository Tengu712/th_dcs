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

void GameInf::clearFontTmp() {
    if (fontsTmp != nullptr)
        delete fontsTmp;
    fontsTmp = new Texture[MAX_FNT_TMP];
}

bool GameInf::addFontTmp(unsigned int code) {
    for (int i = 0; i < MAX_FNT_TMP; ++i) {
        if (fontsTmp[i].id != 0)
            continue;
        if (!dmanager.createFont(code, &fontsTmp[i]))
            return false;
        fontsTmp[i].id = code;
        break;
    }
    return true;
}

bool GameInf::addSentence(LPCSTR str) {
    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (IsDBCSLeadByte(str[i])) {
            if (!addFontTmp((unsigned char)str[i] << 8 | (unsigned char)str[i + 1]))
                return false;
            i++;
        } else {
            if (!addFontTmp((unsigned int)str[i]))
                return false;
        }
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

Texture* GameInf::getFont(unsigned int code) {
    for (int i = 0; i < MAX_FNT; ++i) {
        if (fonts[i].id == code)
            return &fonts[i];
    }
    for (int i = 0; i < MAX_FNT_TMP; ++i) {
        if (fontsTmp[i].id == code)
            return &fontsTmp[i];
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

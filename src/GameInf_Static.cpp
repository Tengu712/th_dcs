#include "../include/HeaderApp.hpp"

#include <stdio.h>

GameInf::~GameInf() {
    if (texs != nullptr)
        delete texs;
    if (fonts != nullptr)
        delete fonts;
    if (fontsTmp != nullptr)
        delete fontsTmp;
    if (queBG != nullptr)
        delete queBG;
    if (queUI != nullptr)
        delete queUI;
    if (queFont != nullptr)
        delete queFont;
    if (bulsE != nullptr)
        delete bulsE;
    if (bulsP != nullptr)
        delete bulsP;
    if (texidsLog != nullptr)
        delete texidsLog;
    if (enemies != nullptr)
        delete enemies;
}

void GameInf::clearFontTmp() {
    if (fontsTmp != nullptr)
        delete fontsTmp;
    fontsTmp = new Texture[MAX_FNT_TMP];
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

bool GameInf::getKey(KEY_CODE code, KEY_STA status) {
    if (status == KEY_STA::Neutral)
        return imanager.getKey(static_cast<char>(code) + 1) == 0;
    else if (status == KEY_STA::Down)
        return (imanager.getKey(static_cast<char>(code) + 1) & 0b010) != 0;
    else if (status == KEY_STA::Pressed)
        return (imanager.getKey(static_cast<char>(code) + 1) & 0b001) != 0;
    else if (status == KEY_STA::Up)
        return (imanager.getKey(static_cast<char>(code) + 1) & 0b100) != 0;
    else
        return false;
}

bool GameInf::saveData() {
    FILE* pFile = fopen("./savedata.dat", "w");
    if (!pFile) {
        MessageBoxA(nullptr, "Failed to open savedata.dat.", "Error", MB_OK);
        return false;
    }

    fclose(pFile);
    return true;
}
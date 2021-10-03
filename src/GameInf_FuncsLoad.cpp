#include "../include/HeaderApp.hpp"

bool GameInf::loadTexture(HMODULE hModule, unsigned int id) {
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

bool GameInf::loadFont(unsigned int code) {
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

bool GameInf::loadFontTmp(unsigned int code) {
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

bool GameInf::loadSentence(LPCSTR str) {
    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (IsDBCSLeadByte(str[i])) {
            if (!loadFontTmp((unsigned char)str[i] << 8 | (unsigned char)str[i + 1]))
                return false;
            i++;
        } else {
            if (!loadFontTmp((unsigned int)str[i]))
                return false;
        }
    }
    return true;
}

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

Texture* GameInf::getFont(unsigned int code) {
    for (int i = 0; i < MAX_FNT; ++i) {
        if (fonts[i].id == code)
            return &fonts[i];
    }
    return nullptr;
}

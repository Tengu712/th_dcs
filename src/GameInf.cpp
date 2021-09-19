#include "../include/HeaderApp.hpp"

GameInf::GameInf() :
    sceCur(0),
    sceNex(0),
    texs {GameTexture()}
{
    for (int i = 0; i < MAX_TEXTURE; ++i) {
        texs[i] = GameTexture();
        fonts[i] = GameTexture();
    }
}

bool GameInf::addTexture(D3DManager* pManager, unsigned int id) {
    for (int i = 0; i < MAX_TEXTURE; ++i) {
        if (texs[i].id != 0)
            continue;
        if (!pManager->createTexture(id, &texs[i]))
            return false;
        texs[i].id = id;
        break;
    }
    return true;
}

bool GameInf::addFont(D3DManager* pManager, unsigned int code) {
    for (int i = 0; i < MAX_TEXTURE; ++i) {
        if (fonts[i].id != 0)
            continue;
        if (!pManager->createFont(code, &fonts[i]))
            return false;
        fonts[i].id = code;
        break;
    }
    return true;
}

Texture* GameInf::getTexture(unsigned int id) {
    for (int i = 0; i < MAX_TEXTURE; ++i) {
        if (texs[i].id == id)
            return &texs[i];
    }
    return nullptr;
}

Texture* GameInf::getFont(unsigned int code) {
    for (int i = 0; i < MAX_TEXTURE; ++i) {
        if (fonts[i].id == code)
            return &fonts[i];
    }
    return nullptr;
}


#include "../include/HeaderApp.h"

char LoadAddImage(struct GameInf* pGinf, struct D3DInf* pDinf, HMODULE hModule, unsigned int id) {
    for (int i = 0; i < MAX_IMG; ++i) {
        if (pGinf->imgs[i].id != 0)
            continue;
        if (!CreateImage(pDinf, &pGinf->imgs[i], hModule, id))
            return 0;
        pGinf->imgs[i].id = id;
        break;
    }
    return 1;
}

char LoadAddFont(struct GameInf* pGinf, struct D3DInf* pDinf, unsigned int code) {
    for (int i = 0; i < MAX_IMG; ++i) {
        if (pGinf->imgs[i].id != 0)
            continue;
        if (!CreateFontImage(pDinf, &pGinf->imgs[i], code))
            return 0;
        pGinf->imgs[i].id = ToFontID(code);
        break;
    }
    return 1;
}

char LoadAddFontTmp(struct GameInf* pGinf, struct D3DInf* pDinf, unsigned int code) {
    for (int i = 0; i < MAX_IMG_TMP; ++i) {
        if (pGinf->imgsTmp[i].id != 0)
            continue;
        if (!CreateFontImage(pDinf, &pGinf->imgsTmp[i], code))
            return 0;
        pGinf->imgsTmp[i].id = ToFontID(code);
        break;
    }
    return 1;
}

char LoadSentence(struct GameInf* pGinf, struct D3DInf* pDinf, LPCSTR str) {
    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (IsDBCSLeadByte(str[i])) {
            if (!LoadAddFontTmp(pGinf, pDinf, (unsigned char)str[i] << 8 | (unsigned char)str[i + 1]))
                return 0;
            i++;
        } else {
            if (!LoadAddFontTmp(pGinf, pDinf, (unsigned int)str[i]))
                return 0;
        }
    }
    return 1;
}

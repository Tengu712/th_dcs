#include "../include/HeaderApp.h"

void ApplyLogue(struct GameInf* pGinf, char flg, char right, unsigned int imgidL, unsigned int imgidR, LPCSTR str) {
    struct Logue tmp = { flg, right, imgidL, imgidR };
    pGinf->log = tmp;

    int cntBuf = 0;
    const int len = strlen(str);
    for (int i = 0; i < MAX_LOGUE; ++i) {
        if (cntBuf >= len) {
            pGinf->imgidsLog[i] = 0;
            continue;
        }
        unsigned int res = 0;
        if (IsDBCSLeadByte(str[cntBuf])) {
            res = ToFontID((unsigned char)str[cntBuf] << 8 | (unsigned char)str[cntBuf + 1]);
            cntBuf++;
        }
        else
            res = ToFontID((unsigned int)str[cntBuf]);
        pGinf->imgidsLog[i] = res;
        cntBuf++;
    }
}

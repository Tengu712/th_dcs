#include "../include/HeaderApp.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void ApplyFact(struct GameInf* pGinf, struct Fact* pFact) {
    pGinf->idea.posX = pFact->posX;
    pGinf->idea.posY = pFact->posY;
    pGinf->idea.posZ = pFact->posZ;
    pGinf->idea.radX = pFact->degX / 180.0f * M_PI;
    pGinf->idea.radY = pFact->degY / 180.0f * M_PI;
    pGinf->idea.radZ = pFact->degZ / 180.0f * M_PI;
    pGinf->idea.sclX = pFact->sclX;
    pGinf->idea.sclY = pFact->sclY;
    pGinf->idea.sclZ = pFact->sclZ;
    pGinf->idea.colR = pFact->colR;
    pGinf->idea.colG = pFact->colG;
    pGinf->idea.colB = pFact->colB;
    pGinf->idea.colA = pFact->colA;
}

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

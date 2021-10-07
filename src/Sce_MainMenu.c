#include "../include/HeaderScene.h"

char InitMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf) {
    pGinf->cntAll = 0U;
    pGinf->cntSce = 0U;
    pGinf->mode = 0U;
    ClearFontTmp(pGinf);
    char res = 1;
    res = res && LoadSentence(pGinf, pDinf, "ASSEMBLESAVEQUIT:imeotalccurnd");
    res = res && LoadSentence(pGinf, pDinf, "博麗神社");
    return res;
}

inline void DrawDestination(struct GameInf* pGinf, struct D3DInf* pDinf, LPCSTR str, float y, char hob) {
    struct Fact fact;
    CreateFact(&fact);
    fact.posX = -140.0f;
    fact.posY = y;
    fact.sclX = 7.0f;
    fact.sclY = 0.55f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_BOX_DEST));
    if (hob) {
        fact.colA = 0.4f * (float)fabs(sin((double)pGinf->cntAll / 90.0 * M_PI));
        DrawImage(pGinf, pDinf, &fact, NULL);
    }
    fact.posX = -450.0f;
    fact.sclX = 0.22f;
    fact.sclY = 0.25f;
    fact.colA = 1.0f;
    const int kLen = strlen(str);
    for (int i = 0; i < kLen; ++i) {
        unsigned int res = 0;
        if (IsDBCSLeadByte(str[i])) {
            res = ToFontID((unsigned char)str[i] << 8 | (unsigned char)str[i + 1]);
            i++;
        } else
            res = ToFontID((unsigned int)str[i]);
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, res));
        fact.posX += 22.0f;
    }
}

inline void DrawSubMenu(struct GameInf* pGinf, struct D3DInf* pDinf, LPCSTR str, float y, char hob) {
    struct Fact fact;
    CreateFact(&fact);
    fact.posX = 360.0f;
    fact.posY = y;
    fact.sclX = 2.6f;
    fact.sclY = 0.55f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_BOX));
    if (hob) {
        fact.colA = 0.4f * (float)fabs(sin((double)pGinf->cntAll / 90.0 * M_PI));
        DrawImage(pGinf, pDinf, &fact, NULL);
    }
    const int kLen = strlen(str);
    fact.posX = 370.0f - (float)kLen * 11.0f;
    fact.sclX = 0.22f;
    fact.sclY = 0.25f;
    fact.colA = 1.0f;
    for (int i = 0; i < kLen; ++i) {
        unsigned int res = 0;
        if (IsDBCSLeadByte(str[i])) {
            res = ToFontID((unsigned char)str[i] << 8 | (unsigned char)str[i + 1]);
            i++;
        } else
            res = ToFontID((unsigned int)str[i]);
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, res));
        fact.posX += 22.0f;
    }
}

void UpdateMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {

    if (GetKey(pIinf, KEY_CODE_Right) & KEY_STA_Down) {
        pGinf->mode = 1U;
        pGinf->cntSce = 0;
    }

    if (GetKey(pIinf, KEY_CODE_Left) & KEY_STA_Down) {
        pGinf->mode = 0U;
        pGinf->cntSce = 0;
    }

    if (GetKey(pIinf, KEY_CODE_Up) & KEY_STA_Down) {
        pGinf->cntSce--;
    }

    if (GetKey(pIinf, KEY_CODE_Down) & KEY_STA_Down) {
        pGinf->cntSce++;
    }

    if (GetKey(pIinf, KEY_CODE_Z) & KEY_STA_Down) {
        if (pGinf->mode == 1U && pGinf->cntSce % 3 == 2)
            pGinf->sceNex = SCE_Exit;
    }
    
    pGinf->cntAll++;

    struct Fact fact;
    
    DrawBegin(pDinf, NULL, FALSE);

    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);

    // Back ground
    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_BG_MAINMENU));

    // Back ground pattern
    CreateFact(&fact);
    fact.posY = -1000.0f;
    fact.colA = 0.08f * (float)fabs(sin((double)pGinf->cntAll / 360.0 * M_PI));
    ApplyImage(pDinf, GetImage(pGinf, IMG_BG_MAINMENU_OVER));
    for (int i = 0; i < 20; ++i) {
        fact.posX = -1200.0f - (float)(pGinf->cntAll % 200);
        for (int j = 0; j < 24; ++j) {
            ApplyFact(pGinf, &fact);
            DrawModel(pDinf, &pGinf->idea);
            fact.posX += 100.0f;
        }
        fact.posY += 100.0f;
    }

    // Main Menu
    CreateFact(&fact);
    fact.posX = -400.0f;
    fact.posY = 410.0f;
    fact.sclX = 4.4f;
    fact.sclY = 1.8f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_MAINMENU));

    // Detail
    CreateFact(&fact);
    fact.posY = -220.0f;
    fact.sclX = 9.9f;
    fact.sclY = 3.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_UI_BOX_DETAIL));

    // Total
    fact.posX = 260.0f;
    fact.posY = -110.0f;
    fact.sclX = 0.16f;
    fact.sclY = 0.2f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("T"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("o"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("t"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("a"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("l"))));
    fact.posX = 456.0f;
    fact.posY -= 28.0f;
    unsigned long long shi = 1LL;
    for (int i = 0; i < 12; ++i) {
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.scoreTotal / shi) % 10) + 48U)));
        fact.posX -= 16.0f;
        shi *= 10LL;
    }

    // Accum
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("A"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("c"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("c"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("u"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("m"))));
    fact.posX = 456.0f;
    fact.posY -= 28.0f;
    shi = 1LL;
    for (int i = 0; i < 12; ++i) {
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.scoreTotal / shi) % 10) + 48U)));
        fact.posX -= 16.0f;
        shi *= 10LL;
    }

    // Time
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("T"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("i"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("m"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("e"))));
    fact.posX = 280.0f;
    fact.posY -= 28.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 2160000) % 6) + 48U)));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 216000) % 6) + 48U)));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint(":"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 36000) % 6) + 48U)));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 3600) % 6) + 48U)));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint(":"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 600) % 6) + 48U)));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntPlay / 60) % 6) + 48U)));
    
    // Around
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("A"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("r"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("o"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("u"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("n"))));
    fact.posX += 16.0f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(Lpcstr2uint("d"))));
    fact.posX = 280.0f;
    fact.posY -= 28.0f;
    if (pGinf->data.cntWorldRound > 9) {
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID(((pGinf->data.cntWorldRound / 10) % 10) + 48U)));
        fact.posX += 16.0f;
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID((pGinf->data.cntWorldRound % 10) + 48U)));
    } else
        DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, ToFontID((pGinf->data.cntWorldRound % 10) + 48U)));


    //
    DrawDestination(pGinf, pDinf, "博麗神社", 300.0f, pGinf->mode == 0 && pGinf->cntSce % 1 == 0);

    //
    DrawSubMenu(pGinf, pDinf, "ASSEMBLE", 300.0f, pGinf->mode == 1 && pGinf->cntSce % 3 == 0);
    DrawSubMenu(pGinf, pDinf, "SAVE", 230.0f, pGinf->mode == 1 && pGinf->cntSce % 3 == 1);
    DrawSubMenu(pGinf, pDinf, "QUIT", 160.0f, pGinf->mode == 1 && pGinf->cntSce % 3 == 2);


    DrawEnd(pDinf);

}

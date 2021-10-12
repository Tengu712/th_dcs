#include "../include/HeaderScene.h"

void DrawDestOrSub(struct GameInf* pGinf, struct D3DInf* pDinf, char isDest, LPCSTR str, float y, char hob) {
    struct Fact fact;
    CreateFact(&fact);
    fact.posX = isDest ? -140.0f : 360.0f;
    fact.posY = y;
    fact.sclX = isDest ? 7.0f : 2.6f;
    fact.sclY = 0.55f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, isDest ? IMG_UI_BOX_DEST : IMG_UI_BOX));
    if (hob) {
        fact.colA = 0.4f * (float)fabs(sin((double)pGinf->cntSce[0] / 90.0 * M_PI));
        DrawImage(pGinf, pDinf, &fact, NULL);
        fact.colA = 1.0f;
    }
    const int kLen = strlen(str);
    fact.posX = isDest ? -450.0f : 370.0f - (float)kLen * 11.0f;
    fact.sclX = 0.22f;
    fact.sclY = 0.25f;
    DrawString(pGinf, pDinf, &fact, str, 22.0f);
}

char InitMainMenu(struct Infs* pinfs) {
    memset(pinfs->pGinf->cntSce, 0, sizeof(int) * MAX_PARAM);
    ClearFontTmp(pinfs->pGinf);
    char res = 1;
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "ASSEMBLESAVEQUIT:imeotalccurnd");
    res = res && LoadSentence(pinfs->pGinf, pinfs->pDinf, "博麗神社");
    return res;
}

void UpdateMainMenu(struct Infs* pinfs) {

    if (GetKey(pinfs->pIinf, KEY_CODE_Right) & KEY_STA_Down) {
        pinfs->pGinf->cntSce[1] = 1U;
        pinfs->pGinf->cntSce[2] = 0;
    }

    if (GetKey(pinfs->pIinf, KEY_CODE_Left) & KEY_STA_Down) {
        pinfs->pGinf->cntSce[1] = 0U;
        pinfs->pGinf->cntSce[2] = 0;
    }

    if (GetKey(pinfs->pIinf, KEY_CODE_Up) & KEY_STA_Down) {
        pinfs->pGinf->cntSce[2]--;
    }

    if (GetKey(pinfs->pIinf, KEY_CODE_Down) & KEY_STA_Down) {
        pinfs->pGinf->cntSce[2]++;
    }

    if (GetKey(pinfs->pIinf, KEY_CODE_Z) & KEY_STA_Down) {
        if (pinfs->pGinf->cntSce[1] == 1U && pinfs->pGinf->cntSce[2] % 3 == 2)
            pinfs->pGinf->sceNex = SCE_Exit;
    }
    
    pinfs->pGinf->cntSce[0]++;



    struct Fact fact;
    
    DrawBegin(pinfs->pDinf, NULL, FALSE);

    ApplyCamera(pinfs->pDinf, &pinfs->pGinf->cameraUI, FALSE);

    // Back ground
    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_BG_MAINMENU));

    // Back ground pattern
    CreateFact(&fact);
    fact.posY = -1000.0f;
    fact.colA = 0.08f * (float)fabs(sin((double)pinfs->pGinf->cntSce[0] / 360.0 * M_PI));
    ApplyImage(pinfs->pDinf, GetImage(pinfs->pGinf, IMG_BG_MAINMENU_OVER));
    for (int i = 0; i < 20; ++i) {
        fact.posX = -1200.0f - (float)(pinfs->pGinf->cntSce[0] % 200);
        for (int j = 0; j < 24; ++j) {
            ApplyFact(pinfs->pGinf, &fact);
            DrawModel(pinfs->pDinf, &pinfs->pGinf->idea);
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
    DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_UI_MAINMENU));

    // Detail
    CreateFact(&fact);
    fact.posY = -220.0f;
    fact.sclX = 9.9f;
    fact.sclY = 3.0f;
    DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, IMG_UI_BOX_DETAIL));

    // Total
    fact.posX = 260.0f;
    fact.posY = -110.0f;
    fact.sclX = 0.16f;
    fact.sclY = 0.2f;
    DrawString(pinfs->pGinf, pinfs->pDinf, &fact, "Total", 16.0f);
    fact.posX = 456.0f;
    fact.posY -= 28.0f;
    unsigned long long shi = 1LL;
    for (int i = 0; i < 12; ++i) {
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, 
            GetImage(pinfs->pGinf, ToFontID(((pinfs->pGinf->data.scoreTotal / shi) % 10) + 48U)));
        fact.posX -= 16.0f;
        shi *= 10LL;
    }

    // Accum
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawString(pinfs->pGinf, pinfs->pDinf, &fact, "Accum", 16.0f);
    fact.posX = 456.0f;
    fact.posY -= 28.0f;
    shi = 1LL;
    for (int i = 0; i < 12; ++i) {
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact,
            GetImage(pinfs->pGinf, ToFontID(((pinfs->pGinf->data.scoreTotalGot / shi) % 10) + 48U)));
        fact.posX -= 16.0f;
        shi *= 10LL;
    }

    // Time
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawString(pinfs->pGinf, pinfs->pDinf, &fact, "Time", 16.0f);
    fact.posX = 280.0f;
    fact.posY -= 28.0f;
    unsigned int imgidsTime[8] = {
        ((pinfs->pGinf->data.cntPlay / 2160000) % 6) + 48U,
        ((pinfs->pGinf->data.cntPlay / 216000) % 10) + 48U,
        Lpcstr2uint(":"),
        ((pinfs->pGinf->data.cntPlay / 36000) % 6) + 48U,
        ((pinfs->pGinf->data.cntPlay / 3600) % 10) + 48U,
        Lpcstr2uint(":"),
        ((pinfs->pGinf->data.cntPlay / 600) % 6) + 48U,
        ((pinfs->pGinf->data.cntPlay / 60) % 10) + 48U
    };
    for (int i = 0; i < 8; ++i) {
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact, GetImage(pinfs->pGinf, ToFontID(imgidsTime[i])));
        fact.posX += 16.0f;
    }
    
    // Around
    fact.posX = 260.0f;
    fact.posY -= 35.0f;
    DrawString(pinfs->pGinf, pinfs->pDinf, &fact, "Around", 16.0f);
    fact.posX = 280.0f;
    fact.posY -= 28.0f;
    if (pinfs->pGinf->data.cntWorldRound > 9) {
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact,
            GetImage(pinfs->pGinf, ToFontID(((pinfs->pGinf->data.cntWorldRound / 10) % 10) + 48U)));
        fact.posX += 16.0f;
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact,
            GetImage(pinfs->pGinf, ToFontID((pinfs->pGinf->data.cntWorldRound % 10) + 48U)));
    } else
        DrawImage(pinfs->pGinf, pinfs->pDinf, &fact,
            GetImage(pinfs->pGinf, ToFontID((pinfs->pGinf->data.cntWorldRound % 10) + 48U)));

    //!
    DrawDestOrSub(pinfs->pGinf, pinfs->pDinf, 1, "博麗神社", 300.0f, pinfs->pGinf->cntSce[1] == 0 && pinfs->pGinf->cntSce[2] % 1 == 0);
    
    // Submenu
    DrawDestOrSub(pinfs->pGinf, pinfs->pDinf, 0, "ASSEMBLE", 300.0f, pinfs->pGinf->cntSce[1] == 1 && pinfs->pGinf->cntSce[2] % 3 == 0);
    DrawDestOrSub(pinfs->pGinf, pinfs->pDinf, 0, "SAVE", 230.0f, pinfs->pGinf->cntSce[1] == 1 && pinfs->pGinf->cntSce[2] % 3 == 1);
    DrawDestOrSub(pinfs->pGinf, pinfs->pDinf, 0, "QUIT", 160.0f, pinfs->pGinf->cntSce[1] == 1 && pinfs->pGinf->cntSce[2] % 3 == 2);

    DrawEnd(pinfs->pDinf);

}

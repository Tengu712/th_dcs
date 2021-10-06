#include "../include/HeaderScene.h"

void InitMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf) {
    pGinf->cntAll = 0U;
    pGinf->cntSce = 0U;
    pGinf->mode = 0U;
}

void UpdateMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf) {
    
    pGinf->cntAll++;

    struct Fact fact;
    
    DrawBegin(pDinf, NULL, FALSE);

    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);

    CreateFact(&fact);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    DrawImage(pGinf, pDinf, &fact, GetImage(pGinf, IMG_BG_MAINMENU));

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

    DrawEnd(pDinf);

}
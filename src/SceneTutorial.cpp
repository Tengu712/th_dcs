#include "../include/HeaderApp.hpp"

void SceneTutorial::init(GameInf* pGinf) {
    gameInit(pGinf);

    pGinf->addSentence("なんていうか今回は");

    bg0.posY = -300.0f;
    bg0.sclX = 4.5f;
    bg0.sclY = 4.5f;
    bg0.degX = 90.0f;
    bg0.texid = TEX_BG_KEIDAI;

    bg1.posY = -300.0f;
    bg1.posZ = 450.0f;
    bg1.sclX = 4.5f;
    bg1.sclY = 4.5f;
    bg1.degX = 90.0f;
    bg1.texid = TEX_BG_KEIDAI;

    pGinf->cameraBG.dirX = 0.0f;
    pGinf->cameraBG.dirY = -1.0f / sqrt(2.0f);
    pGinf->cameraBG.dirZ = 1.0f / sqrt(2.0f);

}

void SceneTutorial::update(GameInf* pGinf) {
    pGinf->cameraBG.posZ += 1.0f;
    pGinf->applyLogue(true, false, TEX_TC_MARISA0, 0, "なんていうか今回は");
    pGinf->pushBG(&bg0);
    pGinf->pushBG(&bg1);
    gameUpdate(pGinf);
}

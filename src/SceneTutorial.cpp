#include "../include/HeaderScene.hpp"

void SceneTutorial::init(GameInf* pGinf) {
    gameInit(pGinf);

    pGinf->loadSentence("なんていうか今回は");

    bg.posY = -300.0f;
    bg.sclX = 4.5f;
    bg.sclY = 4.5f;
    bg.degX = 90.0f;
    bg.texid = TEX_BG_KEIDAI;

    pGinf->cameraBG.dirX = 0.0f;
    pGinf->cameraBG.dirY = -1.0f / sqrt(2.0f);
    pGinf->cameraBG.dirZ = 1.0f / sqrt(2.0f);

}

void SceneTutorial::update(GameInf* pGinf) {
    pGinf->cameraBG.posZ += 1.0f;
    pGinf->applyLogue(true, false, TEX_TC_MARISA0, 0, "なんていうか今回は");
    pGinf->pushBG(&bg);
    gameUpdate(pGinf);
}

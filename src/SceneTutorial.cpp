#include "../include/HeaderApp.hpp"

void SceneTutorial::init(GameInf* pGinf) {
    gameInit(pGinf);
    bg.posZ = 500.0f;
    bg.sclX = 100.0f;
    bg.sclY = 100.0f;
    bg.texid = TEX_BG_TUTORIAL;
}

void SceneTutorial::update(GameInf* pGinf) {
    pGinf->pushBG(&bg);
    gameUpdate(pGinf);
}

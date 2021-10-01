#include "../include/HeaderApp.hpp"

void SceneTutorial::init(GameInf* pGinf) {
    gameInit(pGinf);
    bg.posZ = 500.0f;
    bg.sclX = 100.0f;
    bg.sclY = 100.0f;
    bg.texid = TEX_BG_TUTORIAL;
}

void SceneTutorial::update(GameInf* pGinf) {
    if (cnt % 100 == 0) {
        Enemy e = Enemy();
        pGinf->createEnemy(&e, TEX_CH_FAIRY_R0);
        e.deg = 270;
        e.spd = 200;
        pGinf->pushEnemy(&e);
    }
    pGinf->pushBG(&bg);
    gameUpdate(pGinf);
}

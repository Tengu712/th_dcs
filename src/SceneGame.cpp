#include "../include/HeaderApp.hpp"

void ASceneGame::gameInit(GameInf* pGinf) {
    frame.texid = TEX_UI_FRAME;
    frame.posZ = 1.2f;
    frame.sclX = 12.8f;
    frame.sclY = 9.6f;
}

void ASceneGame::gameUpdate(GameInf* pGinf) {
    pGinf->updatePlayer();
    pGinf->updateEnemies();
    pGinf->updateBullets();
    pGinf->pushUI(&frame);
    cnt++;
}

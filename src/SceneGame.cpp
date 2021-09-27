#include "../include/HeaderApp.hpp"

ASceneGame::ASceneGame() : frame(Fact()) {}

void ASceneGame::gameInit(GameInf* pGinf) {
    frame.texid = TEX_UI_FRAME;
    frame.sclX = 12.8f;
    frame.sclY = 9.6f;
}

void ASceneGame::gameUpdate(GameInf* pGinf) {
    pGinf->pushUI(&frame);
}
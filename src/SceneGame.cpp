#include "../include/HeaderScene.hpp"

void ASceneGame::gameInit(GameInf* pGinf) {}

void ASceneGame::gameUpdate(GameInf* pGinf) {
    pGinf->updatePlayer();
    pGinf->updateEnemies();
    pGinf->updateBullets();
    pGinf->drawGame();
    cnt++;
}

#include "../include/HeaderScene.hpp"

void SceneTitle::init(GameInf* pGinf) {
}

void SceneTitle::update(GameInf* pGinf) {
    pGinf->sceNex = SCE_ID::Tutorial;
}

#include "../include/HeaderApp.hpp"

SceneTitle::SceneTitle() : bg(Fact()) {}

void SceneTitle::init(GameInf* pGinf) {
}

void SceneTitle::update(GameInf* pGinf) {
    pGinf->sceNex = SCE_ID::Tutorial;
}

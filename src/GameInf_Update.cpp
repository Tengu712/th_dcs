#include "../include/HeaderApp.hpp"

#include <time.h>
#pragma comment(lib, "Winmm.lib")

void GameInf::update() {
    queUI.clear();
    queFont.clear();

    startTime = timeGetTime();
    if (startTime - lastTime >= 1000L){
        fps = (float)(cntFps * 1000) / (float)(startTime - lastTime);
        lastTime = startTime;
        cntFps = 0;
    }
    const unsigned int tmpfps = (unsigned int)(fps * 10.0f);
    modelsFps[0].texid = ((tmpfps / 100) % 10) + 48U;
    modelsFps[1].texid = ((tmpfps / 10) % 10) + 48U;
    modelsFps[3].texid = (tmpfps % 10) + 48U;
   for (int i = 0; i < 7; ++i) {
        queFont.push(&modelsFps[i]);
    }
    cntFps++;

    imanager.inspect();
}

void GameInf::draw() {
    dmanager.drawBegin();

    // UI
    dmanager.applyCamera(&cameraUI, false);
    for(int i = 0; i < 10000; ++i) {
        Model* pTmp = queUI.pop();
        if (pTmp == nullptr)
            break;
        dmanager.applyTexture(getTexture(pTmp->texid));
        dmanager.drawModel(pTmp);
    }

    // Font
     for(int i = 0; i < 10000; ++i) {
        Model* pTmp = queFont.pop();
        if (pTmp == nullptr)
            break;
        dmanager.applyTexture(getFont(pTmp->texid));
        dmanager.drawModel(pTmp);
    }

    dmanager.drawEnd();
}


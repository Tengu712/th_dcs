#include "../include/HeaderApp.hpp"

#include <time.h>
#pragma comment(lib, "Winmm.lib")

void GameInf::update() {
    startTime = timeGetTime();
    if (startTime - lastTime >= 1000L){
        fps = (float)(cntFps * 1000) / (float)(startTime - lastTime);
        lastTime = startTime;
        cntFps = 0;
    }
    cntFps++;

    for (int i = 0; i < MAX_QUE_BG; ++i) {
        queBG[i] = nullptr;
    }
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        queUI[i] = nullptr;
    }
    for (int i = 0; i < MAX_QUE_FNT; ++i) {
        queFont[i] = nullptr;
    }

    imanager.inspect();

    data.cntPlay++;
}

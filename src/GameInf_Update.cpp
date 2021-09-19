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
    imanager.inspect();
}

void GameInf::draw() {
    dmanager.drawBegin();

    // Fps
    const unsigned int tmpfps = (unsigned int)(fps * 10.0f);
    dmanager.applyTexture(getFont(((tmpfps / 100) % 10) + 48U));
    dmanager.drawModel(&modelsFps[0]);
    dmanager.applyTexture(getFont(((tmpfps / 10) % 10) + 48U));
    dmanager.drawModel(&modelsFps[1]);
    dmanager.applyTexture(getFont((tmpfps % 10) + 48U));
    dmanager.drawModel(&modelsFps[3]);
    dmanager.applyTexture(getFont(Lpcstr2uint(".")));
    dmanager.drawModel(&modelsFps[2]);
    dmanager.applyTexture(getFont(Lpcstr2uint("f")));
    dmanager.drawModel(&modelsFps[4]);
    dmanager.applyTexture(getFont(Lpcstr2uint("p")));
    dmanager.drawModel(&modelsFps[5]);
    dmanager.applyTexture(getFont(Lpcstr2uint("s")));
    dmanager.drawModel(&modelsFps[6]);

    dmanager.drawEnd();
}

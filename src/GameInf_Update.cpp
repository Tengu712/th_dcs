#include "../include/HeaderApp.hpp"

#include <time.h>
#pragma comment(lib, "Winmm.lib")

void GameInf::update() {
    for (int i = 0; i < MAX_QUE_UI; ++i) {
        queUI[i] = nullptr;
    }
    for (int i = 0; i < MAX_QUE_FNT; ++i) {
        queFont[i] = nullptr;
    }

    startTime = timeGetTime();
    if (startTime - lastTime >= 1000L){
        fps = (float)(cntFps * 1000) / (float)(startTime - lastTime);
        lastTime = startTime;
        cntFps = 0;
    }
    cntFps++;

    imanager.inspect();
}

void GameInf::applyFact(Fact* pFact) {
    ideaSquare.posX = pFact->posX;
    ideaSquare.posY = pFact->posY;
    ideaSquare.posZ = pFact->posZ;
    ideaSquare.degX = pFact->degX;
    ideaSquare.degY = pFact->degY;
    ideaSquare.degZ = pFact->degZ;
    ideaSquare.sclX = pFact->sclX;
    ideaSquare.sclY = pFact->sclY;
    ideaSquare.sclZ = pFact->sclZ;
    ideaSquare.colR = pFact->colR;
    ideaSquare.colG = pFact->colG;
    ideaSquare.colB = pFact->colB;
    ideaSquare.colA = pFact->colA;
}

void GameInf::draw() {
    dmanager.drawBegin();

    dmanager.applyCamera(&cameraGame, false);

    // Player
    applyFact(&player.fact);
    dmanager.applyTexture(getTexture(player.fact.texid));
    dmanager.drawModel(&ideaSquare);

    // UI
    dmanager.applyCamera(&cameraUI, false);
    for(int i = 0; i < MAX_QUE_UI; ++i) {
        if (queUI[i] == nullptr)
            continue;
        applyFact(queUI[i]);
        dmanager.applyTexture(getTexture(queUI[i]->texid));
        dmanager.drawModel(&ideaSquare);
    }

    // Font
    for(int i = 0; i < MAX_QUE_FNT; ++i) {
        if (queFont[i] == nullptr)
            continue;
        applyFact(queFont[i]);
        dmanager.applyTexture(getFont(queFont[i]->texid));
        dmanager.drawModel(&ideaSquare);
    }

    // Fps
    const unsigned int tmpfps = (unsigned int)(fps * 10.0f);
    for (int i = 0; i < 7; ++i) {
        Fact fact = Fact();
        if (i == 0) 
            fact.texid = ((tmpfps / 100) % 10) + 48U;
        else if (i == 1)
            fact.texid = ((tmpfps / 10) % 10) + 48U;
        else if (i == 3)
            fact.texid = (tmpfps % 10) + 48U;
        else if (i == 2)
            fact.texid = Lpcstr2uint(".");
        else if (i == 4)
            fact.texid = Lpcstr2uint("f");
        else if (i == 5)
            fact.texid = Lpcstr2uint("p");
        else 
            fact.texid = Lpcstr2uint("s");
        fact.posX = 15.0f * (float)i + 532.0f;
        fact.posY = -457.0f;
        fact.posZ = 1.1f;
        fact.sclX = 0.15f;
        fact.sclY = 0.2f;
        applyFact(&fact);
        dmanager.applyTexture(getFont(fact.texid));
        dmanager.drawModel(&ideaSquare);
    }

    dmanager.drawEnd();
}


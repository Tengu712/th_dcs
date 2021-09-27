#include "../include/HeaderApp.hpp"

void GameInf::update() {
    queUI.clear();
    queFont.clear();
    fpsCalculator.update();
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

    // UI
    dmanager.applyCamera(&cameraUI, false);
    for(int i = 0; i < 10000; ++i) {
        Fact* pTmp = queUI.pop();
        if (pTmp == nullptr)
            break;
        applyFact(pTmp);
        dmanager.applyTexture(getTexture(pTmp->texid));
        dmanager.drawModel(&ideaSquare);
    }

    // Fps
    const unsigned int tmpfps = (unsigned int)(fpsCalculator.fps * 10.0f);
    Fact fact[7];
    for (int i = 0; i < 7; ++i) {
        unsigned int texid = 0U;
        if (i == 0) 
            texid = ((tmpfps / 100) % 10) + 48U;
        else if (i == 1)
            texid = ((tmpfps / 10) % 10) + 48U;
        else if (i == 3)
            texid = (tmpfps % 10) + 48U;
        else if (i == 2)
            texid = Lpcstr2uint(".");
        else if (i == 4)
            texid = Lpcstr2uint("f");
        else if (i == 5)
            texid = Lpcstr2uint("p");
        else 
            texid = Lpcstr2uint("s");
        fact[i].texid = texid;
        fact[i].posX = 15.0f * (float)i + 532.0f;
        fact[i].posY = -457.0f;
        fact[i].posZ = -1000.0f;
        fact[i].sclX = 0.15f;
        fact[i].sclY = 0.2f;
        queFont.push(&fact[i]);
    }

    // Font
    for(int i = 0; i < 10000; ++i) {
        Fact* pTmp = queFont.pop();
        if (pTmp == nullptr)
            break;
        applyFact(pTmp);
        dmanager.applyTexture(getFont(pTmp->texid));
        dmanager.drawModel(&ideaSquare);
    }

    dmanager.drawEnd();
}


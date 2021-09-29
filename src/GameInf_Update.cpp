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
}

void GameInf::applyFact(Fact* pFact) {
    idea.posX = pFact->posX;
    idea.posY = pFact->posY;
    idea.posZ = pFact->posZ;
    idea.degX = pFact->degX;
    idea.degY = pFact->degY;
    idea.degZ = pFact->degZ;
    idea.sclX = pFact->sclX;
    idea.sclY = pFact->sclY;
    idea.sclZ = pFact->sclZ;
    idea.colR = pFact->colR;
    idea.colG = pFact->colG;
    idea.colB = pFact->colB;
    idea.colA = pFact->colA;
}

void GameInf::draw() {

    // ===== Back ground ===== //

    dmanager.drawBeginWithFrameBuffer(&fbBG, true);
    dmanager.applyCamera(&cameraBG, true);

    for (int i = 0; i < MAX_QUE_BG; ++i) {
        if (queBG[i] == nullptr)
            continue;
        applyFact(queBG[i]);
        dmanager.applyTexture(getTexture(queBG[i]->texid));
        dmanager.drawModel(&idea);
    }

    // ===== Game Object ===== //

    dmanager.drawBeginWithFrameBuffer(&fbGame, false);

    dmanager.applyCamera(&cameraUI, false);
    Fact factForFB = Fact();
    factForFB.sclX = 12.8f;
    factForFB.sclY = 9.6f;
    applyFact(&factForFB);
    dmanager.applyTexture(&fbBG.texture);
    dmanager.drawModel(&idea);

    dmanager.applyCamera(&cameraGame, false);

    // Player
    applyFact(&player.fact);
    dmanager.applyTexture(getTexture(player.fact.texid));
    dmanager.drawModel(&idea);

    // Bullet
    for (int i = 0; i < MAX_KND_BUL_P; ++i) {
        if (mapBulsP[i] == 0)
            continue;
        dmanager.applyTexture(getTexture(mapBulsP[i]));
        for (int j = 0; j < MAX_NUM_BUL_P; ++j) {
            if (!bulsP[i][j].moving)
                continue;
            applyFact(&bulsP[i][j].fact);
            dmanager.drawModel(&idea);
        }
    }
    for (int i = 0; i < MAX_KND_BUL_E; ++i) {
        if (mapBulsE[i] == 0)
            continue;
        dmanager.applyTexture(getTexture(mapBulsE[i]));
        for (int j = 0; j < MAX_NUM_BUL_E; ++j) {
            if (!bulsE[i][j].moving)
                continue;
            applyFact(&bulsE[i][j].fact);
            dmanager.drawModel(&idea);
        }
    }

    // Slow
    if (player.cntSlow > 0U) {
        Fact fact = Fact();
        fact.posX = player.fact.posX;
        fact.posY = player.fact.posY;
        // Atari
        fact.posZ = 1.3f;
        if (player.cntSlow < 10U) {
            fact.sclX = 1600.0f + 500.0f * (1.0f - (float)player.cntSlow / 10U);
            fact.sclY = 1600.0f + 500.0f * (1.0f - (float)player.cntSlow / 10U);
        } else {
            fact.sclX = 1600.0f;
            fact.sclY = 1600.0f;
            fact.degZ = (float)player.cntSlow * 4.0f;
        }
        applyFact(&fact);
        dmanager.applyTexture(getTexture(TEX_CH_ATARI));
        dmanager.drawModel(&idea);
        // Circle
         if (player.cntSlow < 10U) {
            fact.sclX = 10000.0f + 5000.0f * (1.0f - (float)player.cntSlow / 10U);
            fact.sclY = 10000.0f + 5000.0f * (1.0f - (float)player.cntSlow / 10U);
        } else {
            fact.sclX = 10000.0f;
            fact.sclY = 10000.0f;
        }
        applyFact(&fact);
        dmanager.applyTexture(getTexture(TEX_CH_SLOWCIRCLE));
        dmanager.drawModel(&idea);
        fact.degZ *= -1.0f;
        applyFact(&fact);
        dmanager.drawModel(&idea);
    }

    // ===== UI ===== //

    dmanager.drawBegin(false);

    dmanager.applyCamera(&cameraUI, false);
    applyFact(&factForFB);
    dmanager.applyTexture(&fbGame.texture);
    dmanager.drawModel(&idea);

    // UI
    for(int i = 0; i < MAX_QUE_UI; ++i) {
        if (queUI[i] == nullptr)
            continue;
        applyFact(queUI[i]);
        dmanager.applyTexture(getTexture(queUI[i]->texid));
        dmanager.drawModel(&idea);
    }

    // Font
    for(int i = 0; i < MAX_QUE_FNT; ++i) {
        if (queFont[i] == nullptr)
            continue;
        applyFact(queFont[i]);
        dmanager.applyTexture(getFont(queFont[i]->texid));
        dmanager.drawModel(&idea);
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
        fact.sclX = 0.15f;
        fact.sclY = 0.2f;
        applyFact(&fact);
        dmanager.applyTexture(getFont(fact.texid));
        dmanager.drawModel(&idea);
    }

    dmanager.drawEnd();
}


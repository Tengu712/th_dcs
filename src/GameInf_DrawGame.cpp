#include "../include/HeaderApp.hpp"

void GameInf::drawGame() {

    Fact fact = Fact();

    // Back ground : bg 0 t

    dmanager.drawBeginWithFrameBuffer(&fb0, true);
    dmanager.applyCamera(&cameraBG, true);

    for (int i = 0; i < MAX_QUE_BG; ++i) {
        if (!queBG[i].enable)
            continue;
        applyFact(&queBG[i]);
        dmanager.applyTexture(getTexture(queBG[i].texid));
        dmanager.drawModel(&idea);
    }

    // Player Enemy Bullets : game 1 f

    dmanager.drawBeginWithFrameBuffer(&fb1, false);

    dmanager.applyCamera(&cameraUI, false);
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    applyFact(&fact);
    dmanager.applyTexture(&fb0.texture);
    dmanager.drawModel(&idea);

    dmanager.applyCamera(&cameraGame, false);

    int texCur = 0;
    for (int i = 0; i < MAX_ENEMY; ++i) {
        if (!enemies[i].moving)
            continue;
        if (texCur != enemies[i].fact.texid) {
            texCur = enemies[i].fact.texid;
            dmanager.applyTexture(getTexture(texCur));
        }
        applyFact(&enemies[i].fact);
        dmanager.drawModel(&idea);        
    }

    applyFact(&player.fact);
    dmanager.applyTexture(getTexture(player.fact.texid));
    dmanager.drawModel(&idea);

    for (int i = 0; i < MAX_BUL_P; ++i) {
        if (!bulsP[i].moving)
            continue;
        if (texCur != bulsP[i].fact.texid) {
            texCur = bulsP[i].fact.texid;
            dmanager.applyTexture(getTexture(texCur));
        }
        applyFact(&bulsP[i].fact);
        dmanager.drawModel(&idea);
    }

    for (int i = 0; i < MAX_BUL_E; ++i) {
        if (!bulsE[i].moving)
            continue;
        if (texCur != bulsE[i].fact.texid) {
            texCur = bulsE[i].fact.texid;
            dmanager.applyTexture(getTexture(texCur));
        }
        applyFact(&bulsE[i].fact);
        dmanager.drawModel(&idea);
    }

    if (player.cntSlow > 0U) {
        fact = Fact();
        fact.posX = player.fact.posX;
        fact.posY = player.fact.posY;
        // Atari
        fact.sclX = (float)data.r + 300.0f;
        fact.sclY = (float)data.r + 300.0f;
        if (player.cntSlow < 10U) {
            fact.sclX += 500.0f * (1.0f - (float)player.cntSlow / 10U);
            fact.sclY += 500.0f * (1.0f - (float)player.cntSlow / 10U);
        } else 
            fact.degZ = (float)player.cntSlow * 4.0f;
        applyFact(&fact);
        dmanager.applyTexture(getTexture(TEX_CH_ATARI));
        dmanager.drawModel(&idea);
        // Circle
        fact.sclX = 13000.0f;
        fact.sclY = 13000.0f;
        if (player.cntSlow < 10U) {
            fact.sclX += 5000.0f * (1.0f - (float)player.cntSlow / 10U);
            fact.sclY += 5000.0f * (1.0f - (float)player.cntSlow / 10U);
        }
        applyFact(&fact);
        dmanager.applyTexture(getTexture(TEX_CH_SLOWCIRCLE));
        dmanager.drawModel(&idea);
        fact.degZ *= -1.0f;
        applyFact(&fact);
        dmanager.drawModel(&idea);
    }

    // UI Font Frame Logue : ui 1 f

    dmanager.applyCamera(&cameraUI, false);

    for(int i = 0; i < MAX_QUE_UI; ++i) {
        if (!queUI[i].enable)
            continue;
        applyFact(&queUI[i]);
        dmanager.applyTexture(getTexture(queUI[i].texid));
        dmanager.drawModel(&idea);
    }

    for(int i = 0; i < MAX_QUE_FNT; ++i) {
        if (!queFont[i].enable)
            continue;
        applyFact(&queFont[i]);
        dmanager.applyTexture(getFont(queFont[i].texid));
        dmanager.drawModel(&idea);
    }

    fact = Fact();
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    applyFact(&fact);
    dmanager.applyTexture(getTexture(TEX_UI_FRAME));
    dmanager.drawModel(&idea);

    if (log.flg) {
        // Tachie
        if (log.texidLeft != 0) {
            fact = Fact();
            fact.posX = -390.0f;
            fact.posY = -160.0f;
            fact.sclX = 6.5f;
            fact.sclY = 6.5f;
            applyFact(&fact);
            dmanager.applyTexture(getTexture(log.texidLeft));
            dmanager.drawModel(&idea);
        }
        // Box
        fact = Fact();
        fact.posY = -350.0f;
        fact.colR = 0.0f;
        fact.colG = 0.0f;
        fact.colB = 0.0f;
        fact.colA = 0.5f;
        fact.sclX = 6.0f;
        applyFact(&fact);
        dmanager.applyTexture(nullptr);
        dmanager.drawModel(&idea);
        // Logue
        fact = Fact();
        fact.posY = -320.0f;
        fact.posX = -260.0f;
        fact.sclX = 0.27f;
        fact.sclY = 0.27f;
        for (int i = 0; i < MAX_LOGUE; ++i) {
            if (texidsLog[i] == 0)
                break;
            fact.posX += 27.0f;
            applyFact(&fact);
            dmanager.applyTexture(getFont(texidsLog[i]));
            dmanager.drawModel(&idea);
        }
    }

    // Fps : ui - f

    dmanager.drawBegin(false);

    fact = Fact();
    fact.sclX = 12.8f;
    fact.sclY = 9.6f;
    applyFact(&fact);
    dmanager.applyTexture(&fb1.texture);
    dmanager.drawModel(&idea);

    drawFps();

    dmanager.drawEnd();
}

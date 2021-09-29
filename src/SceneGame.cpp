#include "../include/HeaderApp.hpp"

ASceneGame::ASceneGame() : frame(Fact()) {}

void ASceneGame::gameInit(GameInf* pGinf) {
    frame.texid = TEX_UI_FRAME;
    frame.posZ = 1.2f;
    frame.sclX = 12.8f;
    frame.sclY = 9.6f;
}

void ASceneGame::gameUpdate(GameInf* pGinf) {
    pGinf->player.update(pGinf);

    for (int i = 0; i < MAX_KND_BUL_SELF; ++i) {
        if (pGinf->mapBulletsSelf[i] == 0)
            continue;
        int numMoving = 0;
        for (int j = 0; j < MAX_NUM_BUL_SELF; ++j) {
            // Pass empty bullet
            if (!pGinf->bulletsSelf[i][j].moving)
                continue;
            // Update
            const int flgCur = pGinf->bulletsSelf[i][j].flgHit;
            pGinf->bulletsSelf[i][j].update(pGinf);
            // Remove
            if (pGinf->bulletsSelf[i][j].flgHit == 2
                    || pGinf->bulletsSelf[i][j].x > 5000000 || pGinf->bulletsSelf[i][j].x < -5000000 
                    || pGinf->bulletsSelf[i][j].y > 6000000 || pGinf->bulletsSelf[i][j].y < -6000000) {
                pGinf->bulletsSelf[i][j] = Bullet();
                continue;
            }
            numMoving++;
        }
        if (numMoving == 0)
            pGinf->mapBulletsSelf[i] = 0;
    }

    for (int i = 0; i < MAX_KND_BUL; ++i) {
        if (pGinf->mapBullets[i] == 0)
            continue;
        int numMoving = 0;
        for (int j = 0; j < MAX_NUM_BUL; ++j) {
            // Pass empty bullet
            if (!pGinf->bullets[i][j].moving)
                continue;
            // Update
            const int flgCur = pGinf->bullets[i][j].flgHit;
            pGinf->bullets[i][j].update(pGinf);
            // Remove
            if (pGinf->bullets[i][j].flgHit == 2 
                    || pGinf->bullets[i][j].x > 5000000 || pGinf->bullets[i][j].x < -5000000 
                    || pGinf->bullets[i][j].y > 6000000 || pGinf->bullets[i][j].y < -6000000) {
                pGinf->bullets[i][j] = Bullet();
                continue;
            }
            numMoving++;
        }
        if (numMoving == 0) 
            pGinf->mapBullets[i] = 0;
    }

    pGinf->pushUI(&frame);
}

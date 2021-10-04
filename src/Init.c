#include "../include/HeaderApp.h"

inline void InitFact(struct Fact* pFact) {
    memset(pFact, 0, sizeof(struct Fact));
    pFact->texid = 0U;
    pFact->posX = 0.0f;
    pFact->posY = 0.0f;
    pFact->posZ = 0.0f;
    pFact->degX = 0.0f;
    pFact->degY = 0.0f;
    pFact->degZ = 0.0f;
    pFact->sclX = 1.0f;
    pFact->sclY = 1.0f;
    pFact->sclZ = 1.0f;
    pFact->colR = 1.0f;
    pFact->colG = 1.0f;
    pFact->colB = 1.0f;
    pFact->colA = 1.0f;
    pFact->enable = 0;
}

inline void InitEntity(struct Entity* pEntity) {
    memset(pEntity, 0, sizeof(struct Entity));
    pEntity->cnt = 0U;
    pEntity->x = 0;
    pEntity->y = 0;
    pEntity->r = 0;
    pEntity->rGrz = 0;
    pEntity->deg = 0;
    pEntity->spd = 0;
    InitFact(&pEntity->fact);
}

void InitPlayer(struct Player* pPlayer) {
    memset(pPlayer, 0, sizeof(struct Player));
    pPlayer->cntSlow = 0U;
    InitEntity(&pPlayer->entity);
}

void InitBullet(struct Bullet* pBullet) {
    memset(pBullet, 0, sizeof(struct Bullet));
    pBullet->atk = 0;
    pBullet->ptn = 0;
    pBullet->flg = 0;
    InitEntity(&pBullet->entity);
}
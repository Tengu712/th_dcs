#include "../include/HeaderApp.hpp"

Bullet::Bullet() : cnt(0U), ptn(0), moving(false), fact(Fact()) {}

void Bullet::init(int knd) {
    moving = true;
    fact.posZ = 1.5f;
    fact.texid = knd;
    if (knd == TEX_BU_SELF0) {
        fact.sclX = 5000.0f;
        fact.sclY = 5000.0f;
    }
    else {
        moving = false;
        fact.texid = 0;
    }
}

void Bullet::update(GameInf* pGinf) {
    move();

    fact.posX = (float)x;
    fact.posY = (float)y;
    cnt++;
}

#include "../include/HeaderApp.hpp"

Player::Player() : fact(Fact()) {
    fact.posZ = 2.0f;
    fact.sclX = 8600.0f;
    fact.sclY = 8600.0f;
}

void Player::update(GameInf* pGinf) {
    int dx = pGinf->getKey(KEY_CODE::Right, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Left, KEY_STA::Pressed);
    int dy = pGinf->getKey(KEY_CODE::Up, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Down, KEY_STA::Pressed);
    spd = pGinf->getKey(KEY_CODE::Shift, KEY_STA::Pressed) ? spdSlow : spdNorm;
    if (dx == 1 && dy == 0)
        deg = 0;
    else if (dx == 1 && dy == 1)
        deg = 45;
    else if (dx == 0 && dy == 1)
        deg = 90;
    else if (dx == -1 && dy == 1)
        deg = 135;
    else if (dx == -1 && dy == 0)
        deg = 180;
    else if (dx == -1 && dy == -1)
        deg = 225;
    else if (dx == 0 && dy == -1)
        deg = 270;
    else if (dx == 1 && dy == -1)
        deg = 315;
    else
        spd = 0;
    fact.texid = TEX_CH_MARISA_B0;
    move();
    fact.posX = (float)x;
    fact.posY = (float)y;
}

#include "../include/HeaderApp.hpp"

Player::Player() : fact(Fact()) {
    fact.posZ = 2.0f;
    fact.sclX = 10000.0f;
    fact.sclY = 10000.0f;
}

void Player::update(GameInf* pGinf) {
    int dx = pGinf->getKey(KEY_CODE::Right, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Left, KEY_STA::Pressed);
    int dy = pGinf->getKey(KEY_CODE::Up, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Down, KEY_STA::Pressed);
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
    if (dx != 0 || dy != 0)
        move();
    fact.posX = (float)x;
    fact.posY = (float)y;
}

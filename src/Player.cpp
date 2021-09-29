#include "../include/HeaderApp.hpp"

Player::Player() : 
    cnt(0U),
    cntSlow(0U),
    spdNorm(0),
    spdSlow(0),
    interval(0),
    atk(0),
    numOpt(0),
    widthShot(0),
    kndShot(0),
    kndSkill(0),
    fact(Fact()) 
{
    fact.sclX = 8600.0f;
    fact.sclY = 8600.0f;
}

void Player::update(GameInf* pGinf) {

    // Shot
    if (pGinf->getKey(KEY_CODE::Z, KEY_STA::Pressed) && cnt % interval == 0) {
        Bullet bul = Bullet();
        // Main shot
        bul.init(TEX_BU_SELF0);
        bul.deg = 90;
        bul.spd = 4000;
        bul.atk = atk;
        bul.y = y + 100000;
        bul.x = x - 150000;
        pGinf->pushBulletSelf(&bul);
        bul.x = x + 150000;
        pGinf->pushBulletSelf(&bul);
    }

    // Slow
    if (pGinf->getKey(KEY_CODE::Shift, KEY_STA::Pressed))
        cntSlow++;
    else
        cntSlow = 0U;

    int dx = pGinf->getKey(KEY_CODE::Right, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Left, KEY_STA::Pressed);
    int dy = pGinf->getKey(KEY_CODE::Up, KEY_STA::Pressed) - pGinf->getKey(KEY_CODE::Down, KEY_STA::Pressed);
    spd = cntSlow > 0U ? spdSlow : spdNorm;
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
    move();
    x = max(-3800000, min(3800000, x));
    y = max(-4600000, min(4600000, y));
    fact.posX = (float)x;
    fact.posY = (float)y;
    if (dx == 1)
        fact.texid = TEX_CH_MARISA_R0 + ((cnt / 6) % 2);
    else if (dx == -1)
        fact.texid = TEX_CH_MARISA_L0 + ((cnt / 6) % 2);
    else
        fact.texid = TEX_CH_MARISA_B0 + ((cnt / 8) % 4);
    cnt++;
}

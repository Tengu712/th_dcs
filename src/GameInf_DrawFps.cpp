#include "../include/HeaderApp.hpp"

void GameInf::drawFps() {
    Fact fact = Fact();
    fact.posX = 525.0f;
    fact.posY = -457.0f;
    fact.sclX = 0.15f;
    fact.sclY = 0.2f;
    const unsigned int tmpfps = (unsigned int)(fps * 10.0f);
    for (int i = 0; i < 7; ++i) {
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
        fact.posX += 15.0f;
        applyFact(&fact);
        dmanager.applyTexture(getFont(fact.texid));
        dmanager.drawModel(&idea);
    }
}
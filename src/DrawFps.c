#include "../include/HeaderApp.h"

void DrawFps(struct D3DInf* pDinf, struct GameInf* pGinf) {
    struct Fact fact;
    CreateFact(&fact);
    fact.posX = 525.0f;
    fact.posY = -457.0f;
    fact.sclX = 0.15f;
    fact.sclY = 0.2f;
    const unsigned int tmpfps = (unsigned int)(pGinf->fps * 10.0f);
    for (int i = 0; i < 7; ++i) {
        if (i == 0) 
            fact.imgid = ToFontID(((tmpfps / 100) % 10) + 48U);
        else if (i == 1)
            fact.imgid = ToFontID(((tmpfps / 10) % 10) + 48U);
        else if (i == 3)
            fact.imgid = ToFontID((tmpfps % 10) + 48U);
        else if (i == 2)
            fact.imgid = ToFontID(Lpcstr2uint("."));
        else if (i == 4)
            fact.imgid = ToFontID(Lpcstr2uint("f"));
        else if (i == 5)
            fact.imgid = ToFontID(Lpcstr2uint("p"));
        else
            fact.imgid = ToFontID(Lpcstr2uint("s"));
        fact.posX += 15.0f;
        ApplyFact(pGinf, &fact);
        ApplyImage(pDinf, GetImage(pGinf, fact.imgid));
        DrawModel(pDinf, &pGinf->idea);
    }
}

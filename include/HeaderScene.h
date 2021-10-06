#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.h"

#define MAX_NUM_SCE 5

#define SCE_Title 0
#define SCE_MainMenu 1
#define SCE_Tutorial 2
#define SCE_Exit 255

inline void DrawImage(struct GameInf* pGinf, struct D3DInf* pDinf, struct Fact* pFact, struct Image* pImage) {
    ApplyFact(pGinf, pFact);
    ApplyImage(pDinf, pImage);
    DrawModel(pDinf, &pGinf->idea);
}

void InitTitle(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateTitle(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

void InitMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

void InitGame(struct GameInf* pGinf);
void UpdateGame(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);
void DrawGame(struct GameInf* pGinf, struct D3DInf* pDinf);

void InitTutorial(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

#endif
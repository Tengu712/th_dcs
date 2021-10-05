#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.h"

#define SCE_Title 0
#define SCE_Tutorial 1
#define SCE_Exit 255

void InitTitle(struct GameInf* pGinf);
void UpdateTitle(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

void InitGame(struct GameInf* pGinf);
char UpdateGame(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);
void DrawGame(struct GameInf* pGinf, struct D3DInf* pDinf);

void InitTutorial(struct GameInf* pGinf);
void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

#endif
#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.h"

#define MAX_NUM_SCE 5

#define SCE_Title 0
#define SCE_MainMenu 1
#define SCE_Tutorial 2
#define SCE_Exit 255

char InitTitle(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateTitle(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

char InitMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateMainMenu(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

char InitGame(struct GameInf* pGinf);
void UpdateGame(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);
void DrawGame(struct GameInf* pGinf, struct D3DInf* pDinf);

char InitTutorial(struct GameInf* pGinf, struct D3DInf* pDinf);
void UpdateTutorial(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf);

#endif
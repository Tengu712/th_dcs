#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.h"

#define MAX_NUM_SCE 5

#define SCE_Title 0
#define SCE_MainMenu 1
#define SCE_Tutorial 2
#define SCE_EscapeNumber 120
#define SCE_Exit 121

#define SCE_GAME_Logue 0
#define SCE_GAME_Game 1
#define SCE_GAME_Win 2
#define SCE_GAME_Result 3
#define SCE_GAME_Pause 4
#define SCE_GAME_GOFade 5
#define SCE_GAME_GameOver 6

#define CNT_ALL 0
#define CNT_MODE 1
#define CNT_STATE 2
#define CNT_FADE 3
#define CNT_PREV 4

struct Infs {
    struct GameInf* pGinf;
    struct D3DInf* pDinf;
    struct InputInf* pIinf;
};

char InitTitle(struct Infs* pinfs);
void UpdateTitle(struct Infs* pinfs);

char InitMainMenu(struct Infs* pinfs);
void UpdateMainMenu(struct Infs* pinfs);

char InitGame(struct Infs* pinfs);
void UpdateGame(struct Infs* pinfs, void (*fLog)(struct Infs*),
        void (*fGame)(struct Infs*), void (*fWin)(struct Infs*), void (*fBG)(struct Infs*));

char InitTutorial(struct Infs* pinfs);
void UpdateTutorial(struct Infs* pinfs);

#endif
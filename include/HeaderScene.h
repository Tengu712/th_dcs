#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.h"

#define MAX_NUM_SCE 5

#define SCE_Title 0
#define SCE_MainMenu 1
#define SCE_Tutorial 2
#define SCE_Exit 255

#define SCE_GAME_Logue 0
#define SCE_GAME_Game 1
#define SCE_GAME_Win 2
#define SCE_GAME_Result 3
#define SCE_GAME_Pause 4

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
inline void SwitchPause(struct Infs* pinfs) {
    if (GetKey(pinfs->pIinf, KEY_CODE_Esc) & KEY_STA_Down && pinfs->pGinf->cntSce1 != SCE_GAME_Result) {
        if (pinfs->pGinf->cntSce1 == SCE_GAME_Pause) {
            pinfs->pGinf->cntSce1 = 0; //!
        } else {
            pinfs->pGinf->cntSce1 = SCE_GAME_Pause;
        }
    }
}
void UpdateGame(struct Infs* pinfs);
void DrawGame(struct Infs* pinfs);

char InitTutorial(struct Infs* pinfs);
void UpdateTutorial(struct Infs* pinfs);

#endif
#pragma once
#ifndef _HEADER_APP_H_
#define _HEADER_APP_H_

#include "HeaderD3D11.h"
#include "HeaderXInput.h"
#include "resource.h"

#pragma comment(lib, "mydx.lib")

// ================================================================================================================= //
//                                          Constants
// ================================================================================================================= //

#define MAX_IMG 100
#define MAX_IMG_TMP 100
#define MAX_QUE_BG 20
#define MAX_QUE_UI 50
#define MAX_BUL_E 150
#define MAX_BUL_P 20
#define MAX_LOGUE 20

#define KEY_CODE_Up 1
#define KEY_CODE_Down 2
#define KEY_CODE_Left 3
#define KEY_CODE_Right 4
#define KEY_CODE_Z 5
#define KEY_CODE_Shift 6
#define KEY_CODE_Esc 7

#define KEY_STA_Down 0b010
#define KEY_STA_Pressed 0b001
#define KEY_STA_Up 0b100

#define MAX_KND_BUL 10
#define BUL_SELF_0 0

// ================================================================================================================= //
//                                          Structs
// ================================================================================================================= //

struct Fact {
    char enable;
    unsigned int imgid;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
};

struct Player {
    unsigned int cnt;
    unsigned int cntSlow;
    int x, y, dx;
};

struct Bullet {
    // 0:empty 1:moving 2:grazed 3:hitted
    unsigned int flg;
    unsigned int cnt;
    unsigned int knd;
    int atk;
    int ptn;
    int x, y;
    int deg, spd;
};

struct BulletInf {
    int r, rGrz;
    float sclX, sclY;
    float colR, colG, colB, colA;
};

struct Logue {
    char flg;
    char isRight;
    unsigned int imgidL;
    unsigned int imgidR;
};

struct SaveData {
    unsigned int cntPlay;
    unsigned int cntWorldRound;
    unsigned int scoreTotalGot;
    unsigned int scoreTotal;
    int spdNorm;
    int spdSlow;
    int r;
    int rGrz;
    int atk;
    int interval;
    int numOpt;
    int widthShot;
    int kndShot;
    int kndSkill;
};

struct GameInf {
    // System
    float fps;
    char sceCur;
    char sceNex;
    struct ModelInf idea;
    struct FrameBuffer fb0;
    struct FrameBuffer fb1;
    struct SaveData data;
    // Camera
    struct Camera cameraBG;
    struct Camera cameraGame;
    struct Camera cameraUI;
    // Storage
    struct Image* imgs;
    struct Image* imgsTmp;
    struct Fact* queBG;
    struct Fact* queUI;
    struct Bullet* bulsE;
    struct Bullet* bulsP;
    struct BulletInf* binfs;
    unsigned int* imgidsLog;
    // Game
    struct Player player;
    struct Logue log;
};

// ================================================================================================================= //
//                                          Functions
// ================================================================================================================= //

char CreateGameInf(struct GameInf* pGinf, struct D3DInf* pDinf, unsigned int width, unsigned int height);
void UpdateGameInf(struct GameInf* pGinf);
void FreeGameInf(struct GameInf* pGinf);

char LoadAddImage(struct GameInf* pGinf, struct D3DInf* pDinf, HMODULE hModule, unsigned int id);
char LoadAddFont(struct GameInf* pGinf, struct D3DInf* pDinf, unsigned int code);
char LoadSentence(struct GameInf* pGinf, struct D3DInf* pDinf, LPCSTR str);

struct Image* GetImage(struct GameInf* pGinf, unsigned int id);

inline unsigned int ToFontID(unsigned int code) {
    return code | (1 << 24);
}
inline void ClearFontTmp(struct GameInf* pGinf) {
    for (int i = 0; i < MAX_IMG_TMP; ++i) {
        FreeImage(&pGinf->imgsTmp[i]);
    }
}

void pushBG(struct GameInf* pGinf, struct Fact* pFact);
void pushUI(struct GameInf* pGinf, struct Fact* pFact);
void pushBulletE(struct GameInf* pGinf, struct Bullet* pBul);
void pushBulletP(struct GameInf* pGinf, struct Bullet* pBul);

inline void CreateFact(struct Fact* pFact) {
    memset(pFact, 0, sizeof(struct Fact));
    struct Fact tmp = {
        0, 0U,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    *pFact = tmp;
}
inline void ApplyFact(struct GameInf* pGinf, struct Fact* pFact) {
    pGinf->idea.posX = pFact->posX;
    pGinf->idea.posY = pFact->posY;
    pGinf->idea.posZ = pFact->posZ;
    pGinf->idea.degX = pFact->degX;
    pGinf->idea.degY = pFact->degY;
    pGinf->idea.degZ = pFact->degZ;
    pGinf->idea.sclX = pFact->sclX;
    pGinf->idea.sclY = pFact->sclY;
    pGinf->idea.sclZ = pFact->sclZ;
    pGinf->idea.colR = pFact->colR;
    pGinf->idea.colG = pFact->colG;
    pGinf->idea.colB = pFact->colB;
    pGinf->idea.colA = pFact->colA;
}

void CreateBullet(struct Bullet* pBul, unsigned int knd);
void UpdateBullet(struct GameInf* pGinf, struct Bullet* pBul);

void CreatePlayer(struct Player* pPlayer);
void UpdatePlayer(struct GameInf* pGinf, struct InputInf* pIinf, struct Player* pPlayer);

void DrawFps(struct GameInf* pGinf, struct D3DInf* pDinf);

void ApplyLogue(struct GameInf* pGinf, char flg, char right, unsigned int imgidL, unsigned int imgidR, LPCSTR str);


#endif

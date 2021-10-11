#pragma once
#ifndef _HEADER_APP_H_
#define _HEADER_APP_H_

#include "HeaderD3D11.h"
#include "HeaderXInput.h"
#include "resource.h"

#pragma comment(lib, "mydx.lib")

#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// ================================================================================================================= //
//                                          Constants
// ================================================================================================================= //

#define MAX_IMG 100
#define MAX_IMG_TMP 100
#define MAX_BUL_E 1024
#define MAX_BUL_P 20
#define MAX_LOGUE 20
#define MAX_PARAM 10

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
#define BUL_HUDA 1

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
    unsigned int imgid;
    unsigned int cnt;
    unsigned int atk, ptn;
    int x, y, r;
    int deg, spd;
    float sclX, sclY;
    float colR, colG, colB, colA;
};

struct Enemy {
    unsigned int imgid;
    unsigned int cnt;
    unsigned int timlim;
    int hp, hpMax;
    int x, y;
    int deg, spd;
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
    unsigned long long scoreTotalGot;
    unsigned long long scoreTotal;
    long long scoreInit;
    int spdNorm;
    int spdSlow;
    int r;
    int rGrz;
    int atk;
    int interval;
    int invtime;
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
    LOGFONTA font;
    struct Image* imgs;
    struct Image* imgsTmp;
    struct Bullet* bulsE;
    struct Bullet* bulsP;
    unsigned int* imgidsLog;
    // Game
    int* cntSce;
    unsigned int grz;
    long long score;
    struct Player player;
    struct Enemy enemy;
    struct Logue log;
};

// ================================================================================================================= //
//                                          Functions
// ================================================================================================================= //

char CreateGameInf(struct GameInf* pGinf, struct D3DInf* pDinf, struct InputInf* pIinf,
        unsigned int width, unsigned int height);
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

inline void MoveEntity(int* x, int* y, int deg, int spd) {
    const double rad = (double)deg / 180.0 * M_PI;
    *x += (int)(cos(rad) * 10000.0) * spd / 100;
    *y += (int)(sin(rad) * 10000.0) * spd / 100;
}

inline void CreateFact(struct Fact* pFact) {
    memset(pFact, 0, sizeof(struct Fact));
    pFact->sclX = 1.0f;
    pFact->sclY = 1.0f;
    pFact->sclZ = 1.0f;
    pFact->colR = 1.0f;
    pFact->colG = 1.0f;
    pFact->colB = 1.0f;
    pFact->colA = 1.0f;
}
void ApplyFact(struct GameInf* pGinf, struct Fact* pFact);

inline void DrawImage(struct GameInf* pGinf, struct D3DInf* pDinf, struct Fact* pFact, struct Image* pImage) {
    ApplyFact(pGinf, pFact);
    ApplyImage(pDinf, pImage);
    DrawModel(pDinf, &pGinf->idea);
}

void CreateBullet(struct Bullet* pBul, unsigned int knd);
void UpdateBullet(struct GameInf* pGinf, struct Bullet* pBul);
void ClearBulletE(struct GameInf* pGinf);

void UpdatePlayer(struct GameInf* pGinf, struct InputInf* pIinf, struct Player* pPlayer, char shootable);

void DrawFps(struct D3DInf* pDinf, struct GameInf* pGinf);

void ApplyLogue(struct GameInf* pGinf, char flg, char right, unsigned int imgidL, unsigned int imgidR, LPCSTR str);


#endif

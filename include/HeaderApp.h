#ifndef INCLUDE_APP
#define INCLUDE_APP

#include "HeaderDX11.hpp"
#include "resource.hpp"

#pragma comment(lib, "mydx.lib")

// ================================================================================================================= //
//                                          Constants
// ================================================================================================================= //

// MAX
#define MAX_TEX 100
#define MAX_FNT 20
#define MAX_FNT_TMP 100
#define MAX_QUE_BG 20
#define MAX_QUE_UI 50
#define MAX_QUE_FNT 50
#define MAX_BUL_E 150
#define MAX_BUL_P 20
#define MAX_ENEMY 20
#define MAX_LOGUE 20

// SCE_ID
#define SCE_ID_Title 0
#define SCE_ID_Tutorial 1
#define SCE_ID_Exit 255

// KEY_CODE
#define KEY_CODE_Up 1
#define KEY_CODE_Down 2
#define KEY_CODE_Left 3
#define KEY_CODE_Right 4
#define KEY_CODE_Z 5
#define KEY_CODE_Shift 6
#define KEY_CODE_Esc 7

// KEY_STA
#define KEY_STA_Neutral 0
#define KEY_STA_Down 1
#define KEY_STA_Pressed 2
#define KEY_STA_Up 3

// ================================================================================================================= //
//                                          Structs
// ================================================================================================================= //

struct Fact {
    unsigned int texid;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    char enable;
};

struct Entity {
    unsigned int cnt;
    int x, y;
    int r, rGrz;
    int deg, spd;
    struct Fact fact;
};

struct Player {
    unsigned int cntSlow;
    struct Entity entity;
};

struct Bullet {
    int atk;
    int ptn;
    int flg;
    struct Entity entity;
};

/*
struct Logue {
    bool flg;
    bool right;
    unsigned int texidLeft;
    unsigned int texidRight;
    Logue() :
        flg(false),
        right(false),
        texidLeft(0),
        texidRight(0)
    {}
};
*/

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
    // Manager
    D3DManager dmanager;
    InputManager imanager;
    AudioManager amanager;
    // System
    ModelInf idea;
    FrameBuffer fb0;
    FrameBuffer fb1;
    SaveData data;
    // Storage
    Texture* texs;
    Texture* fonts;
    Texture* fontsTmp;
    Fact* queBG;
    Fact* queUI;
    Fact* queFont;
    Bullet* bulsE;
    Bullet* bulsP;
    unsigned int* texidsLog;
    // Camera
    Camera cameraGame;
    Camera cameraUI;
    // Fps
    int cntFps;
    float fps;
    long startTime;
    long lastTime;
    // Game
    Player player;
    Enemy* enemies;
    Logue log;
    char sceCur;
    char sceNex;
    Camera cameraBG;
};

// ================================================================================================================= //
//                                          Functions
// ================================================================================================================= //

void InitFact(struct Fact* pFact);
void InitEntity(struct Entity* pEntity);
void InitPlayer(struct Player* pPlayer);
void InitBullet(struct Bullet* pBullet);

bool InitGameInf(GameInf* pGinf, HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName,
        unsigned int width, unsigned int height, bool windowed);

bool setKeyConfig();
bool loadFontTmp(unsigned int code);
void applyFact(Fact* pFact);
void updateEntity(Entity* pEntity);
void updateBullet(Bullet* pBul);
void updateEnemy(Enemy* pEnemy);
int isHit(Bullet* pBul, Entity* pEntity);
void drawFps();

// Static
void clearFontTmp();
Texture* getTexture(unsigned int id);
Texture* getFont(unsigned int code);
bool getKey(KEY_CODE code, KEY_STA status);
bool saveData();
// Load
bool loadTexture(HMODULE hModule, unsigned int id);
bool loadFont(unsigned int code);
bool loadSentence(LPCSTR str);
// Push
void pushBG(Fact* pFact);
void pushUI(Fact* pFact);
void pushFont(Fact* pFact);
void pushBulletE(Bullet* pBul);
void pushBulletP(Bullet* pBul);
void pushEnemy(Enemy* pEnemy);
// Object
void applyLogue(bool flg, bool right, unsigned int texidLeft, unsigned int texidRight, LPCSTR str);
void createBullet(Bullet* pBul, int knd);
void createEnemy(Enemy* pEnemy, int knd);
// Update
void update();
void updateBullets();
void updateEnemies();
void updatePlayer();
// Draw
void drawTitle();
void drawGame();
void drawMainMenu();

#endif

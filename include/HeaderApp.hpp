#ifndef INCLUDE_APP
#define INCLUDE_APP

#include "HeaderDX11.hpp"
#include "resource.hpp"

#pragma comment(lib, "mydx.lib")

class GameInf;

struct Fact {
    unsigned int texid;
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    bool enable;
    Fact() :
        texid(0U),
        posX(0.0f),
        posY(0.0f),
        posZ(0.0f),
        degX(0.0f),
        degY(0.0f),
        degZ(0.0f),
        sclX(1.0f),
        sclY(1.0f),
        sclZ(1.0f),
        colR(1.0f),
        colG(1.0f),
        colB(1.0f),
        colA(1.0f),
        enable(false)
    {}
};

struct Entity {
    unsigned int cnt;
    int x, y;
    int r, rGrz;
    int deg, spd;
    Fact fact;
    Entity() :
        cnt(0U),
        x(0),
        y(0),
        r(0),
        rGrz(0),
        deg(0),
        spd(0),
        fact(Fact())
    {}
};

struct Player : public Entity {
    unsigned int cntSlow;
    Player() :
        cntSlow(0U)
    {}
};

struct Bullet : public Entity {
    int atk;
    int ptn;
    int flgHit;
    bool moving;
    Bullet() :
        atk(0),
        ptn(0),
        flgHit(0),
        moving(false)
    {}
};

struct Enemy : public Entity {
    int hp;
    int ptnMov, ptnFire;
    bool moving;
    Enemy() :
        hp(0),
        ptnMov(0),
        ptnFire(0),
        moving(false)
    {}
};

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

enum struct SCE_ID : char {
    Title,
    Tutorial,
    Exit,
};

enum struct KEY_CODE : char {
    Up,
    Down,
    Left,
    Right,
    Z,
    Shift,
    Esc,
};

enum struct KEY_STA : char {
    Neutral,
    Down,
    Pressed,
    Up,
};

class GameInf {
    private:
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
        // Method
        bool setKeyConfig();
        bool loadFontTmp(unsigned int code);
        void applyFact(Fact* pFact);
        void updateEntity(Entity* pEntity);
        void updateBullet(Bullet* pBul);
        void updateEnemy(Enemy* pEnemy);
        int isHit(Bullet* pBul, Entity* pEntity);
        void drawFps();
    public:
        SCE_ID sceCur;
        SCE_ID sceNex;
        Camera cameraBG;

        // Method
        GameInf() :
            dmanager(D3DManager()),
            imanager(InputManager()),
            amanager(AudioManager()),
            idea(ModelInf()),
            fb0(FrameBuffer()),
            fb1(FrameBuffer()),
            data(SaveData()),
            texs(nullptr),
            fonts(nullptr),
            fontsTmp(nullptr),
            queBG(nullptr),
            queUI(nullptr),
            queFont(nullptr),
            bulsE(nullptr),
            bulsP(nullptr),
            texidsLog(nullptr),
            cameraGame(Camera()),
            cameraUI(Camera()),
            cntFps(0),
            fps(0.0f),
            startTime(timeGetTime()),
            lastTime(timeGetTime()),
            player(Player()),
            enemies(nullptr),
            log(Logue()),
            sceCur(SCE_ID::Title),
            sceNex(SCE_ID::Title),
            cameraBG(Camera())
    {}
        bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, 
                unsigned int width, unsigned int height, bool windowed);
        // Static
        ~GameInf();
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
};

#endif
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
        colA(1.0f)
    {}
};


class Entity {
    public:
        int x, y, r, rGrz;
        int deg, spd;
        int flgHit;
        Entity();
        void move();
        void checkHit(Entity* pEntity);
};


class Player : public Entity {
    public:
        unsigned int cnt;
        unsigned int cntSlow;
        int spdNorm;
        int spdSlow;
        int interval;
        Fact fact;
        Player();
        void update(GameInf* pGinf);
};

class Bullet : public Entity {
    public:
        unsigned int cnt;
        int ptn;
        bool moving;
        Fact fact;
        Bullet();
        void init(int knd);
        void update(GameInf* pGinf);
};


// --------------------------------------------------------------------------------------------------------------------

#define MAX_TEX 100
#define MAX_FNT 100
#define MAX_QUE_BG 20
#define MAX_QUE_UI 50
#define MAX_QUE_FNT 50
#define MAX_KND_BUL 5
#define MAX_NUM_BUL 150
#define MAX_KND_BUL_SELF 2
#define MAX_NUM_BUL_SELF 20

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
        D3DManager dmanager;
        InputManager imanager;
        AudioManager amanager;
        void applyFact(Fact* pFact);
    public:
        // System
        SCE_ID sceCur;
        SCE_ID sceNex;
        Texture* texs;
        Texture* fonts;
        ModelInf ideaSquare;
        // Queue
        Fact** queBG;
        Fact** queUI;
        Fact** queFont;
        // Camera
        Camera cameraBG;
        Camera cameraGame;
        Camera cameraUI;
        // Fps
        int cntFps;
        float fps;
        long startTime;
        long lastTime;
        // Game
        Player player;
        int* mapBullets;
        int* mapBulletsSelf;
        Bullet** bullets;
        Bullet** bulletsSelf;

        // Method
        GameInf();
        ~GameInf();
        bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, 
                unsigned int width, unsigned int height, bool windowed);
        // System
        bool addTexture(HMODULE hModule, unsigned int id);
        bool addFont(unsigned int code);
        bool setKeyConfig();
        bool getKey(KEY_CODE code, KEY_STA status);
        bool saveData();
        Texture* getTexture(unsigned int id);
        Texture* getFont(unsigned int code);
        // Queue
        void pushBG(Fact* pFact);
        void pushUI(Fact* pFact);
        void pushFont(Fact* pFact);
        // Game
        void pushBullet(Bullet* bullet);
        void pushBulletSelf(Bullet* bullet);
        // General
        void update();
        void draw();
};

class AScene {
    public:
        virtual void init(GameInf* pGinf) = 0;
        virtual void update(GameInf* pGinf) = 0;
};

class SceneTitle : public AScene {
    private:
        Fact bg;
    public:
        SceneTitle();
        void init(GameInf* pGinf);
        void update(GameInf* pGinf);
};

class ASceneGame : public AScene {
    private:
        Fact frame;
    public:
        ASceneGame();
        void gameInit(GameInf* pGinf);
        void gameUpdate(GameInf* pGinf);
        virtual void init(GameInf* pGinf) = 0;
        virtual void update(GameInf* pGinf) = 0;
};

class SceneTutorial : public ASceneGame {
    private:
        Fact bg;
    public:
        SceneTutorial();
        void init(GameInf* pGinf);
        void update(GameInf* pGinf);
};


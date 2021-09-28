#include "HeaderDX11.hpp"
#include "resource.hpp"

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
        int spdNorm;
        int spdSlow;
        Fact fact;
        Player();
        void update(GameInf* pGinf);
};


// --------------------------------------------------------------------------------------------------------------------

#define MAX_TEX 100
#define MAX_FNT 100
#define MAX_QUE_UI 50
#define MAX_QUE_FNT 50

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
        Fact** queUI;
        Fact** queFont;
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
        void pushUI(Fact* pFact);
        void pushFont(Fact* pFact);
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
    public:
        SceneTutorial();
        void init(GameInf* pGinf);
        void update(GameInf* pGinf);
};


#include "HeaderDX11.hpp"

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

class FactQueue {
    private:
        unsigned int nummax;
        Fact** facts;
    public:
        FactQueue();
        ~FactQueue();
        void init(unsigned int argmax);
        void clear();
        void push(Fact* pFact);
        Fact* pop();
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

class FpsCalculator {
    private:
        int cntFps;
        long startTime;
        long lastTime;
    public:
        float fps;
        FpsCalculator();
        void update();
};


// --------------------------------------------------------------------------------------------------------------------

#define MAX_TEX 100
#define MAX_FNT 100
#define MAX_QUE_UI 50
#define MAX_QUE_FNT 50

enum struct SCE_ID : char {
    Title,
    Exit,
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
        FactQueue queUI;
        FactQueue queFont;
        // Camera
        Camera cameraUI;
        // Fps
        FpsCalculator fpsCalculator;
        // Method
        GameInf();
        ~GameInf();
        bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, 
                unsigned int width, unsigned int height, bool windowed, HMODULE hModule);
        // System
        bool addTexture(HMODULE hModule, unsigned int id);
        bool addFont(unsigned int code);
        bool setKeyConfig();
        Texture* getTexture(unsigned int id);
        Texture* getFont(unsigned int code);

        // General
        void update();
        void draw();
};

class AScene {
    public:
        virtual bool init(GameInf* pGinf) = 0;
        virtual void update(GameInf* pGinf) = 0;
};

class SceneTitle : public AScene {
    private:
        Fact bg;
    public:
        SceneTitle();
        bool init(GameInf* pGinf);
        void update(GameInf* pGinf);
};


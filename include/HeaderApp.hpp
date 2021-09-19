#include "HeaderDX11.hpp"

struct GameTexture : public Texture {
    unsigned int id;
};

class Model : public ModelInf {
    public:
        unsigned int texid;
        Model();
        virtual bool init(D3DManager* pManager) = 0;
};

class ModelSquare : public Model {
    public:
        bool init(D3DManager* pManager);
};

class ModelCube : public Model {
    public:
        bool init(D3DManager* pManager);
};

class ModelQueue {
    private:
        unsigned int nummax;
        Model** models;
    public:
        ModelQueue();
        ~ModelQueue();
        void init(unsigned int argmax);
        void push(Model* pModel);
        Model* pop();
};


// --------------------------------------------------------------------------------------------------------------------

#define MAX_TEX 100
#define MAX_FNT 100
#define MAX_UI 50

enum struct SCE_ID : char {
    Title,
    Exit,
};

class GameInf {
    private:
        D3DManager dmanager;
        InputManager imanager;
        AudioManager amanager;
    public:
        // System
        SCE_ID sceCur;
        SCE_ID sceNex;
        GameTexture* texs;
        GameTexture* fonts;
        // Queue
        ModelQueue queUI;
        // Camera
        Camera cameraUI;
        // Fps
        int cntFps;
        float fps;
        long startTime;
        long lastTime;
        ModelSquare* modelsFps;

        // Method
        GameInf();
        ~GameInf();
        bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
                unsigned int height, bool windowed);
        // System
        bool addTexture(unsigned int id);
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
        ModelSquare bg;
    public:
        SceneTitle();
        bool init(GameInf* pGinf);
        void update(GameInf* pGinf);
};


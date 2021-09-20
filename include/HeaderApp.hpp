#include "HeaderDX11.hpp"

class Model : public ModelInf {
    public:
        unsigned int texid;
        Model();
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
        Texture* texs;
        Texture* fonts;
        // Queue
        ModelQueue queUI;
        // Camera
        Camera cameraUI;
        // Fps
        int cntFps;
        float fps;
        long startTime;
        long lastTime;
        Model* modelsFps;

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

        // Model
        bool createModelSquare(Model* pModel);

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
        Model bg;
    public:
        SceneTitle();
        bool init(GameInf* pGinf);
        void update(GameInf* pGinf);
};


#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include "./HeaderApp.hpp"

class AScene {
    public:
        virtual void init(GameInf* pGinf) = 0;
        virtual void update(GameInf* pGinf) = 0;
};

class ASceneGame {
    public:
        int cnt;
        ASceneGame() : cnt(0) {}
        void gameInit(GameInf* pGinf);
        void gameUpdate(GameInf* pGinf);
};

class SceneTitle : public AScene {
    private:
        Fact bg;
    public:
        SceneTitle() : bg(Fact()) {}
        void init(GameInf* pGinf);
        void update(GameInf* pGinf);
};

class SceneTutorial : public AScene, ASceneGame {
    private:
        Fact bg, cl;
    public:
        SceneTutorial() :
            bg(Fact()),
            cl(Fact())
    {}
        void init(GameInf* pGinf);
        void update(GameInf* pGinf);
};

#endif
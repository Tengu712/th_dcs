#include "HeaderDX11.hpp"

struct GameTexture : public Texture{
    unsigned int id;
};

class Model : public ModelInf {
    public:
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


// --------------------------------------------------------------------------------------------------------------------

#define MAX_TEXTURE 100

bool SetKeyConfig(InputManager* pManager);

enum SCE {
    SCE_TITLE,
    SCE_EXIT,
};

class GameInf {
    public:
        int sceCur, sceNex;
        GameTexture texs[MAX_TEXTURE];
        GameTexture fonts[MAX_TEXTURE];
        GameInf();
        bool addTexture(D3DManager* pManager, unsigned int id);
        bool addFont(D3DManager* pManager, unsigned int code);
        Texture* getTexture(unsigned int id);
        Texture* getFont(unsigned int code);
};

class AScene {
    public:
        virtual void update() = 0;
};


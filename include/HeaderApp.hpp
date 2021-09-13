#include "HeaderDX11.hpp"

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


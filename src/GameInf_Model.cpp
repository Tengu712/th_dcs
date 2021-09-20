#include "../include/HeaderApp.hpp"

bool GameInf::createModelSquare(Model* pModel) {
    const unsigned int kNumVtx = 4U;
    const unsigned int kNumIdx = 6U;
    pModel->numIdx = kNumIdx;

    struct Vertex dataPCNU[kNumVtx] = {
        {-50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
        {-50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        {+50.0f, +50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
        {+50.0f, -50.0f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
    };
    unsigned int dataIdx[kNumIdx] = {0, 1, 2, 0, 2, 3};

    return dmanager.createModelBuffers(kNumVtx, dataPCNU, dataIdx, pModel);
}


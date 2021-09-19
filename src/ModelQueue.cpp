#include "../include/HeaderApp.hpp"

ModelQueue::ModelQueue() : nummax(0U), models(nullptr) {}

ModelQueue::~ModelQueue() {
    if (models != nullptr)
        delete models;
}

void ModelQueue::init(unsigned int argmax) {
    nummax = argmax;
    models = new Model*[nummax];
}

void ModelQueue::push(Model* pModel) {
    for (int i = 0; i < nummax; ++i) {
        if (models[i] != nullptr) 
            continue;
        models[i] = pModel;
        break;
    }
}

Model* ModelQueue::pop() {
    Model* res = nullptr;
    for (int i = 0; i < nummax; ++i) {
        if (models[i] == nullptr)
            continue;
        res = models[i];
        models[i] = nullptr;
        break;
    }
    return res;
}

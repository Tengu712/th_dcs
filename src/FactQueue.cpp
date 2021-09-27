#include "../include/HeaderApp.hpp"

FactQueue::FactQueue() : nummax(0U), facts(nullptr) {}

FactQueue::~FactQueue() {
    if (facts != nullptr)
        delete facts;
}

void FactQueue::init(unsigned int argmax) {
    nummax = argmax;
    facts = new Fact*[argmax];
}

void FactQueue::clear() {
    for (int i = 0; i < nummax; ++i) {
        facts[i] = nullptr;
    }
}

void FactQueue::push(Fact* pFact) {
    for (int i = 0; i < nummax; ++i) {
        if (facts[i] != nullptr) 
            continue;
        facts[i] = pFact;
        break;
    }
}

Fact* FactQueue::pop() {
    Fact* res = nullptr;
    for (int i = 0; i < nummax; ++i) {
        if (facts[i] == nullptr)
            continue;
        res = facts[i];
        facts[i] = nullptr;
        break;
    }
    return res;
}

#include "../include/HeaderApp.hpp"

#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

Entity::Entity() :
    x(0),
    y(0),
    r(0),
    rGrz(0),
    deg(0),
    spd(0),
    flgHit(0)
{}

void Entity::move() {
    const double rad = (double)deg / 360.0 * 2.0 * M_PI;
    x += (int)cos(rad);
    y += (int)sin(rad);
}

void Entity::checkHit(Entity* pEntity) {
    const int dis = (x - pEntity->x) * (x - pEntity->x) + (y - pEntity->y) * (y - pEntity->y);
    if (rGrz + pEntity->rGrz > dis)
        flgHit = 1;
    if (r + pEntity->r > dis)
        flgHit = 2;
}

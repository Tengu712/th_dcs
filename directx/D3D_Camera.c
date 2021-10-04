#include "../include/HeaderD3D11.h"

#include <math.h>

void CreateCamera(float width, float height, struct Camera* pCamera) {
    memset(pCamera, 0, sizeof(struct Camera));
    struct Camera tmp = {
        width, height, 
        MD_PIDIV4, 1.0f, 2000.0f, 
        0.0f, 0.0f, 0.0f, 
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    *pCamera = tmp;
}

inline float dot(float a1, float a2, float a3, float b1, float b2, float b3) {
    return a1 * b1 + a2 * b2 + a3 * b3;
}

void ApplyCamera(struct D3DInf* pDinf, struct Camera* pCamera, BOOL parse) {
    
    struct MDFLOAT4x4 matView = {
        pCamera->rghX, pCamera->uppX, pCamera->dirX,
        -1.0f * dot(pCamera->rghX, pCamera->rghY, pCamera->rghZ, pCamera->posX, pCamera->posY, pCamera->posZ),
        pCamera->rghY, pCamera->uppY, pCamera->dirY,
        -1.0f * dot(pCamera->uppX, pCamera->uppY, pCamera->uppZ, pCamera->posX, pCamera->posY, pCamera->posZ),
        pCamera->rghZ, pCamera->uppZ, pCamera->dirZ,
        -1.0f * dot(pCamera->dirX, pCamera->dirY, pCamera->dirZ, pCamera->posX, pCamera->posY, pCamera->posZ),
        0.0f, 0.0f, 0.0f, 1.0f
    };
    pDinf->cbuffer.matView = matView;

    const float f_n = pCamera->farZ - pCamera->nearZ;
    if (parse) {
        const float cot = 1 / tan(pCamera->angle / 2.0f);
        struct MDFLOAT4x4 matProj = {
            pCamera->height / pCamera->width * cot, 0.0f, 0.0f, 0.0f,
            0.0f, cot, 0.0f, 0.0f,
            0.0f, 0.0f, pCamera->farZ / f_n, pCamera->farZ * -pCamera->nearZ / f_n,
            0.0f, 0.0f, 1.0f, 0.0f
        };
        pDinf->cbuffer.matProj = matProj;
    } else {
        struct MDFLOAT4x4 matProj = {
            2.0f / pCamera->width, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / pCamera->height, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f / f_n, pCamera->nearZ / f_n,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        pDinf->cbuffer.matProj = matProj;
    }
}

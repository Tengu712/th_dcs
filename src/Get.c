#include "../include/HeaderApp.h"

struct Image* GetImage(struct GameInf* pGinf, unsigned int id) {
    for (int i = 0; i < MAX_IMG; ++i) {
        if (pGinf->imgs[i].id == id)
            return &pGinf->imgs[i];
    }
    for (int i = 0; i < MAX_IMG_TMP; ++i) {
        if (pGinf->imgsTmp[i].id == id) 
            return &pGinf->imgsTmp[i];
    }
    return NULL;
}
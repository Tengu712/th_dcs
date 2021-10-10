#include "../include/HeaderScene.h"

char InitTitle(struct Infs* pinfs) {
    return 1;
}

void UpdateTitle(struct Infs* pinfs) {
    pinfs->pGinf->sceNex = SCE_Tutorial;
}
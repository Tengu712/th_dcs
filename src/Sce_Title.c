#include "../include/HeaderApp.h"

void InitTitle(struct GameInf* pGinf) {

}

void UpdateTitle(struct GameInf* pGinf, struct D3DInf* pDinf) {
    DrawBegin(pDinf, NULL, FALSE);
    ApplyCamera(pDinf, &pGinf->cameraUI, FALSE);
    DrawModel(pDinf, &pGinf->idea);
    DrawEnd(pDinf);
}
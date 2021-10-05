#include "../include/HeaderD3D11.h"

void CreateModelInf(struct ModelInf* pMinf) {
    memset(pMinf, 0, sizeof(struct ModelInf));
    struct ModelInf tmp = {
        0U,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        NULL, NULL
    };
    *pMinf = tmp;
}

BOOL CreateModelBuffer(struct D3DInf* pDinf, struct ModelInf* pMinf, 
        unsigned int numVtx, struct Vertex* dataPCU, unsigned int* dataIdx) 
{
    D3D11_BUFFER_DESC descVBuffer = {
        sizeof(struct Vertex) * numVtx,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0U,
        0U,
        0U
    };
    D3D11_SUBRESOURCE_DATA dataVBuffer = {dataPCU, 0U, 0U};
    if (FAILED(pDinf->pDevice->lpVtbl->CreateBuffer(pDinf->pDevice, &descVBuffer, &dataVBuffer, &pMinf->pVBuffer)))
        return FALSE;

    D3D11_BUFFER_DESC descIndex = {
        (unsigned int)sizeof(unsigned int) * pMinf->numIdx,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_INDEX_BUFFER,
        0U, 
        0U, 
        0U
    };
    D3D11_SUBRESOURCE_DATA dataIndex = {dataIdx, 0U, 0U};
    if (FAILED(pDinf->pDevice->lpVtbl->CreateBuffer(pDinf->pDevice, &descIndex, &dataIndex, &pMinf->pIBuffer)))
        return FALSE;

    return TRUE;
}

void FreeModelInf(struct ModelInf* pMinf) {
    if (pMinf->pVBuffer != NULL)
        COMRelease(pMinf->pVBuffer);
    if (pMinf->pIBuffer != NULL)
        COMRelease(pMinf->pIBuffer);
    memset(pMinf, 0, sizeof(struct ModelInf));
}

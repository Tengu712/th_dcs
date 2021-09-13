#include "../include/HeaderDX11.hpp"

void D3DManager::createCamera(float width, float height, Camera* camera){
    *camera = {width, height, DirectX::XM_PIDIV4, 1.0f, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
}

void D3DManager::applyCamera(Camera* camera) {
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matView, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixLookToLH(
            DirectX::XMVectorSet(camera->posX, camera->posY, camera->posZ, 0.0f),
            DirectX::XMVectorSet(camera->dirX, camera->dirY, camera->dirZ, 0.0f),
            DirectX::XMVectorSet(camera->uppX, camera->uppY, camera->uppZ, 0.0f))));
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matProj, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixPerspectiveFovLH(camera->angle, camera->width / camera->height, camera->nearZ, camera->farZ)));
}

void D3DManager::changeToUI(float width, float height) {
 DirectX::XMStoreFloat4x4(&inf.cbuffer.matView, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixLookToLH(
            DirectX::XMVectorSet(0.0f, 0.0f, -1200.0f, 0.0f),
            DirectX::XMVectorSet(0.0f, 0.0f, 1.0, 0.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))));
    DirectX::XMStoreFloat4x4(&inf.cbuffer.matProj, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixOrthographicLH(width, height, 1.0f, 1000.0f)));
}

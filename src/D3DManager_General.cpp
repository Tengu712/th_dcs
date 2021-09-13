#include "../include/HeaderDX11.hpp"

D3DManager::D3DManager() {
    hWnd = nullptr;
    ZeroMemory(&inf, sizeof(D3DInf));
}

HWND D3DManager::getWindowHandle (){
    return hWnd;
}

D3DInf* D3DManager::getD3DInformation(){
    return &inf;
}

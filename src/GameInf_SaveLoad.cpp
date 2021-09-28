#include "../include/HeaderApp.hpp"

#include <stdio.h>

bool GameInf::loadData() {
    FILE* pFile = fopen("./savedata.dat", "r");
    if (!pFile) {
        MessageBoxA(nullptr, "Failed to open savedata.dat.", "Error", MB_OK);
        return false;
    }

    player.r = 10;
    player.spd = 500;
    player.rGrz = 100;

    fclose(pFile);
    return true;
}

bool GameInf::saveData() {
    FILE* pFile = fopen("./savedata.dat", "w");
    if (!pFile) {
        MessageBoxA(nullptr, "Failed to open savedata.dat.", "Error", MB_OK);
        return false;
    }

    fclose(pFile);
    return true;
}

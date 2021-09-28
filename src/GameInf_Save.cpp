#include "../include/HeaderApp.hpp"

#include <stdio.h>

bool GameInf::saveData() {
    FILE* pFile = fopen("./savedata.dat", "w");
    if (!pFile) {
        MessageBoxA(nullptr, "Failed to open savedata.dat.", "Error", MB_OK);
        return false;
    }

    fclose(pFile);
    return true;
}

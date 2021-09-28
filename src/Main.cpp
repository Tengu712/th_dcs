#include "../include/HeaderApp.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    GameInf ginf = GameInf();
    if(!ginf.init(
                hInst,
                cmdShow,
                L"幻想異郷　～ Darkside Connection of Skypunch",
                L"TH_DCS",
                1280U, 960U,
                MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO)) {
        MessageBoxA(nullptr, "Failed to start game.", "Error", MB_OK);
        UnregisterClassW(L"TH_DCS", hInst);
        return 1;
    }

    AScene* sce = new SceneTitle();
    sce->init(&ginf);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (true) {
        if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            continue;
        }

        ginf.update();
        if (ginf.sceCur != ginf.sceNex) {
            ginf.saveData();
            delete sce;
            if (ginf.sceNex == SCE_ID::Title) 
                sce = new SceneTitle();
            else if (ginf.sceNex == SCE_ID::Tutorial) 
                sce = new SceneTutorial();
            else 
                break;
            sce->init(&ginf);
            ginf.sceCur = ginf.sceNex;
        }
        sce->update(&ginf);
        ginf.draw();
    }

    UnregisterClassW(L"TH_DCS", hInst);
    return 0;
}


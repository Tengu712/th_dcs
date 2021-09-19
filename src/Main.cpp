#include "../include/HeaderApp.hpp"
#include "../include/resource.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    GameInf ginf = GameInf();
    if (!ginf.init(
                hInst,
                cmdShow,
                L"～ Darkside Connection of Skypunch",
                L"TH_DCS",
                1280U, 960U,
                MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO))
        return 1;

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    // Load
    {
        bool tmp = true;
        // Load texture
        // Load font
        tmp &= ginf.addFont(Lpcstr2uint("0"));
        tmp &= ginf.addFont(Lpcstr2uint("1"));
        tmp &= ginf.addFont(Lpcstr2uint("2"));
        tmp &= ginf.addFont(Lpcstr2uint("3"));
        tmp &= ginf.addFont(Lpcstr2uint("4"));
        tmp &= ginf.addFont(Lpcstr2uint("5"));
        tmp &= ginf.addFont(Lpcstr2uint("6"));
        tmp &= ginf.addFont(Lpcstr2uint("7"));
        tmp &= ginf.addFont(Lpcstr2uint("8"));
        tmp &= ginf.addFont(Lpcstr2uint("9"));
        tmp &= ginf.addFont(Lpcstr2uint("."));
        tmp &= ginf.addFont(Lpcstr2uint("f"));
        tmp &= ginf.addFont(Lpcstr2uint("p"));
        tmp &= ginf.addFont(Lpcstr2uint("s"));
        // Key map
        tmp &= ginf.setKeyConfig();
        if (!tmp) {
            MessageBoxA(nullptr, "Failed to load.", "Error", MB_OK);
            UnregisterClassW(L"TH_DCS", hInst);
            return 1;
        }
    }

    // Main
    AScene* sce = new SceneTitle();
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
            delete sce;
            if (ginf.sceNex == SCE_ID::Title) 
                sce = new SceneTitle();
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


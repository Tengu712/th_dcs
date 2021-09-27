#include "../include/HeaderApp.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    GameInf ginf = GameInf();

    {
        bool flg = true;

        HMODULE hModule = LoadLibrary("./resource.dll");
        if (!hModule) {
            MessageBoxA(nullptr, "resource.dll not found.", "Error", MB_OK);
            return 1;
        }

        flg = flg && ginf.init(
                hInst,
                cmdShow,
                L"～ Darkside Connection of Skypunch",
                L"TH_DCS",
                1280U, 960U,
                MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO,
                hModule);

        // Load texture
        flg = flg && ginf.addTexture(hModule, TEX_UI_FRAME);

        // Load font
        flg = flg && ginf.addFont(Lpcstr2uint("0"));
        flg = flg && ginf.addFont(Lpcstr2uint("1"));
        flg = flg && ginf.addFont(Lpcstr2uint("2"));
        flg = flg && ginf.addFont(Lpcstr2uint("3"));
        flg = flg && ginf.addFont(Lpcstr2uint("4"));
        flg = flg && ginf.addFont(Lpcstr2uint("5"));
        flg = flg && ginf.addFont(Lpcstr2uint("6"));
        flg = flg && ginf.addFont(Lpcstr2uint("7"));
        flg = flg && ginf.addFont(Lpcstr2uint("8"));
        flg = flg && ginf.addFont(Lpcstr2uint("9"));
        flg = flg && ginf.addFont(Lpcstr2uint("."));
        flg = flg && ginf.addFont(Lpcstr2uint("f"));
        flg = flg && ginf.addFont(Lpcstr2uint("p"));
        flg = flg && ginf.addFont(Lpcstr2uint("s"));

        // Key map
        flg = flg && ginf.setKeyConfig();

        // Finish
        FreeLibrary(hModule);
        if (!flg) {
            MessageBoxA(nullptr, "Failed to load.", "Error", MB_OK);
            UnregisterClassW(L"TH_DCS", hInst);
            return 1;
        }
    }

    AScene* sce = new SceneTitle();

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


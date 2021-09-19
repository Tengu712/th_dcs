#include "../include/HeaderApp.hpp"
#include "../include/resource.hpp"

#include <time.h>
#pragma comment(lib, "Winmm.lib")

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    // Define constants
    LPCWSTR kNameWnd = L"Test DX11";
    LPCWSTR kNameWndClass = L"DX11_CLASS";
    const unsigned int kWidth = 1280;
    const unsigned int kHeight = 960U;
    const bool kWindowed = MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO;

    // Initialize DirectX11
    D3DManager dmanager = D3DManager();
    if (!dmanager.init(hInst, cmdShow, kNameWnd, kNameWndClass, kWidth, kHeight, kWindowed))
        return 1;

    InputManager imanager = InputManager();

    AudioManager amanager = AudioManager();
    if (!amanager.init()) {
        UnregisterClassW(kNameWndClass, hInst);
        return 1;
    }

    // Define general variants
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    Camera cameraUI = Camera();
    dmanager.createCamera(kWidth, kHeight, &cameraUI);
    cameraUI.posZ = -1200.0f;

    GameInf ginf = GameInf();

    // Load
    {
        int cnt = 0;
        bool flg = false;
        ModelSquare bgLoad = ModelSquare();
        while (true) {
            if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
                continue;
            }

            bool tmp = true;
            // Load screen
            if (cnt == 0) {
                tmp &= bgLoad.init(&dmanager);
                tmp &= ginf.addTexture(&dmanager, TEX_LOAD);
                bgLoad.posZ = -900.0f;
                bgLoad.sclX = 12.8f;
                bgLoad.sclY = 9.6f;
            }
            // Create Font
            if (cnt == 1) {
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("0"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("1"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("2"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("3"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("4"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("5"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("6"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("7"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("8"));
                tmp &= ginf.addFont(&dmanager, Lpcstr2uint("9"));
            }
            // Key map
            else if (cnt == 2) {
                tmp &= imanager.addKeycode(12, VK_RIGHT, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_DPAD_RIGHT);
            }
            // Finish
            else if (cnt == 100) {
                flg = true;
                break;
            }
            if (!tmp)
                break;
            dmanager.drawBegin();
            dmanager.applyCamera(&cameraUI, false);
            dmanager.applyTexture(ginf.getTexture(TEX_LOAD));
            dmanager.drawModel(&bgLoad);
            dmanager.drawEnd();
            cnt++;
        }

        if (!flg) {
            UnregisterClassW(kNameWndClass, hInst);
            return 1;
        }
    }

    // Define variants for main
    float fps = 0.0f;
    unsigned int cntFps = 0U;
    long startTime = timeGetTime();
    long lastTime = timeGetTime();

    ModelSquare modelsFps[3];
    for (int i = 0; i < 3; ++i) {
        modelsFps[i] = ModelSquare();
        modelsFps[i].init(&dmanager);
        modelsFps[i].posZ = -1000.0f;
        modelsFps[i].posX = 100.0f * (float)i;
    }

    // Main
    while (true) {
        if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            continue;
        }

        startTime = timeGetTime();
        if (startTime - lastTime >= 1000L){
            fps = (float)(cntFps * 1000U) / (float)(startTime - lastTime);
            lastTime = startTime;
            cntFps = 0U;
        }
        cntFps++;

        imanager.inspect();
        dmanager.drawBegin();
        dmanager.applyCamera(&cameraUI, false);

        for (int i = 0; i < 3; ++i) {
            dmanager.applyTexture(ginf.getFont(48U));
            dmanager.drawModel(&modelsFps[i]);
        }

        dmanager.drawEnd();
    }

    UnregisterClassW(kNameWndClass, hInst);
    return 0;
}


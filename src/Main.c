#include "../include/HeaderScene.h"

#include <time.h>
#pragma comment(lib, "Winmm.lib")

inline char ErrorExit(LPCSTR error, HINSTANCE hInst) {
        MessageBoxA(NULL, error, "Error", MB_OK);
        UnregisterClassW(L"TH_DCS", hInst);
        return 1;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {

    struct D3DInf dinf;
    if (!CreateD3DInf(
                &dinf,
                hInst,
                L"幻想異郷　～ Darkside Connection of Skypunch",
                L"TH_DCS",
                1280U, 960U,
                MessageBoxA(NULL, "フルスクリーンで起動しますか", "確認", MB_YESNO) == IDNO))
        return ErrorExit("Failed to create D3DInf.", hInst);

    struct InputInf iinf;
    CreateInputInf(&iinf);

    struct GameInf ginf;
    if (!CreateGameInf(&ginf, &dinf, &iinf, 1280U, 960U)) 
        return ErrorExit("Failed to create GameInf.", hInst);

    struct Infs infs;
    infs.pGinf = &ginf;
    infs.pDinf = &dinf;
    infs.pIinf = &iinf;

    long startTime = timeGetTime();
    long lastTime = timeGetTime();
    unsigned int cntFps = 0;

    char (*funcsInitScene[MAX_NUM_SCE])(struct Infs*);
    void (*funcsUpdateScene[MAX_NUM_SCE])(struct Infs*);
    for (int i = 0; i < MAX_NUM_SCE; ++i) {
        funcsInitScene[i] = InitMainMenu;
        funcsUpdateScene[i] = UpdateMainMenu;
    }
    funcsInitScene[SCE_Title] = InitTitle;
    funcsUpdateScene[SCE_Title] = UpdateTitle;
    funcsInitScene[SCE_Tutorial] = InitTutorial;
    funcsUpdateScene[SCE_Tutorial] = UpdateTutorial;

    MSG msg;
    memset(&msg, 0, sizeof(MSG));
    while (1) {
        if (PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            continue;
        }

        startTime = timeGetTime();
        if (startTime - lastTime >= 1000L){
            ginf.fps = (float)(cntFps * 1000) / (float)(startTime - lastTime);
            lastTime = startTime;
            cntFps = 0;
        }
        cntFps++;

        ginf.data.cntPlay++;
        InspectInput(&iinf);

        if (ginf.sceCur != ginf.sceNex) {
            if (ginf.sceNex == SCE_Exit)
                break;
            ginf.sceCur = ginf.sceNex;
            if (!funcsInitScene[ginf.sceCur](&infs))
                break;
        }
        
        funcsUpdateScene[ginf.sceCur](&infs);
    }

    FreeGameInf(&ginf);
    FreeInputInf(&iinf);
    FreeD3DInf(&dinf);
    UnregisterClassW(L"TH_DCS", hInst);
    return 0;
}


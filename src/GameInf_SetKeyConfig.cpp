#include "../include/HeaderApp.hpp"

#include <iostream>
#include <fstream>
#include <string>

bool GameInf::setKeyConfig() {

    bool res = true;
    const unsigned int kNumKey = 7U;

    std::ifstream ifs = std::ifstream("./keyconfig.cfg");
    if (!ifs.is_open()) {
        MessageBoxA(nullptr, "Failed to open keyconfig.cfg.", "Error", MB_OK);
        return false;
    }

    int cnt = 0;
    std::string buf;
    std::string bufs[kNumKey];
    while (cnt < kNumKey && std::getline(ifs, buf)) {
        if (buf != "up" && buf != "down" && buf != "left" && buf != "right"
                && buf != "start" && buf != "back"
                && buf != "l1" && buf != "l2" && buf != "l3"
                && buf != "r1" && buf != "r2" && buf != "r3"
                && buf != "a" && buf != "b" && buf != "x" && buf != "y"
                && buf != "ll" && buf != "lr" && buf != "lu" && buf != "ld"
                && buf != "rl" && buf != "rr" && buf != "ru" && buf != "rd") {
            MessageBoxA(nullptr, "Invalid key code in keyconfig.cfg.", "Error", MB_OK);
            return false;
        }
        bufs[cnt] = buf;
        cnt++;
    }

    ifs.close();

    if (cnt != kNumKey) {
        MessageBoxA(nullptr, "Length of keyconfig.cfg must be 7 lines.", "Error", MB_OK);
        return false;
    }

    GAMEPAD_KEYTYPE keytypes[kNumKey];
    short codes[kNumKey];

    for (int i = 0; i < kNumKey; ++i) {
        GAMEPAD_KEYTYPE tk = GAMEPAD_KEYTYPE::Buttons;
        short tc = XINPUT_GAMEPAD_DPAD_UP;
        if (bufs[i] == "up")
            tc = XINPUT_GAMEPAD_DPAD_UP;
        else if (bufs[i] == "down") 
            tc = XINPUT_GAMEPAD_DPAD_DOWN;
        else if (bufs[i] == "left")
            tc = XINPUT_GAMEPAD_DPAD_LEFT;
        else if (bufs[i] == "right")
            tc = XINPUT_GAMEPAD_DPAD_RIGHT;
        else if (bufs[i] == "start")
            tc = XINPUT_GAMEPAD_START;
        else if (bufs[i] == "back")
            tc = XINPUT_GAMEPAD_BACK;
        else if (bufs[i] == "l3")
            tc = XINPUT_GAMEPAD_LEFT_THUMB;
        else if (bufs[i] == "r3")
            tc = XINPUT_GAMEPAD_RIGHT_THUMB;
        else if (bufs[i] == "l1")
            tc = XINPUT_GAMEPAD_LEFT_SHOULDER;
        else if (bufs[i] == "r1")
            tc = XINPUT_GAMEPAD_RIGHT_SHOULDER;
        else if (bufs[i] == "a")
            tc = XINPUT_GAMEPAD_A;
        else if (bufs[i] == "b")
            tc = XINPUT_GAMEPAD_B;
        else if (bufs[i] == "x")
            tc = XINPUT_GAMEPAD_X;
        else if (bufs[i] == "y")
            tc = XINPUT_GAMEPAD_Y;
        else
            tc = 128;
        if (bufs[i] == "l2") 
            tk = GAMEPAD_KEYTYPE::LTrigger;
        else if (bufs[i] == "r2") 
            tk = GAMEPAD_KEYTYPE::RTrigger;
        else if (bufs[i] == "ll") 
            tk = GAMEPAD_KEYTYPE::ThumbLL;
        else if (bufs[i] == "lr") 
            tk = GAMEPAD_KEYTYPE::ThumbLR;
        else if (bufs[i] == "lu") 
            tk = GAMEPAD_KEYTYPE::ThumbLU;
        else if (bufs[i] == "ld") 
            tk = GAMEPAD_KEYTYPE::ThumbLD;
        else if (bufs[i] == "rl") 
            tk = GAMEPAD_KEYTYPE::ThumbRL;
        else if (bufs[i] == "rr") 
            tk = GAMEPAD_KEYTYPE::ThumbRR;
        else if (bufs[i] == "ru") 
            tk = GAMEPAD_KEYTYPE::ThumbRU;
        else if (bufs[i] == "rd") 
            tk = GAMEPAD_KEYTYPE::ThumbRD;
        keytypes[i] = tk;
        codes[i] = tc;
    }

    res = res && imanager.addKeycode(1, VK_UP, keytypes[0], codes[0]);
    res = res && imanager.addKeycode(2, VK_DOWN, keytypes[1], codes[1]);
    res = res && imanager.addKeycode(3, VK_LEFT, keytypes[2], codes[2]);
    res = res && imanager.addKeycode(4, VK_RIGHT, keytypes[3], codes[3]);
    res = res && imanager.addKeycode(5, 0x5A, keytypes[4], codes[4]);
    res = res && imanager.addKeycode(6, VK_LSHIFT, keytypes[5], codes[5]);
    res = res && imanager.addKeycode(7, VK_ESCAPE, keytypes[6], codes[6]);

    return res;
}

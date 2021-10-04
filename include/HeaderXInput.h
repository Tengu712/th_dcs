#pragma once
#ifndef _HEADER_XINPUT_H_
#define _HEADER_XINPUT_H_

#include <windows.h>
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

#define MD_GAMEPAD_KEYTYPE_BUTTONS 1
#define MD_GAMEPAD_KEYTYPE_LTRIGGER 2
#define MD_GAMEPAD_KEYTYPE_RTRIGGER 3
#define MD_GAMEPAD_KEYTYPE_THUMBLL 4
#define MD_GAMEPAD_KEYTYPE_THUMBLR 5
#define MD_GAMEPAD_KEYTYPE_THUMBLU 6
#define MD_GAMEPAD_KEYTYPE_THUMBLD 7
#define MD_GAMEPAD_KEYTYPE_THUMBRL 8
#define MD_GAMEPAD_KEYTYPE_THUMBRR 9
#define MD_GAMEPAD_KEYTYPE_THUMBRU 10
#define MD_GAMEPAD_KEYTYPE_THUMBRD 11

struct KeyInf {
    char state;
    char codeKey;
    char codeKeyboard;
    char typeGamepadKey;
    short codeGamepad;
};

struct InputInf {
    int numRegistered;
    char* statePrev;
    struct KeyInf* inf;
};

void CreateInputInf(struct InputInf* pIinf);
void FreeInputInf(struct InputInf* pIinf);

BOOL AddKeycode(struct InputInf* pIinf, char codeKey, char codeKeyboard, char typeGamepadKey, short codeGamepad);
void InspectInput(struct InputInf* pIinf);
char GetKey(struct InputInf* pIinf, char codeKey);

#endif

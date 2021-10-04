#include "../include/HeaderXInput.h"

#define MD_MAX_KEY 64

void CreateInputInf(struct InputInf* pIinf) {
    pIinf->numRegistered = 0;
    pIinf->statePrev = (char*)malloc(MD_MAX_KEY);
    pIinf->inf = (struct KeyInf*)malloc(MD_MAX_KEY);
}

void FreeInputInf(struct InputInf* pIinf) {
    if (pIinf->statePrev != NULL) {
        free(pIinf->statePrev);
        pIinf->statePrev = NULL;
    }
    if (pIinf->inf != NULL) {
        free(pIinf->inf);
        pIinf->inf = NULL;
    }
}

BOOL AddKeycode(struct InputInf* pIinf, char codeKey, char codeKeyboard, char typeGamepadKey, short codeGamepad) {
    if (pIinf->numRegistered >= 64)
        return FALSE;
    struct KeyInf tmp = {
        0, codeKey, codeKeyboard, typeGamepadKey, codeGamepad
    };
    pIinf->inf[pIinf->numRegistered] = tmp;
    pIinf->numRegistered++;
    return TRUE;
}

void InspectInput(struct InputInf* pIinf) {
    XINPUT_STATE stateXInp;
    DWORD dwRes = XInputGetState(0, &stateXInp);
    for (int i = 0; i < pIinf->numRegistered; ++i) {
        BOOL flagKB = GetAsyncKeyState(pIinf->inf[i].codeKeyboard) & 0x8000;
        BOOL flagXI = FALSE;
        if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_BUTTONS)
            flagXI = stateXInp.Gamepad.wButtons & pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_LTRIGGER)
            flagXI = stateXInp.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_RTRIGGER)
            flagXI = stateXInp.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBLL)
            flagXI = stateXInp.Gamepad.sThumbLX < -pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBLR)
            flagXI = stateXInp.Gamepad.sThumbLX > pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBLU)
            flagXI = stateXInp.Gamepad.sThumbLY < -pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBLD)
            flagXI = stateXInp.Gamepad.sThumbLY > pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBRL)
            flagXI = stateXInp.Gamepad.sThumbRX < -pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBRR)
            flagXI = stateXInp.Gamepad.sThumbRX > pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBRU)
            flagXI = stateXInp.Gamepad.sThumbRY < -pIinf->inf[i].codeGamepad;
        else if (pIinf->inf[i].typeGamepadKey == MD_GAMEPAD_KEYTYPE_THUMBRD)
            flagXI = stateXInp.Gamepad.sThumbRY > pIinf->inf[i].codeGamepad;
        if (dwRes != ERROR_SUCCESS)
            flagXI = FALSE;
        if (flagKB || flagXI)
            pIinf->inf[i].state = ((pIinf->statePrev[i] << 1) ^ 0b011) & 0b011;
        else
            pIinf->inf[i].state = (pIinf->statePrev[i] << 2) & 0b100;
        pIinf->statePrev[i] = pIinf->inf[i].state;
    }
}

char GetKey(struct InputInf* pIinf, char codeKey) {
    for (int i = 0; i < pIinf->numRegistered; ++i) {
        if (pIinf->inf[i].codeKey == codeKey)
            return pIinf->inf[i].state;
    }
    return 0;
}
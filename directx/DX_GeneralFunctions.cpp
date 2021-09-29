#include "../include/HeaderDX11.hpp"
#include <stdio.h>

void DebugBox(int i, const char* title) {
    char buff[64] = "";
    sprintf(buff, "%d", i);
    MessageBoxA(nullptr, buff, title, MB_OK);
}

void DebugBox(const char* str, const char* title){
    MessageBoxA(nullptr, str, title, MB_OK);
}

unsigned int Lpcstr2uint(LPCSTR c) {
    if (IsDBCSLeadByte(*c))
        return (unsigned char)c[0] << 8 | (unsigned char)c [1];    
    else
        return c[0];
}

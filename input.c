#include "data.h"
#include "input.h"
#include <windows.h>

void update_input(InputState* input) {
    input->w = GetAsyncKeyState('W') & 0x8000;
    input->a = GetAsyncKeyState('A') & 0x8000;
    input->s = GetAsyncKeyState('S') & 0x8000;
    input->d = GetAsyncKeyState('D') & 0x8000;
    input->q = GetAsyncKeyState('Q') & 0x8000;
    input->e = GetAsyncKeyState('E') & 0x8000;
}


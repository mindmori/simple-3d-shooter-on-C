#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "renderer.h"
#include "const.h"

char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "################",
    "#..............#",
    "#.......########",
    "#..............#",
    "#......##......#",
    "#......##......#",
    "#..............#",
    "###............#",
    "##.............#",
    "#......####..###",
    "#......#.......#",
    "#......#.......#",
    "#..............#",
    "#......#########",
    "#..............#",
    "################"
};

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hBuffer);

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
    DWORD dwBytesWritten = 0;

    float playerAngle = 0.0f;
    float playerX = 7.0f;
    float playerY = 7.0f;

    while (1) {
        playerAngle += M_PI / (36 * 60);

        render_frame(screen, playerAngle, playerX, playerY);

        char outputBuffer[SCREEN_HEIGHT * (SCREEN_WIDTH + 2)];
        int pos = 0, i;
        for (i = 0; i < SCREEN_HEIGHT; i++) {
            memcpy(&outputBuffer[pos], screen[i], SCREEN_WIDTH);
            pos += SCREEN_WIDTH;
            outputBuffer[pos++] = '\r';
            outputBuffer[pos++] = '\n';
        }

        WriteConsoleOutputCharacterA(hBuffer, outputBuffer, pos, (COORD){0, 0}, &dwBytesWritten);
        Sleep(FRAME_TIME_MS);
    }

    return 0;
}


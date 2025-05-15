#include <stdio.h>
#include <windows.h>
#include <math.h>

#define _USE_MATH_DEFINES
#define FPS 60
#define FRAME_TIME_MS (1000 / FPS)
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define FOV (M_PI / 4.0)
#define MAX_DEPTH 16.0f
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

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

void render_frame(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], float playerAngle, float playerX, float playerY) {
	int x;
    for (x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = playerAngle - FOV / 2.0f + ((float)x / (float)SCREEN_WIDTH) * FOV;

        float distanceToWall = 0;
        int hitWall = 0;

        float eyeX = cosf(rayAngle);
        float eyeY = sinf(rayAngle);

        while (!hitWall && distanceToWall < MAX_DEPTH) {
            distanceToWall += 0.1f;
            int testX = (int)(playerX + eyeX * distanceToWall);
            int testY = (int)(playerY + eyeY * distanceToWall);

            if (testX >= 0 && testX < MAP_WIDTH && testY >= 0 && testY < MAP_HEIGHT) {
                if (map[testY][testX] == '#') {
                    hitWall = 1;
                }
            } else {
                hitWall = 1;
            }
        }

        int ceiling = (float)(SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / ((float)distanceToWall);
        int floor = SCREEN_HEIGHT - ceiling, y;
        for (y = 0; y < SCREEN_HEIGHT; y++) {
            if (y < ceiling)
                screen[y][x] = ' ';
            else if (y > ceiling && y <= floor)
                screen[y][x] = '#';
            else
                screen[y][x] = '.';
        }
    }
	int y;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
        screen[y][SCREEN_WIDTH] = '\0';
    }
}

int main() {
    // Создаём буфер для вывода
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

        // Объединяем строки в один буфер вывода
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


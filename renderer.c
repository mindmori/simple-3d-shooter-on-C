#include "renderer.h"
#include "const.h"
#include <math.h>

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

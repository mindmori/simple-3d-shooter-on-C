#include "const.h"

#ifndef RENDERER_H
#define RENDERER_H

extern char map[MAP_HEIGHT][MAP_WIDTH + 1];

void render_frame(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], float playerAngle, float playerX, float playerY);

#endif

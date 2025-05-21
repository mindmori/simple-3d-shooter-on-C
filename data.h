#include <math.h>

#ifndef CONST_H
#define CONST_H

#define FPS 60
#define FRAME_TIME_MS (1000.0f / FPS)
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define FOV (M_PI / 4.0f)
#define MAX_DEPTH 16.0f
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30
#define ENEMY_AMOUNT 3

typedef struct{
	float x;
	float y;
} Vector2;

typedef struct{
	float x;
	float y;
	float z;
} Vector3;


#endif

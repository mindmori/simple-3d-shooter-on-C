#include <math.h>

#ifndef DATA_H
#define DATA_H

#define FPS 60
#define FRAME_TIME_MS (1000.0f / FPS)
#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define FOV (M_PI / 4.0f)
#define MAX_DEPTH 16.0f
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

#define ENEMY_AMOUNT 3
#define ENEMY_VIEW_DISTANCE 10
#define ROT_SPEED 2.0f
#define PLAYER_MS 0.1f
#define ENEMY_MS 0.01f
#define ENEMY_KILL_DIST 0.05f
#define SHOOT_RANGE 0.3f

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

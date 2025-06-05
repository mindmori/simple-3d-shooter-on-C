#include <math.h>

#ifndef DATA_H
#define DATA_H

#define GAME_NAME "SIMPLE SHOOTER ON C"
#define FPS 60
#define MENU_CD_MS 100.0f
#define FRAME_TIME_MS (1000.0f / FPS)
#define DELTA_TIME 1.0f / FPS
#define FOV (M_PI / 4.0f)
#define MAX_DEPTH 16.0f

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

#define FIRE_CD_MS 1000.0f
#define ENEMY_AMOUNT 3
#define ENEMY_MIN_RESPAWN_DIST 5
#define ENEMY_VIEW_DISTANCE 5
#define ROT_SPEED 2.0f
#define PLAYER_MS 0.1f
#define ENEMY_MS 0.01f
#define ENEMY_KILL_DIST 0.1f
#define SHOOT_RANGE 0.3f

#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY

typedef struct{
	float x;
	float y;
} Vector2;

typedef enum {
	INIT,
	REINIT,
	MENU,
	RUNNING,
	DEAD
} GameState;

typedef struct {
	Vector2 pos;
	float angle;
	int score;
    float cooldown;
} Player;


#endif

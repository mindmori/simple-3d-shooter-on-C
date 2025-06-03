#include "data.h"
#include <windows.h>

#ifndef RENDERER_H
#define RENDERER_H

static void resize_console(Screen* screen);

void screen_init(Screen* screen);

//void render_frame(
//	const HANDLE buffer, 
//	const Vector3 player, 
//	const char map[MAP_HEIGHT][MAP_WIDTH],
//	const Vector2 enemy[ENEMY_AMOUNT]
//);

void render_menu(
	Screen* screen,
	const int score,
	const GameState state,
	int* menuState,
	InputState* input
);
#endif

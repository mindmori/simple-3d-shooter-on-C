#include "data.h"
#include <windows.h>
#include "gamemap.h"

#ifndef RENDERER_H
#define RENDERER_H

void render_frame(
	const HANDLE buffer, 
	const Vector3 player, 
	GameMap map,
	const Vector2 enemy[ENEMY_AMOUNT]
);

#endif

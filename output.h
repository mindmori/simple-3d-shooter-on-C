#include "data.h"
#include <windows.h>

#ifndef RENDERER_H
#define RENDERER_H

void render_frame(
	const HANDLE buffer, 
	const Vector3 player, 
	const char map[MAP_HEIGHT][MAP_WIDTH]
);

#endif

#include "gamemap.h"
#include "data.h"

#ifndef GAME_H
#define GAME_H

typedef struct {
	Vector2 pos;
	float angle;
	int score;
} Player;

void initializeGame(
	GameMap *map, 
	Vector3 *player, 
	Vector2 enemies[ENEMY_AMOUNT]
);

void updateGame(
    GameMap *map,
    Vector3 *player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState *input,
    float deltaTime,
    GameState* state
);

#endif

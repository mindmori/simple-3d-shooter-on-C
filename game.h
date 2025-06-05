#include "gamemap.h"
#include "data.h"

#ifndef GAME_H
#define GAME_H

void initializeGame(
	GameMap *map, 
	Player *player, 
	Vector2 enemies[ENEMY_AMOUNT]
);

void updateGame(
    GameMap *map,
    Player *player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState *input,
    GameState* state
);

#endif

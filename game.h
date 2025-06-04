#ifndef GAME_H
#define GAME_H

void initializeGame(GameMap *map, Vector3 *player, Vector2 enemies[ENEMY_AMOUNT]);
void updateGame(
    GameMap *map,
    Vector3 *player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState *input,
    float deltaTime);

#endif
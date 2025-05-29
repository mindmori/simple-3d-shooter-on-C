#ifndef GAME_H
#define GAME_H

void initializeGame(const char map[MAP_HEIGHT][MAP_WIDTH], Vector3* player, Vector2 enemies[ENEMY_AMOUNT]);
void updateGame(
    const char map[MAP_HEIGHT][MAP_WIDTH],
    Vector3* player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState* input,
    float deltaTime
);


#endif
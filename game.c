#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "data.h"
#include "input.h"
#include "game.h"
#include "gamemap.h"

int canMove(const GameMap *map, float x, float y)
{
    int mapX = (int)x;
    int mapY = (int)y;

    if (mapX < 0 || mapX >= map->width || mapY < 0 || mapY >= map->height)
    {
        return 0;
    }

    return map->data[mapY][mapX] != '#';
}

void respawnEnemy(const GameMap *map, Vector2 *enemy)
{
    do
    {
        enemy->x = 2.0f + rand() % (map->width - 4);
        enemy->y = 2.0f + rand() % (map->height - 4);
    } while (!canMove(map, enemy->x, enemy->y));
}

void initializeGame(GameMap *map, Vector3 *player, Vector2 enemies[ENEMY_AMOUNT])
{
    player->x = 1.5f;
    player->y = 1.5f;
    player->z = 0.0f;

    srand(time(NULL));
    for (int i = 0; i < ENEMY_AMOUNT; i++)
    {
        respawnEnemy(map, &(enemies[i]));
    }
}

void rotatePlayer(Vector3 *player, float angle)
{
    player->z += angle;
    player->z = fmod(player->z, 2 * M_PI);
    if (player->z <= 0)
    {
        player->z += 2 * M_PI;
    }
}

void movePlayerInDirection(const GameMap *map, Vector3 *player, int direction)
{
    float newX = player->x + direction * cos(player->z) * PLAYER_MS;
    float newY = player->y + direction * sin(player->z) * PLAYER_MS;
    if (canMove(map, newX, player->y))
    {
        player->x = newX;
    }
    if (canMove(map, player->x, newY))
    {
        player->y = newY;
    }
}

void updateEnemies(const GameMap *map, Vector3 *player, Vector2 enemies[ENEMY_AMOUNT], GameState* state)
{
    for (int i = 0; i < ENEMY_AMOUNT; i++)
    {
        float dx = player->x - enemies[i].x;
        float dy = player->y - enemies[i].y;
        float dist = sqrt(pow(dx, 2) + pow(dy, 2));

        if (dist < ENEMY_KILL_DIST)
        {
            *state = REINIT;
            return;
        }

        if (dist < ENEMY_VIEW_DISTANCE)
        {
            float newX = enemies[i].x + dx / dist * ENEMY_MS;
            float newY = enemies[i].y + dy / dist * ENEMY_MS;

            if (canMove(map, newX, enemies[i].y))
                enemies[i].x = newX;
            if (canMove(map, enemies[i].x, newY))
                enemies[i].y = newY;
        }
    }
}

void fire(const GameMap *map, Vector3 *player, Vector2 enemies[ENEMY_AMOUNT])
{
    float distanceToWall = 0;
    int hitWall = 0;
    int hitEnemy = 0;

    float eyeX = cosf(player->z);
    float eyeY = sinf(player->z);

    while (!hitEnemy && !hitWall && distanceToWall < MAX_DEPTH)
    {
        distanceToWall += 0.1f;
        float testfX = player->x + eyeX * distanceToWall;
        float testfY = player->y + eyeY * distanceToWall;
        int testX = (int)testfX;
        int testY = (int)testfY;

        if (testX >= 0 && testX < map->width && testY >= 0 && testY < map->height)
        {
            for (int i = 0; i < ENEMY_AMOUNT; i++)
            {
                float dx = testfX - enemies[i].x;
                float dy = testfY - enemies[i].y;
                float distToEnemy = sqrtf(dx * dx + dy * dy);

                if (distToEnemy < SHOOT_RANGE)
                {
                    hitEnemy = 1;
                    respawnEnemy(map, &(enemies[i]));
                    break;
                }
            }

            if (map->data[testY][testX] == '#')
            {
                hitWall = 1;
            }
        }
        else
        {
            hitWall = 1;
        }
    }
}

void updateGame(
    GameMap *map,
    Vector3 *player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState *input,
    float deltaTime,
	GameState* state)
{
    if (input->w)
    {
        movePlayerInDirection(map, player, 1);
    }
    if (input->s)
    {
        movePlayerInDirection(map, player, -1);
    }
    if (input->a)
    {
        rotatePlayer(player, -ROT_SPEED * deltaTime);
    }
    if (input->d)
    {
        rotatePlayer(player, ROT_SPEED * deltaTime);
    }
    if (input->e)
    {
        fire(map, player, enemies);
    }

    updateEnemies(map, player, enemies, state);
}

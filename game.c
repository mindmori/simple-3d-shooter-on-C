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

double calculateDistanceToPlayer(Player *player, Vector2 enemy)
{
    float dx = player->pos.x - enemy.x;
    float dy = player->pos.y - enemy.y;
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

void respawnEnemy(const GameMap *map, Vector2 *enemy, Player *player)
{
    do
    {
        enemy->x = 2.0f + rand() % (map->width - 4);
        enemy->y = 2.0f + rand() % (map->height - 4);
    } while (!canMove(map, enemy->x, enemy->y) || (calculateDistanceToPlayer(player, *enemy) < map->width / 3));
}

void initializeGame(GameMap *map, Player *player, Vector2 enemies[ENEMY_AMOUNT])
{
    player->pos.x = 1.5f;
    player->pos.y = 1.5f;
    player->angle = 0.0f;
    player->score = 0;

    srand(time(NULL));
    for (int i = 0; i < ENEMY_AMOUNT; i++)
    {
        respawnEnemy(map, &(enemies[i]), player);
    }
}

void rotatePlayer(Player *player, float angle)
{
    player->angle += angle;
    player->angle = fmod(player->angle, 2 * M_PI);
    if (player->angle <= 0)
    {
        player->angle += 2 * M_PI;
    }
}

void movePlayerInDirection(const GameMap *map, Player *player, int direction)
{
    float newX = player->pos.x + direction * cos(player->angle) * PLAYER_MS;
    float newY = player->pos.y + direction * sin(player->angle) * PLAYER_MS;
    if (canMove(map, newX, player->pos.y))
    {
        player->pos.x = newX;
    }
    if (canMove(map, player->pos.x, newY))
    {
        player->pos.y = newY;
    }
}


void updateEnemies(const GameMap *map, Player *player, Vector2 enemies[ENEMY_AMOUNT], GameState *state)
{
    for (int i = 0; i < ENEMY_AMOUNT; i++)
    {
        float dx = player->pos.x - enemies[i].x;
        float dy = player->pos.y - enemies[i].y;
        float dist = calculateDistanceToPlayer(player, enemies[i]);

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

void fire(const GameMap *map, Player *player, Vector2 enemies[ENEMY_AMOUNT])
{
    float distanceToWall = 0;
    int hitWall = 0;
    int hitEnemy = 0;

    float eyeX = cosf(player->angle);
    float eyeY = sinf(player->angle);

    while (!hitEnemy && !hitWall && distanceToWall < MAX_DEPTH)
    {
        distanceToWall += 0.1f;
        float testfX = player->pos.x + eyeX * distanceToWall;
        float testfY = player->pos.y + eyeY * distanceToWall;
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
                    player->score++;
                    respawnEnemy(map, &(enemies[i]), player);
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
    Player *player,
    Vector2 enemies[ENEMY_AMOUNT],
    const InputState *input,
    GameState *state)
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
        rotatePlayer(player, -ROT_SPEED * DELTA_TIME);
    }
    if (input->d)
    {
        rotatePlayer(player, ROT_SPEED * DELTA_TIME);
    }
    if (input->e && player->cooldown <= 0)
    {
        fire(map, player, enemies);
        player->cooldown = FIRE_CD_MS;
    }

    updateEnemies(map, player, enemies, state);
}

#include "output.h"
#include "data.h"
#include <math.h>
#include <windows.h>

static void render_screen(
	CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH],
	const Vector3 player,
	const char map[MAP_HEIGHT][MAP_WIDTH],
	const Vector2 enemy[ENEMY_AMOUNT]
){
	int x, y;
	for (x = 0; x < SCREEN_WIDTH; x++) {
    	int hitWall = 0;
    	int hitEnemy = 0;
        float rayAngle = player.z - FOV / 2.0f + ((float)x / (float)SCREEN_WIDTH) * FOV;
        float distanceToWall = 0;
        float eyeX = cosf(rayAngle);
        float eyeY = sinf(rayAngle);
        
        while (!hitWall && distanceToWall < MAX_DEPTH) {
            distanceToWall += 0.1f;
            float testXf = player.x + eyeX * distanceToWall;
    		float testYf = player.y + eyeY * distanceToWall;
    		int testX = (int)testXf;
    		int testY = (int)testYf;

            if (testX >= 0 && testX < MAP_WIDTH && testY >= 0 && testY < MAP_HEIGHT) {
                if (map[testY][testX] == '#') {
                    hitWall = 1;
                }
            } else {
                hitWall = 1;
            }
            
            int i;
            for (i = 0; i < ENEMY_AMOUNT; i++){
            	float distToEnemy = sqrtf((enemy[i].x - testXf) * (enemy[i].x - testXf) + (enemy[i].y - testYf) * (enemy[i].y - testYf));
    			if (distToEnemy < 0.2f){
        			hitEnemy = 1;
        			hitWall = 1;
					break;
    			}
			}
        }

        int ceiling = (float)(SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / ((float)distanceToWall);
        int floor = SCREEN_HEIGHT - ceiling;
        for (y = 0; y < SCREEN_HEIGHT; y++) {
    		if (hitEnemy && y > ceiling && y <= floor) {
				screen[y][x].Char.AsciiChar = 'E';
        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
    		} else {
        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        		if (y <= ceiling)
            		screen[y][x].Char.AsciiChar = ' ';
        		else if (y > ceiling && y <= floor)
            		screen[y][x].Char.AsciiChar = '#';
        		else
            		screen[y][x].Char.AsciiChar = '.';
    		}
		}
    }
}

static void print_to_buffer(
	const HANDLE hBuffer, 
	const CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH]
){
	COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
	COORD dwBufferCoord = {0, 0};
	SMALL_RECT writeRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
	WriteConsoleOutput(hBuffer, (CHAR_INFO*)screen, dwBufferSize, dwBufferCoord, &writeRegion);
}

void render_frame(const HANDLE hBuffer, const Vector3 player, const char map[MAP_HEIGHT][MAP_WIDTH], const Vector2 enemy[ENEMY_AMOUNT]) {
	
	CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH];
	render_screen(screen, player, map, enemy);
    print_to_buffer(hBuffer, screen);
}

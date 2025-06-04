#include "output.h"
#include "data.h"
#include <math.h>
#include <windows.h>
#include "gamemap.h"

static int checkWall(
	const int x,
	const int y,
	const GameMap map
){
	if (x <= 0 || x > map.width || y <= 0 || y > map.height)
		return 1;
	
	if (map.data[y][x] == '#') {
		return 1;
    }
    
    return 0;
}

static int checkEnemy(
	const float x,
	const float y,
	const Vector2 enemy[ENEMY_AMOUNT]
){
	int i;
	for (i = 0; i < ENEMY_AMOUNT; i++){
		float distToEnemy = sqrtf((enemy[i].x - x) * (enemy[i].x - x) + (enemy[i].y - y) * (enemy[i].y - y));
		if (distToEnemy < 0.2f){
			return 1;
		}
	}
	return 0;
}

static void render_screen(
	CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH],
	const Vector3 player,
	const GameMap map,
	const Vector2 enemy[ENEMY_AMOUNT]
){
	int x, y;
	for (x = 0; x < SCREEN_WIDTH; x++) {
    	int hitWall = 0;
    	int hitEnemy = 0;
        float rayAngle = player.z - FOV / 2.0f + ((float)x / (float)SCREEN_WIDTH) * FOV;
        float distance = 0.0f;
        float eyeX = cosf(rayAngle);
        float eyeY = sinf(rayAngle);
        
        
        float testXf, testYf;
        int testX, testY;
        while (!hitWall && !hitEnemy && distance < MAX_DEPTH){
        	distance += 0.1f;
        	testXf = player.x + eyeX * distance;
    		testYf = player.y + eyeY * distance;
    		testX = (int)testXf;
    		testY = (int)testYf;
    		
    		hitWall = checkWall(testX, testY, map);
    		hitEnemy = checkEnemy(testXf, testYf, enemy);
		}
		
        int ceiling = (float)(SCREEN_HEIGHT / 2.0f) - SCREEN_HEIGHT / ((float)distance);
        int floor = SCREEN_HEIGHT - ceiling;
        for (y = 0; y < SCREEN_HEIGHT; y++) {
        	if (y <= ceiling || y >= floor || (!hitWall && !hitEnemy) ){
        			screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            		screen[y][x].Char.AsciiChar = ' ';
            		continue;
			}
        	
        	if(hitWall){
        		char wallStr[] = "@#?=+~:.";
        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				screen[y][x].Char.AsciiChar = wallStr[ (int)(distance * 0.5f) ];
				continue;
			}
        	
    		if (hitEnemy) {
        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				screen[y][x].Char.AsciiChar = 'E';
				continue;
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

void render_frame(
	const HANDLE hBuffer, 
	const Vector3 player, 
	GameMap map,
	const Vector2 enemy[ENEMY_AMOUNT]
){
	CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH];
	render_screen(screen, player, map, enemy);
    print_to_buffer(hBuffer, screen);
}

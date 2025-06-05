#include "output.h"
#include "data.h"
#include <math.h>
#include <windows.h>
#include "gamemap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int initConsoleBuffer(ConsoleBuffer* cb, SHORT width, SHORT height) {
    cb->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cb->size.X = width;
    cb->size.Y = height;
    cb->region = (SMALL_RECT){0, 0, width - 1, height - 1};
    cb->activeIndex = 0;
    
	int i;
    for (i = 0; i < 2; ++i) {
        cb->buffers[i] = (CHAR_INFO*)malloc(width * height * sizeof(CHAR_INFO));
        if (!cb->buffers[i]) return 0;
    }

    return 1;
}

static void setCharAt(ConsoleBuffer* cb, int x, int y, char ch, WORD attr) {
    if (x < 0 || y < 0 || x >= cb->size.X || y >= cb->size.Y)
        return;

    CHAR_INFO* active = cb->buffers[cb->activeIndex];
    int index = y * cb->size.X + x;

    active[index].Char.AsciiChar = ch;
    active[index].Attributes = attr;
}

void clearBuffer(ConsoleBuffer* cb, CHAR c, WORD attr) {
	CHAR_INFO* buf = cb->buffers[cb->activeIndex];
    int i;
    for (i = 0; i < cb->size.X * cb->size.Y; ++i) {
        buf[i].Char.AsciiChar = c;
        buf[i].Attributes = attr;
    }
    
    buf = cb->buffers[1 - cb->activeIndex];
    for (i = 0; i < cb->size.X * cb->size.Y; ++i) {
        buf[i].Char.AsciiChar = c;
        buf[i].Attributes = attr;
    }
}

void overlayText(ConsoleBuffer* cb, const char* text, COORD pos, WORD attr) {
    CHAR_INFO* buf = cb->buffers[1 - cb->activeIndex];

    int x = pos.X;
    int y = pos.Y;
    if (y < 0 || y >= cb->size.Y || x >= cb->size.X)
        return;

	int i;
    for (i = 0; text[i] != '\0'; ++i) {
        int px = x + i;
        if (px >= 0 && px < cb->size.X) {
            int idx = y * cb->size.X + px;
            buf[idx].Char.AsciiChar = text[i];
            buf[idx].Attributes = attr;
        }
    }
}

static void renderBuffer(ConsoleBuffer* cb) {
    WriteConsoleOutput(
        cb->hConsole,
        cb->buffers[1 - cb->activeIndex],
        cb->size,
        (COORD){0, 0},
        &cb->region
    );
    cb->activeIndex = 1 - cb->activeIndex;
}

void freeConsoleBuffer(ConsoleBuffer* cb) {
	int i;
    for (i = 0; i < 2; ++i) {
        if (cb->buffers[i]) free(cb->buffers[i]);
    }
}

static int resizeToWindow(ConsoleBuffer* cb) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(cb->hConsole, &csbi)) {
        return 0;
    }

    SHORT newWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    SHORT newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;

    if (cb->size.X == newWidth && cb->size.Y == newHeight) {
        return 1;
    }

    for (int i = 0; i < 2; ++i) {
        free(cb->buffers[i]);
        cb->buffers[i] = NULL;
    }

    cb->size.X = newWidth;
    cb->size.Y = newHeight;
    cb->region = (SMALL_RECT){0, 0, newWidth - 1, newHeight - 1};

    for (int i = 0; i < 2; ++i) {
        cb->buffers[i] = (CHAR_INFO*)malloc(newWidth * newHeight * sizeof(CHAR_INFO));
        if (!cb->buffers[i]) return 0;
    }

    return 1;
}

static int checkWall(int x, int y, GameMap map) {
	if (x <= 0 || x > map.width || y <= 0 || y > map.height)
		return 1;
	
	if (map.data[y][x] == '#') {
		return 1;
    }
    
    return 0;
}

static int checkEnemy(float x, float y, Vector2* enemy) {
	int i;
	for (i = 0; i < ENEMY_AMOUNT; i++){
		float distToEnemy = sqrtf((enemy[i].x - x) * (enemy[i].x - x) + (enemy[i].y - y) * (enemy[i].y - y));
		if (distToEnemy < 0.2f){
			return 1;
		}
	}
	return 0;
}

static void render_game_frame(ConsoleBuffer* cb, Vector2 pos, float angle, GameMap map, Vector2* enemy){
	int x, y;
	int width = cb->size.X;
	int height = cb->size.Y;
	for (x = 0; x < width; x++) {
    	int hitWall = 0;
    	int hitEnemy = 0;
        float rayAngle = angle - FOV / 2.0f + ((float)x / (float)width) * FOV;
        float distance = 0.0f;
        float eyeX = cosf(rayAngle);
        float eyeY = sinf(rayAngle);
        
        
        float testXf, testYf;
        int testX, testY;
        while (!hitWall && !hitEnemy && distance < MAX_DEPTH){
        	distance += 0.1f;
        	testXf = pos.x + eyeX * distance;
    		testYf = pos.y + eyeY * distance;
    		testX = (int)testXf;
    		testY = (int)testYf;
    		
    		hitWall = checkWall(testX, testY, map);
    		hitEnemy = checkEnemy(testXf, testYf, enemy);
		}
		
        int ceiling = (float)(height / 2.0f) - height / ((float)distance);
        int floor = height - ceiling;
        for (y = 0; y < height; y++) {
        	if (y <= ceiling || y >= floor || (!hitWall && !hitEnemy) ){
        			setCharAt(cb, x, y, ' ', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            		continue;
			}
        	
        	if(hitWall){
        		char wallStr[] = "@#?=+~:.";
        		setCharAt(cb, x, y, wallStr[(int)(distance * 8 / MAX_DEPTH )], FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				continue;
			}
        	
    		if (hitEnemy) {
    			setCharAt(cb, x, y, 'E', FOREGROUND_RED | FOREGROUND_INTENSITY);
				continue;
    		}
		}
    }
}

void render_frame(ConsoleBuffer* cb, Vector2 pos, float angle, int score, GameMap map, Vector2* enemy){
	resizeToWindow(cb);
	render_game_frame(cb, pos, angle, map, enemy);
	char scoreStr[16];
	snprintf(scoreStr, sizeof(scoreStr), "Score: %d", score);
//	char timeStr[8];
//	snprintf(timeStr, sizeof(timeStr), "%d", time);
	overlayText(cb, scoreStr, (COORD){0, 0}, FOREGROUND_RED | FOREGROUND_GREEN);
//	overlayText(cb, timeStr, (COORD){0, cb->size.Y - 1}, FOREGROUND_RED | FOREGROUND_GREEN);
	renderBuffer(cb);
}

void render_menu(ConsoleBuffer* cb, int score, int menuChoice){
	resizeToWindow(cb);
	clearBuffer(cb, ' ', WHITE);
	COORD pos = {cb->size.X / 2, cb->size.Y / 2};
	char playBtn[16];
	char exitBtn[16];
	WORD active = YELLOW;
	WORD inactive = WHITE;
	
	
	overlayText(cb, GAME_NAME, pos, YELLOW);
	pos.Y += 2;
	
	if (score >= 0){
		overlayText(cb, "YOU ARE DEAD!", pos, RED);
		pos.Y++;
		
		char scoreStr[16];
		snprintf(scoreStr, sizeof(scoreStr), "Score: %d", score);
		overlayText(cb, scoreStr, pos, WHITE);
		pos.Y += 2;
		
		sprintf(playBtn, "%s%s", !menuChoice ? "->" : "  ", "Try again");
		overlayText(cb, playBtn, pos, !menuChoice ? active : inactive);
		pos.Y++;
	}
	else {
		sprintf(playBtn, "%s%s", !menuChoice ? "->" : "  ", "Play");
		overlayText(cb, playBtn, pos, !menuChoice ? active : inactive);
		pos.Y++;
	}
	
	sprintf(playBtn, "%s%s", menuChoice ? "->" : "  ", "Quit");
	overlayText(cb, playBtn, pos, menuChoice ? active : inactive);
	
	pos.Y = cb->size.Y - 3;
	overlayText(cb, "CREATED BY", pos, YELLOW);
	pos.Y++;
	overlayText(cb, "mori", pos, WHITE);
	pos.Y++;
	overlayText(cb, "selivura", pos, WHITE);
	
	renderBuffer(cb);
}

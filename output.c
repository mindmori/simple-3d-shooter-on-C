#include "output.h"
#include "data.h"
#include <math.h>
#include <windows.h>

static void switch_buffer(Screen* screen){
	screen->back = screen->current;
    screen->current = 1 - screen->current;
    SetConsoleActiveScreenBuffer(screen->buffer[screen->current]);
}

static void resize_console(Screen* screen){
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(screen->buffer[screen->current], &csbi)){
		int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if ( (width != SCREEN_WIDTH) || (height != SCREEN_HEIGHT) ){
			SMALL_RECT rect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
			SetConsoleWindowInfo(screen->buffer[screen->current], TRUE, &rect);
		}
	}
}

void screen_init(Screen* screen){
    screen->current = 0;
    COORD size = {SCREEN_WIDTH, SCREEN_HEIGHT};
	screen->buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screen->buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(screen->buffer[0], size);
	SetConsoleScreenBufferSize(screen->buffer[1], size);
    SetConsoleActiveScreenBuffer(screen->buffer[0]);
}

//static void render_frame(
//	CHAR_INFO screen[SCREEN_HEIGHT][SCREEN_WIDTH],
//	const Vector2 pos,
//	const float angle,
//	const char map[MAP_HEIGHT][MAP_WIDTH],
//	const Vector2 enemy[ENEMY_AMOUNT]
//){
//	int x, y;
//	for (x = 0; x < SCREEN_WIDTH; x++) {
//    	int hitWall = 0;
//    	int hitEnemy = 0;
//        float rayAngle = player.z - FOV / 2.0f + ((float)x / (float)SCREEN_WIDTH) * FOV;
//        float distanceToWall = 0;
//        float eyeX = cosf(rayAngle);
//        float eyeY = sinf(rayAngle);
//        
//        while (!hitWall && distanceToWall < MAX_DEPTH) {
//            distanceToWall += 0.1f;
//            float testXf = player.x + eyeX * distanceToWall;
//    		float testYf = player.y + eyeY * distanceToWall;
//    		int testX = (int)testXf;
//    		int testY = (int)testYf;
//
//            if (testX >= 0 && testX < MAP_WIDTH && testY >= 0 && testY < MAP_HEIGHT) {
//                if (map[testY][testX] == '#') {
//                    hitWall = 1;
//                }
//            } else {
//                hitWall = 1;
//            }
//            
//            int i;
//            for (i = 0; i < ENEMY_AMOUNT; i++){
//            	float distToEnemy = sqrtf((enemy[i].x - testXf) * (enemy[i].x - testXf) + (enemy[i].y - testYf) * (enemy[i].y - testYf));
//    			if (distToEnemy < 0.2f){
//        			hitEnemy = 1;
//        			hitWall = 1;
//					break;
//    			}
//			}
//        }
//
//        int ceiling = (float)(SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / ((float)distanceToWall);
//        int floor = SCREEN_HEIGHT - ceiling;
//        for (y = 0; y < SCREEN_HEIGHT; y++) {
//    		if (hitEnemy && y > ceiling && y <= floor) {
//				screen[y][x].Char.AsciiChar = 'E';
//        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
//    		} else {
//        		screen[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
//        		if (y <= ceiling)
//            		screen[y][x].Char.AsciiChar = ' ';
//        		else if (y > ceiling && y <= floor)
//            		screen[y][x].Char.AsciiChar = '#';
//        		else
//            		screen[y][x].Char.AsciiChar = '.';
//    		}
//		}
//    }
//}

static void print_to_buffer(
	const HANDLE buffer,
	CHAR_INFO ci[SCREEN_HEIGHT][SCREEN_WIDTH],
	const int sizeX,
	const int sizeY,
	const int posX,
	const int posY
	
){
	COORD dwBufferSize = {sizeX, sizeY};
	COORD dwBufferCoord = {0, 0};
	
	SMALL_RECT writeRegion = {posX, posY, posX + sizeX - 1, posY + sizeY - 1};
	WriteConsoleOutput(buffer, (CHAR_INFO*)ci, dwBufferSize, dwBufferCoord, &writeRegion);
}

//void render_screen(
//	Screen* screen,
//	const Player player,
//	const char map[MAP_HEIGHT][MAP_WIDTH],
//	const Vector2 enemy[ENEMY_AMOUNT]
//){
//	CHAR_INFO ci[SCREEN_HEIGHT][SCREEN_WIDTH];
//	render_frame(ci, player.pos, player.angle, map, enemy);
//    print_to_buffer(screen->buffer[screen->back], ci);
//    switch_buffer(&screen);
//    resize_screen(&screen);
//}

static void get_empty_ci(CHAR_INFO ci[SCREEN_HEIGHT][SCREEN_WIDTH]){
	int x, y;
	for (y = 0; y < SCREEN_HEIGHT; y++){
		for (x = 0; x < SCREEN_WIDTH; x++){
			ci[y][x].Char.AsciiChar = ' ';
            //ci[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}
}

static void get_title_ci(CHAR_INFO ci[4][16]){
	char title[4][16] = {
		"|== |\\  | /|  /\\",
		"|   | | |\\ | / /",
		"| | |=| |  | \\/ ",
		"|=| | | |  |  \\="
	};
	int x, y;
	for (y = 0; y < 4; y++){
		for (x = 0; x < 16; x++){
			ci[y][x].Char.AsciiChar = title[y][x];
		}
	}
}

static void get_btn(CHAR_INFO ci[10], char btn[8], const int btnSize, const int menuState, const int btnNum){
	
	int x;
	for (x = 2; x < btnSize; x++){
		ci[x].Char.AsciiChar = btn[x];
	}
	
	if(menuState == btnNum){
		char pntr[3] = "->";
		for (x = 0; x < 3; x++){
			ci[x].Char.AsciiChar = pntr[x];
		}
		
		for (x = 0; x < 10; x++){
			ci[x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN;
		}
	}
}


void render_menu(
	Screen* screen,
	const int score,
	const GameState state,
	int* menuState,
	InputState* input
){
	CHAR_INFO ci[SCREEN_HEIGHT][SCREEN_WIDTH];
	get_empty_ci(ci);
	print_to_buffer(screen->buffer[screen->back], ci, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	
	CHAR_INFO title[TITLE_HEIGHT][TITLE_WIDTH];
	get_title_ci(title);
	
	int titlePosX = (SCREEN_WIDTH - TITLE_WIDTH) / 2;
	int titlePosY = (SCREEN_HEIGHT - TITLE_HEIGHT) / 2;
	print_to_buffer(screen->buffer[screen->back], title, TITLE_WIDTH, TITLE_HEIGHT, titlePosX, titlePosY);
	
	
	CHAR_INFO btn[1][10];
	char btnStr = "PLAY";
	get_btn(btn, btnStr, 4, menuState, 0);
	
	int btnPosX = SCREEN_WIDTH  / 2 - 5;
	int btnPosY = (SCREEN_HEIGHT + TITLE_HEIGHT) / 2 + 1;
	print_to_buffer(screen->buffer[screen->back], btn, 10, 1, btnPosX, btnPosY);
	
	btnStr = "QUIT";
	get_btn(btn, btnStr, 4, menuState, 1);
	print_to_buffer(screen->buffer[screen->back], btn, 10, 1, btnPosX, btnPosY + 1);
	
	resize_console(screen);
}

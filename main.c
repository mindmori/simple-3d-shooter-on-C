#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "output.h"
#include "data.h"
#include "input.h"
#include "gamemap.h"
#include "game.h"

void quit(){
	PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
}

int main() {
    char testStr[8];
	
	
	//game vars
    Player player;
    Vector2 enemy[ENEMY_AMOUNT] = {
    	{7.0f, 14.0f},
    	{0.0f, 0.0f},
    	{0.0f, 0.0f}
	};
	int finalScore = -1;
	GameMap map;
	GameState state;
	int menuChoice = 0;
	player.cooldown = MENU_CD_MS;
	
	
	//timer vars
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	float elapsed = 0;
	
	
	//output/input vars
    InputState input;
    ConsoleBuffer cb;
    
    
    //main game loop
    state = INIT;
    while (1) {
    	QueryPerformanceCounter(&start);
    	switch (state){
    		
    		case INIT:
    			initConsoleBuffer(&cb);
    			clearBuffer(&cb, ' ', WHITE);
    			map = *loadMapFromFile("map.txt");
    			initializeGame(&map, &player, enemy);
    			state = MENU;
				break;
			
			case REINIT:
    			finalScore = player.score;
				initializeGame(&map, &player, enemy);
				clearBuffer(&cb, ' ', WHITE);
				state = MENU;
				break;
				
			case MENU:
				update_input(&input);
//				sprintf(testStr, "%lf", btnCD);
//				overlayText(&cb, testStr, (COORD) {0, 0}, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//				sprintf(testStr, "%c%c", input.w ? 'W' : '-', input.s ? 'S' : '-');
//				overlayText(&cb, testStr, (COORD) {0, 1}, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				if (input.e){
					if(!menuChoice)
						state = RUNNING;
					else{
						freeConsoleBuffer(&cb);
						return 0;
					}
						
				}
				
				if ((input.w || input.s) && player.cooldown  <= 0){
					menuChoice = 1 - menuChoice;
					player.cooldown  = MENU_CD_MS;
				}
				
				render_menu(&cb, finalScore, menuChoice);
				break;
			
			case RUNNING:
    			render_frame(&cb, player, map, enemy);
        		update_input(&input);
        		updateGame(&map, &player, enemy, &input, &state);
        
    			break;
		}
		
		//framerate control
		QueryPerformanceCounter(&end);
        elapsed = (end.QuadPart - start.QuadPart) * 1000.0f / freq.QuadPart;
        if (player.cooldown  > 0)
        	player.cooldown  -= FRAME_TIME_MS;
        if (elapsed < FRAME_TIME_MS)
    		Sleep(FRAME_TIME_MS - elapsed);
    }
    
    return 0;
}


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
    Vector3 playerV;
    Vector2 enemy[ENEMY_AMOUNT] = {
    	{7.0f, 14.0f},
    	{0.0f, 0.0f},
    	{0.0f, 0.0f}
	};
	int finalScore;
	GameMap map;
	GameState state;
	int menuChoice = 0;
	float btnCD = MENU_CD_MS;
	
	
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
    			initConsoleBuffer(&cb, 120, 30);
    			clearBuffer(&cb, ' ', WHITE);
    			map = *loadMapFromFile("map.txt");
    			initializeGame(&map, &playerV, enemy);
    			state = MENU;
    			finalScore = 0;
				break;
			
			case REINIT:
				initializeGame(&map, &playerV, enemy);
				clearBuffer(&cb, ' ', WHITE);
				state = MENU;
				break;
				
			case MENU:
				update_input(&input);
				if (input.e){
					if(!menuChoice)
						state = RUNNING;
					else{
						freeConsoleBuffer(&cb);
						return 0;
					}
						
				}
				
				if ((input.w || input.s) && btnCD <= 0){
					menuChoice = 1 - menuChoice;
					btnCD = MENU_CD_MS;
				}
				
				render_menu(&cb, finalScore, menuChoice);
				break;
			
			case RUNNING:
    			finalScore = 1;
    			render_frame(&cb, player.pos, player.angle, player.score, map, enemy);
        		update_input(&input);
        		updateGame(&map, &playerV, enemy, &input, DELTA_TIME, &state);
        
        		player.pos = (Vector2) {playerV.x, playerV.y};
        		player.angle = playerV.z;
    			break;
		}
		
		//framerate control
		QueryPerformanceCounter(&end);
        elapsed = (end.QuadPart - start.QuadPart) * 1000.0f / freq.QuadPart;
        if (btnCD > 0)
        	btnCD -= FRAME_TIME_MS;
        if (elapsed < FRAME_TIME_MS)
    		Sleep(FRAME_TIME_MS - elapsed);
    }
    
    return 0;
}


#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "output.h"
#include "data.h"
#include "input.h"
//#include "game.h"

int main() {
	
	
	//game vars
    Player player;
    Vector2 enemy[ENEMY_AMOUNT];
    GameState state;
    int menuState;
	char map[MAP_HEIGHT][MAP_WIDTH] = {
    	"################",
    	"#..............#",
    	"#.......########",
    	"#..............#",
    	"#......##......#",
    	"#......##......#",
    	"#..............#",
    	"###............#",
    	"##.............#",
    	"#......####..###",
    	"#......#.......#",
    	"#......#.......#",
    	"#..............#",
    	"#......#########",
    	"#..............#",
    	"################"
	};
	
	
	//timer vars
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	float elapsed;
	
	
	//input/output vars
    InputState input;
    Screen screen;
    
    
    //main game loop
    state = INIT;
    while (1) {
    	switch (state){
    		
    		
    		case INIT:
    			screen_init(&screen);
				menuState = 0;
				//initializeGame(map, &player, enemy);
				state = MENU;
				menuState = 0;
				break;
				
			case REINIT:
    			//initializeGame(map, &player, enemy);
    			state = MENU;
    			menuState = 0;
    			break;
    			
    		case MENU:
    			render_menu(&screen, state, player.score, &menuState, &input);
    			update_input(&input);
    			break;
    		
    		
    		case RUNNING:
    			QueryPerformanceCounter(&start);
    			
        		//render_screen(&screen, player, map, enemy);
        		update_input(&input);
				//updateGame(map, &player, enemy, &input, FRAME_TIME_MS / 1000.0f);
        		
				QueryPerformanceCounter(&end);
        		elapsed = (end.QuadPart - start.QuadPart) * 1000.0f / freq.QuadPart;
        		player.time += elapsed;
        		if (elapsed < FRAME_TIME_MS)
    				Sleep(FRAME_TIME_MS - elapsed);
    			break;
		}
    }
    
    return 0;
}


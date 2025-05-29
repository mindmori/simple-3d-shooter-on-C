#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "output.h"
#include "data.h"
#include "input.h"

int main() {
	
	
	//game vars
    Vector3 player = {7.0f, 7.0f, 0.0f};
    Vector2 enemy[ENEMY_AMOUNT] = {
    	{7.0f, 14.0f},
    	{0.0f, 0.0f},
    	{0.0f, 0.0f}
	};
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
	
	//output/input vars
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hBuffer[2];
    int currentBuffer = 0, backBuffer;
    DWORD dwBytesWritten = 0;
    InputState input;
    
    
    //buffers init
    hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hBuffer[0]);
    
    
    //main game loop
    while (1) {
    	QueryPerformanceCounter(&start);
    	
    	backBuffer = 1 - currentBuffer;
    	
        
        render_frame(hBuffer[backBuffer], player, map, enemy);
        update_input(&input);
        
        if (input.a) player.z -= M_PI / (36 * 15);
        if (input.d) player.z += M_PI / (36 * 15);
        
        SetConsoleActiveScreenBuffer(hBuffer[backBuffer]);
        currentBuffer = backBuffer;
        
        
        //framerate control
        QueryPerformanceCounter(&end);
        elapsed = (end.QuadPart - start.QuadPart) * 1000.0f / freq.QuadPart;
        if (elapsed < FRAME_TIME_MS)
    		Sleep(FRAME_TIME_MS - elapsed);
    }
    
    return 0;
}


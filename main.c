#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "output.h"
#include "data.h"

int main() {
	
	
	//game vars
    Vector3 player = {7.0f, 7.0f, 0.0f};
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
	
	//output vars
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hBuffer[2];
    int currentBuffer = 0, backBuffer;
    DWORD dwBytesWritten = 0;
    
    
    //buffers init
    hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hBuffer[0]);
    
    
    //main game loop
    while (1) {
    	QueryPerformanceCounter(&start);
    	
    	backBuffer = 1 - currentBuffer;
    	
        player.z += M_PI / (36 * 60);
        render_frame(hBuffer[backBuffer], player, map);
        
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


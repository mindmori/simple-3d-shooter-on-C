#include <windows.h>
#include "data.h"
#include "gamemap.h"

#ifndef OUTPUT_H
#define OUTPUT_H

typedef struct {
    HANDLE hConsole;
    CHAR_INFO* buffers[2];
    int activeIndex;
    COORD size;
    SMALL_RECT region;
} ConsoleBuffer;

int initConsoleBuffer(ConsoleBuffer* cb);

void render_frame(ConsoleBuffer* cb, Player player, GameMap map, Vector2* enemy);

void clearBuffer(ConsoleBuffer* cb, CHAR c, WORD attr);

void render_menu(ConsoleBuffer* cb, int score, int menuChoice);

void overlayText(ConsoleBuffer* cb, const char* text, COORD pos, WORD attr);

void freeConsoleBuffer(ConsoleBuffer* cb);

#endif

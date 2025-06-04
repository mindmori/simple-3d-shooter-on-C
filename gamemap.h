#ifndef GAMEMAP_H
#define GAMEMAP_H

typedef struct
{
	char **data;
	int width;
	int height;
} GameMap;

void freeGameMap(GameMap *map);
GameMap *loadMapFromFile(const char *filename);

#endif

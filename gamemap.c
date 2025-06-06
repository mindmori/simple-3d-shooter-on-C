#include "gamemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

GameMap *createDefaultMap()
{
	GameMap *map = (GameMap *)malloc(sizeof(GameMap));

	if (!map)
		return NULL;

	map->width = MAP_WIDTH;
	map->height = MAP_HEIGHT;

	map->data = (char **)malloc(map->height * sizeof(char *));

	if (!map->data)
	{
		free(map);
		return NULL;
	}
	const char *defaultMapData[] = {
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
		"################"};

	for (int y = 0; y < map->height; y++)
	{
		map->data[y] = (char *)malloc(map->width * sizeof(char));

		if (!map->data[y])
		{
			for (int j = 0; j < y; j++)
			{
				free(map->data[j]);
			}
			free(map->data);
			free(map);
			return NULL;
		}

		strncpy(map->data[y], defaultMapData[y], map->width);
	}

	return map;
}

GameMap *loadMapFromFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		return createDefaultMap();
	}

	GameMap *map = (GameMap *)malloc(sizeof(GameMap));
	if (!map)
	{
		fclose(file);
		return createDefaultMap();
	}

	// CHECK MAP SIZE
	char line[256];
	map->height = 0;
	map->width = 0;

	while (fgets(line, sizeof(line), file))
	{
		int length = strlen(line);

		// repal;ce \n's with null term.
		if (line[length - 1] == '\n')
		{
			line[--length] = '\0';
		}

		// update map width
		if (length > map->width)
		{
			map->width = length;
		}
		map->height++;
	}

	// height alloc
	map->data = (char **)malloc(map->height * sizeof(char *));

	if (!map->data) // null check
	{
		fclose(file);
		free(map);
		return createDefaultMap();
	}

	// row allocs
	for (int i = 0; i < map->height; i++)
	{
		map->data[i] = (char *)malloc(map->width * sizeof(char));

		if (!map->data[i]) // null check
		{
			for (int j = 0; j < i; j++)
			{
				free(map->data[j]);
			}

			free(map->data);
			fclose(file);
			free(map);
			return createDefaultMap();
		}
	}

	// READ MAP

	rewind(file);

	int y = 0;

	while (fgets(line, sizeof(line), file) && y < map->height)
	{
		int len = strlen(line);
		if (line[len - 1] == '\n')
		{
			line[--len] = '\0';
		}

		for (int x = 0; x < map->width; x++)
		{
			if (x < len)
			{
				map->data[y][x] = line[x];
			}
			else
			{
				map->data[y][x] = ' '; // fill row with empty if shorter
			}
		}
		y++;
	}

	fclose(file);
	return map;
}

void freeGameMap(GameMap *map)
{
	if (map && map->data)
	{
		for (int i = 0; i < map->height; i++)
		{
			free(map->data[i]);
		}
		free(map->data);
		map->data = NULL;
	}
}

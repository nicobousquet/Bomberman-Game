/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/map.h"
#include "../include/monster.h"
#include "../include/bomb.h"
#include "../include/window.h"
#include "../include/sprite.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct map {
    int width;
    int height;
    unsigned char *grid;
    struct bomb *bomb_array[NUM_MAX_BOMBS]; /* bombs of current map */
    struct monster *monster_array[NUM_MONSTER_MAX];
};

struct bomb **map_get_bomb_array(struct map *map) {
    assert(map);
    return map->bomb_array;
}

struct monster **map_get_monster_array(struct map *map) {
    assert(map);
    return map->monster_array;
}

int map_get_size() {
    return sizeof(struct map);
}

int map_get_size_ptr() {
    return sizeof(struct map *);
}

unsigned char *map_get_grid(struct map *map) {
    assert(map);
    return map->grid;
}

struct map *map_new(int width, int height) {
    assert(width > 0 && height > 0);

    struct map *map = malloc(sizeof(struct map));
    if (map == NULL)
        perror("map_new : malloc map failed");

    map->width = width;
    map->height = height;

    map->grid = malloc(height * width);
    if (!map->grid)
        perror("map_new : malloc grid failed");

    // Grid cleaning
    int i, j;
    for (i = 0; i < width; i++)
        for (j = 0; j < height; j++)
            map->grid[CELL(i, j)] = CELL_EMPTY;

    return map;
}

int map_is_inside(struct map *map, int x, int y) {
    assert(map);
    if (x >= 0 && x < map->width && y >= 0 && y < map->height)
        return 1;
    return 0;
}

void map_free(struct map *map) {
    if (map == NULL)
        return;
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (map->monster_array[i] != NULL)
            monster_free(map->monster_array[i]);
    }
    for (int j = 0; j < NUM_MAX_BOMBS; j++) {
        if (map->bomb_array[j] != NULL)
            bomb_free(map->bomb_array[j]);
    }
    free(map->grid);
    free(map);
}

int map_get_width(struct map *map) {
    assert(map != NULL);
    return map->width;
}

int map_get_height(struct map *map) {
    assert(map);
    return map->height;
}

enum cell_type map_get_cell_value(struct map *map, int x, int y) {
    assert(map && map_is_inside(map, x, y));
    return map->grid[CELL(x, y)];
}

void map_set_cell_type(struct map *map, int x, int y, enum cell_type type) {
    assert(map && map_is_inside(map, x, y));
    map->grid[CELL(x, y)] = type;
}

void map_set_grid(struct map *map, unsigned char *grid) {
    assert(map);
    assert(grid);
    map->grid = grid;
}

/* displaying map */
void map_display(struct map *map) {
    assert(map);
    assert(map->height > 0 && map->width > 0);

    /* running through map->grid and displaying each cell */
    for (int i = 0; i < map->width; i++) {
        for (int j = 0; j < map->height; j++) {
            int x = i * SIZE_BLOC;
            int y = j * SIZE_BLOC;

            unsigned char type = map->grid[CELL(i, j)];

            switch ((enum cell_type) (type & 0xf0)) {
                case CELL_SCENERY:
                    window_display_image(sprite_get_scenery((enum scenery_type) (type & 0x0f)), x, y);
                    break;

                case CELL_BOX:
                    window_display_image(sprite_get_box(), x, y);
                    break;

                case CELL_BONUS:
                    window_display_image(sprite_get_bonus((enum bonus_type) (type & 0x0f)), x, y);
                    break;

                case CELL_KEY:
                    window_display_image(sprite_get_key(), x, y);
                    break;

                case CELL_DOOR:
                    window_display_image(sprite_get_door((enum door_status) (type & 0x01)), x, y);
                    break;

                case CELL_BOMB:
                    window_display_image(sprite_get_bomb((type & 0x0f)), x, y);
                    break;

                default:
                    break;
            }
        }
    }
}

struct map *map_get_map(char *filename) {
    assert(filename);
    /* opening map file */
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        perror("open");

    /* getting size of file */
    struct stat s;
    if (fstat(fd, &s) == -1)
        perror("fstat");

    off_t size = s.st_size;
    char *grid = malloc(size);
    if (!grid)
        perror("malloc");

    int offset = 0;
    /* reading file  and setting map->grid */
    while (offset < size) {
        int numread = read(fd, grid + offset, size - offset);
        offset += numread;
    }
    int width = atoi(grid);
    int height = atoi(grid + 3);
    char *grid_addr = grid;
    grid += 6;
    struct map *map = map_new(width, height);
    for (int i = 0; i < width * height; i++) {
        int ret = strtol(grid, &grid, 10);
        map->grid[i] = (unsigned char) ret;
    }
    free(grid_addr);
    return map;
}

/* filling monster_array */
void map_set_monsters(struct map *map) {
    assert(map);
    assert(map_get_height(map) > 0 && map_get_width(map) > 0);
    /* running through the map and looking for CELL_MONSTER */
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_MONSTER) {
                /* creating new monster */
                struct monster *monster = malloc(monster_get_size());
                if (!monster)
                    perror("malloc");

                monster_set_x(monster, i);
                monster_set_y(monster, j);
                monster_set_direction(monster, WEST);
                monster_set_t0(monster, SDL_GetTicks());
                /* setting new monster in monster_array */
                struct monster **monster_array = map_get_monster_array(map);
                for (int k = 0; k < NUM_MONSTER_MAX; k++) {
                    if (monster_array[k] == NULL) {
                        monster_array[k] = monster;
                        break;
                    }
                }
                map_set_cell_type(map, i, j, CELL_EMPTY);
            }
        }
    }
}

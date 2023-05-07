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
#include <fcntl.h>


struct map {
    int width;
    int height;
    unsigned char *grid;
    struct bomb *bombs_list[NUM_MAX_BOMBS]; /* bombs of current map */
    struct monster *monsters_list[NUM_MONSTER_MAX];
};

struct bomb **map_get_bombs_list(struct map *map) {
    assert(map);
    return map->bombs_list;
}

struct monster **map_get_monsters_list(struct map *map) {
    assert(map);
    return map->monsters_list;
}

int map_get_size() {
    return sizeof(struct map);
}

unsigned char *map_get_grid(struct map *map) {
    assert(map);
    return map->grid;
}

struct map *map_new(int width, int height) {
    assert(width > 0 && height > 0);

    struct map *map = malloc(sizeof(struct map));
    if (map == NULL) {
        perror("map_new : malloc map failed");
    }

    map->width = width;
    map->height = height;

    map->grid = malloc(height * width);
    if (!map->grid) {
        perror("map_new : malloc grid failed");
    }

    // Grid cleaning
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            map->grid[CELL(i, j)] = CELL_EMPTY;
        }
    }
    return map;
}

int map_is_inside(struct map *map, int x, int y) {
    assert(map);
    if (x >= 0 && x < map->width && y >= 0 && y < map->height) {
        return 1;
    }
    return 0;
}

void map_free(struct map *map) {
    assert(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (map->monsters_list[i] != NULL) {
            monster_free(map->monsters_list[i]);
        }
    }
    for (int j = 0; j < NUM_MAX_BOMBS; j++) {
        if (map->bombs_list[j] != NULL) {
            bomb_free(map->bombs_list[j]);
        }
    }
    free(map->grid);
    free(map);
}

int map_get_width(struct map *map) {
    assert(map);
    return map->width;
}

int map_get_height(struct map *map) {
    assert(map);
    return map->height;
}

uint8_t map_get_cell_value(struct map *map, int x, int y) {
    assert(map);
    assert(map->grid);
    assert(map_is_inside(map, x, y));
    return map->grid[CELL(x, y)];
}

void map_set_cell_type(struct map *map, int x, int y, enum cell_type type) {
    assert(map);
    assert(map_is_inside(map, x, y));
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

    /* running through map->grid and displaying each cell */
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            int x = i * SIZE_BLOC;
            int y = j * SIZE_BLOC;

            uint8_t type = map_get_cell_value(map, i, j);

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
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    /* getting size of file */
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *grid = malloc(size);
    if (!grid) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* reading file and setting map->grid */
    size_t numread = fread(grid, sizeof(char), size, fp);
    if ((long int) numread != size) {
        perror("fread");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    char *grid_addr = grid;
    int width = strtol(grid, &grid, 10);
    int height = strtol(grid + 1, &grid, 10);
    struct map *map = map_new(width, height);
    for (int i = 0; i < width * height; i++) {
        int ret = strtol(grid, &grid, 10);
        map->grid[i] = (unsigned char) ret;
    }
    free(grid_addr);
    return map;
}

/* filling monsters_list */
void map_set_monsters(struct map *map) {
    assert(map);
    /* running through the map and looking for CELL_MONSTER */
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_MONSTER) {
                /* creating new monster */
                struct monster *monster = monster_init(i, j, DURATION_MONSTER_MOVE);
                timer_start(monster_get_timer(monster));
                /* setting new monster in monsters_list */
                struct monster **monsters_list = map_get_monsters_list(map);
                for (int k = 0; k < NUM_MONSTER_MAX; k++) {
                    if (monsters_list[k] == NULL) {
                        monsters_list[k] = monster;
                        break;
                    }
                }
                map_set_cell_type(map, i, j, CELL_EMPTY);
            }
        }
    }
}

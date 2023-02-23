/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

#include "constant.h"

enum bomb_type {
    EXPLOSION = 1,
    TTL1,
    TTL2,
    TTL3,
    TTL4
};

enum bonus_type {
    EMPTY = 0,
    BONUS_BOMB_RANGE_DEC,
    BONUS_BOMB_RANGE_INC,
    BONUS_BOMB_NB_DEC,
    BONUS_BOMB_NB_INC,
    BONUS_LIFE,
    BONUS_MONSTER
};

enum scenery_type {
    SCENERY_STONE = 1,    // 0001
    SCENERY_TREE = 2,    // 0010
    SCENERY_PRINCESS = 4  // 0100
};

enum door_status {
    OPENED = 0,
    CLOSED
};

enum levels {
    LEVEL1 = 0x00,
    LEVEL2 = 0x02,
    LEVEL3 = 0x04,
    LEVEL4 = 0x06,
    LEVEL5 = 0x08,
    LEVEL6 = 0x0a,
    LEVEL7 = 0x0c,
    LEVEL8 = 0x0e
};

enum cell_type {
    CELL_EMPTY = 0x00,        //  0000 0000
    CELL_SCENERY = 0x10,        //  0001 0000
    CELL_BOX = 0x20,        //  0010 0000 = 32
    CELL_DOOR = 0x30,        //  0011 0000
    CELL_KEY = 0x40,        //  0100 0000
    CELL_BONUS = 0x50,        // 	0101 0000
    CELL_MONSTER = 0x60,        // 	0110 0000 = 96
    CELL_BOMB = 0x70,        // 	0111 0000

    /*
    CELL_TREE = CELL_SCENERY | SCENERY_TREE,
    CELL_STONE = CELL_SCENERY | SCENERY_STONE,
    CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC, // = 34
    CELL_BOX_BOMBDEC = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_BOMBINC = CELL_BOX | BONUS_BOMB_NB_INC, // = 36
    CELL_BOX_MONSTER = CELL_BOX | BONUS_MONSTER, // = 38
    CELL_BOX_LIFE = CELL_BOX | BONUS_LIFE, // = 37

    CELL_MONSTER_NORTH = CELL_MONSTER | NORTH,
    CELL_MONSTER_SOUTH = CELL_MONSTER | SOUTH,
    CELL_MONSTER_EAST = CELL_MONSTER | EAST,
    CELL_MONSTER_WEST = CELL_MONSTER | WEST,

    BOMB_TTL4 = CELL_BOMB | TTL4,
    BOMB_TTL3 = CELL_BOMB | TTL3,
    BOMB_TTL2 = CELL_BOMB | TTL2,
    BOMB_TTL1 = CELL_BOMB | TTL1,
    BOMB_EXPLOSION = CELL_BOMB | EXPLOSION,

    BOMB_RANGE_DEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
    BOMB_RANGE_INC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    BOMB_NB_DEC = CELL_BONUS | BONUS_BOMB_NB_DEC,
    BOMB_NB_INC = CELL_BONUS | BONUS_BOMB_NB_INC,
    MONSTER = CELL_BONUS | BONUS_MONSTER,
    LIFE = CELL_BONUS | BONUS_LIFE
    */
};

struct map;

// Create a new empty map
struct map *map_new(int width, int height);

void map_free(struct map *map);

// Return the height and width of a map
int map_get_width(struct map *map);

int map_get_height(struct map *map);

// Return the type of cell
enum cell_type map_get_cell_value(struct map *map, int x, int y);

// Set the type of cell
void map_set_cell_type(struct map *map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map *map, int x, int y);

// Display the map on the screen
void map_display(struct map *map);

void display_bonus(int x, int y, unsigned char type);

struct bomb **map_get_bomb_array(struct map *map);

int map_get_size();

unsigned char *map_get_grid(struct map *map);

void map_set_grid(struct map *map, unsigned char *grid);

struct map *map_get_map(char *filename);

struct monster **map_get_monster_array(struct map *map);

void map_set_monsters(struct map *map);

#endif /* MAP_H_ */

/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2018-2019"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       60

// Size (# of pixels) of banner
#define LINE_HEIGHT    6
#define BANNER_HEIGHT    60

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define STATIC_MAP_WIDTH  12
#define STATIC_MAP_HEIGHT 12

#define NUM_MONSTER_MAX 10
#define NUM_MAX_BOMBS 9

#define CELL(i, j) ((i) + (j) * map->width)

enum direction {
    NORTH = 0,
    EAST,  // 1
    SOUTH, // 2
    WEST   // 3
};

#endif /* CONSTANT_H_ */

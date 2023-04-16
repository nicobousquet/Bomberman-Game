/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "Bombeirb"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       60

// Size (# of pixels) of banner
#define LINE_HEIGHT    6
#define BANNER_HEIGHT    60

// Time management
#define DEFAULT_GAME_FPS 30

#define NUM_MONSTER_MAX 10
#define NUM_MAX_BOMBS 9

#define DURATION_BOMB_TTL 1000
#define DURATION_MONSTER_MOVE 1000
#define DURATION_INVINCIBILITY_PLAYER 1000

#define CELL(i, j) ((i) + (j) * map->width)

#define NUM_BONUS_TYPE 6

#endif /* CONSTANT_H_ */

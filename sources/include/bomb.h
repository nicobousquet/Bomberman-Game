/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef SOURCES_BOMB_H
#define SOURCES_BOMB_H

#include "map.h"
#include "bomb.h"
#include "player.h"

struct bomb;

void bomb_update(struct map *map, struct player *player);

int bomb_propagation(struct map *map, struct player *player, struct bomb *bomb, enum direction dir);

int bomb_get_size();

void bomb_set_x(struct bomb *bomb, int x);

void bomb_set_y(struct bomb *bomb, int y);

void bomb_init(struct bomb *bomb, int x, int y, int ttl, int t0, int range, int north_range, int south_range, int east_range, int west_range, int propag);

void bomb_free(struct bomb *bomb);

void bomb_set_t0(struct bomb *bomb, int t0);

#endif //SOURCES_BOMB_H

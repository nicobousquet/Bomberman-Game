/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef SOURCES_MONSTER_H
#define SOURCES_MONSTER_H

#include "map.h"
#include "player.h"
#include "constant.h"

struct player;

struct monster;

void monster_free(struct monster* monster);

void monster_update(struct map *map, struct player *player);

int monster_get_size();

void monster_set_x(struct monster *monster, int x);

void monster_set_y(struct monster *monster, int y);

void monster_set_direction(struct monster *monster, enum direction direction);

void monster_set_t0(struct monster *monster, int t0);

int monster_get_x(struct monster *monster);

int monster_get_y(struct monster *monster);

void monsters_display(struct map *map);

#endif //SOURCES_MONSTER_H

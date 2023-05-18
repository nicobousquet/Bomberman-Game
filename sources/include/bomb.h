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

int bomb_explosion(struct map *map, struct player *player, struct bomb *bomb, enum direction dir);

int bomb_get_size();

struct timer *bomb_get_timer(struct bomb *bomb);

void bomb_set_timer(struct bomb *bomb, struct timer *timer);

void bomb_set_x(struct bomb *bomb, int x);

void bomb_set_y(struct bomb *bomb, int y);

void bomb_init(struct bomb *bomb, int x, int y, int range);

void bomb_free(struct bomb *bomb);

#endif //SOURCES_BOMB_H

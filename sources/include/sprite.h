/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include "map.h"
#include "constant.h"

// load game's sprites
void sprite_load();

void sprite_free();

SDL_Surface *sprite_get_princess();

SDL_Surface *sprite_get_player(enum direction direction);

SDL_Surface *sprite_get_bonus(enum bonus_type bonus_type);

SDL_Surface *sprite_get_tree();

SDL_Surface *sprite_get_box();

SDL_Surface *sprite_get_key();

SDL_Surface *sprite_get_stone();

SDL_Surface *sprite_get_door(enum door_status status);

SDL_Surface *sprite_get_number(short number);

SDL_Surface *sprite_get_banner_life();

SDL_Surface *sprite_get_banner_bomb();

SDL_Surface *sprite_get_banner_line();

SDL_Surface *sprite_get_banner_line_vert();

SDL_Surface *sprite_get_banner_range();

SDL_Surface *sprite_get_bomb(int ttl);

SDL_Surface *sprite_get_monster(enum direction direction);

SDL_Surface *sprite_get_scenery(enum scenery_type type);

#endif /* SPRITE_H_ */

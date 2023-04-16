/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include "direction.h"
#include "constant.h"
#include "map.h"

struct player;

// Creates a new player with a given number of available bombs
struct player *player_init(int bomb_number);

enum direction player_get_direction(struct player *player);

void player_free(struct player *player);

// Returns the current position of the player
int player_get_x(struct player *player);

int player_get_y(struct player *player);

// Set the direction of the next move of the player
void player_set_current_way(struct player *player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_num_bomb(struct player *player);

void player_inc_num_bomb(struct player *player);

void player_dec_num_bomb(struct player *player);

void player_set_num_bombs(struct player *player, int num);

void player_dec_num_keys(struct player *player);

// Move the player according to the current direction
int player_move(struct player *player, struct map *map);

// Display the player on the screen
void player_display(struct player *player);

int player_get_num_lives(struct player *player);

void player_dec_num_lives(struct player *player);

int player_get_size();

int player_get_range(struct player *player);

int player_get_num_keys(struct player *player);

struct timer *player_get_timer_invincibility(struct player *player);

void player_set_timer_invincibility(struct player *player, struct timer *timer_invincibility);

#endif /* PLAYER_H_ */

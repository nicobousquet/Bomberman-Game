#ifndef PLAYER_H_
#define PLAYER_H_

#include "direction.h"
#include "cell_types.h"

struct player;

// Creates a new player with a given number of available bombs
struct player *player_init(int num_bombs);

enum direction player_get_direction(struct player *player);

void player_free(struct player *player);

// Returns the current position of the player
int player_get_x(struct player *player);

int player_get_y(struct player *player);

void player_set_x(struct player *player, int x);

void player_set_y(struct player *player, int y);

void player_inc_bombs_range(struct player *player);

void player_dec_bombs_range(struct player *player);

void player_inc_num_lives(struct player *player);

// Set the direction of the next move of the player
void player_set_current_way(struct player *player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_num_bomb(struct player *player);

void player_inc_num_bomb(struct player *player);

void player_dec_num_bomb(struct player *player);

void player_set_num_bombs(struct player *player, int num_bombs);

void player_dec_num_keys(struct player *player);

// Display the player on the screen
void player_display(struct player *player);

int player_get_num_lives(struct player *player);

void player_dec_num_lives(struct player *player);

void player_inc_num_keys(struct player *player);

int player_get_size();

enum direction player_get_direction(struct player *player);

int player_get_bombs_range(struct player *player);

int player_get_num_keys(struct player *player);

struct timer *player_get_timer_invincibility(struct player *player);

void player_set_timer_invincibility(struct player *player, struct timer *timer_invincibility);

void player_get_bonus(struct player *player, enum bonus_type bonus_type);

#endif /* PLAYER_H_ */

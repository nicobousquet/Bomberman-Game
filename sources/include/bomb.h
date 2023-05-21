#ifndef SOURCES_BOMB_H_
#define SOURCES_BOMB_H_

#include "bomb.h"

struct bomb;

void bomb_init(struct bomb *bomb, int x, int y, int range);

void bomb_free(struct bomb *bomb);

int bomb_get_size();

int bomb_get_x(struct bomb *bomb);

int bomb_get_y(struct bomb *bomb);

void bomb_set_x(struct bomb *bomb, int x);

void bomb_set_y(struct bomb *bomb, int y);

enum bomb_type bomb_get_ttl(struct bomb *bomb);

void bomb_dec_ttl(struct bomb *bomb);

int bomb_get_range(struct bomb *bomb);

int bomb_get_north_range(struct bomb *bomb);

int bomb_get_south_range(struct bomb *bomb);

int bomb_get_east_range(struct bomb *bomb);

int bomb_get_west_range(struct bomb *bomb);

void bomb_set_north_range(struct bomb *bomb, int north_range);

void bomb_set_south_range(struct bomb *bomb, int south_range);

void bomb_set_east_range(struct bomb *bomb, int east_range);

void bomb_set_west_range(struct bomb *bomb, int west_range);

int *bomb_get_north_range_ptr(struct bomb *bomb);

int *bomb_get_south_range_ptr(struct bomb *bomb);

int *bomb_get_east_range_ptr(struct bomb *bomb);

int *bomb_get_west_range_ptr(struct bomb *bomb);

struct timer *bomb_get_timer(struct bomb *bomb);

void bomb_set_timer(struct bomb *bomb, struct timer *timer);

#endif //SOURCES_BOMB_H_

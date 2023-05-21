#ifndef MONSTER_H_
#define MONSTER_H_

#include "direction.h"
#include "timer.h"

struct monster;

struct monster *monster_init(int x, int y, int timer_duration);

void monster_free(struct monster* monster);

int monster_get_size();

void monster_set_x(struct monster *monster, int x);

void monster_set_y(struct monster *monster, int y);

enum direction monster_get_direction(struct monster *monster);

void monster_set_direction(struct monster *monster, enum direction direction);

int monster_get_x(struct monster *monster);

int monster_get_y(struct monster *monster);

void monster_set_timer(struct monster *monster, struct timer *timer);

struct timer *monster_get_timer(struct monster *monster);

#endif //MONSTER_H_

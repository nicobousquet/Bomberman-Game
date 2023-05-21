#include "../include/monster.h"
#include <assert.h>
#include <stdlib.h>
#include <SDL/SDL.h>

struct monster {
    int x, y;
    enum direction direction;
    struct timer *timer; /* timer */
};

struct monster *monster_init(int x, int y, int timer_duration) {
    struct monster *monster = malloc(monster_get_size());
    if (!monster) {
        perror("malloc");
    }
    memset(monster, 0, sizeof(struct monster));
    monster_set_x(monster, x);
    monster_set_y(monster, y);
    monster_set_direction(monster, WEST);
    monster->timer = timer_init(timer_duration);
    return monster;
}

void monster_free(struct monster *monster) {
    assert(monster);
    assert(monster->timer);
    free(monster->timer);
    free(monster);
}

int monster_get_size() {
    return sizeof(struct monster);
}

int monster_get_x(struct monster *monster) {
    assert(monster);
    return monster->x;
}

int monster_get_y(struct monster *monster) {
    assert(monster);
    return monster->y;
}

void monster_set_x(struct monster *monster, int x) {
    assert(monster);
    monster->x = x;
}

enum direction monster_get_direction(struct monster *monster) {
    assert(monster);
    return monster->direction;
}

void monster_set_direction(struct monster *monster, enum direction direction) {
    assert(monster);
    monster->direction = direction;
}

void monster_set_y(struct monster *monster, int y) {
    assert(monster);
    monster->y = y;
}

void monster_set_timer(struct monster *monster, struct timer *timer) {
    monster->timer = timer;
}

struct timer *monster_get_timer(struct monster *monster) {
    return monster->timer;
}

void monster_set_current_way(struct monster *monster, enum direction way) {
    assert(monster);
    monster->direction = way;
}

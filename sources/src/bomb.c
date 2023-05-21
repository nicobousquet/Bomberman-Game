#include "../include/bomb.h"
#include "../include/timer.h"
#include "../include/cell_types.h"
#include <assert.h>
#include <stdlib.h>

struct bomb {
    int x; /* position x */
    int y; /* position y */
    enum bomb_type ttl; /* time to live for the bomb */
    struct timer *timer; /* timer of 1 sec */
    int range; /* range of explosion */

    /* range of explosion in each direction */
    int north_range;
    int south_range;
    int east_range;
    int west_range;
};

void bomb_init(struct bomb *bomb, int x, int y, int range) {
    assert(bomb);
    assert(range > 0);
    bomb->x = x;
    bomb->y = y;
    bomb->ttl = TTL4;
    bomb->timer = timer_init();
    bomb->range = range;
    bomb->north_range = 0;
    bomb->south_range = 0;
    bomb->east_range = 0;
    bomb->west_range = 0;
}

void bomb_free(struct bomb *bomb) {
    assert(bomb);
    assert(bomb->timer);
    free(bomb->timer);
    free(bomb);
}

int bomb_get_x(struct bomb *bomb) {
    assert(bomb);
    return bomb->x;
}

int bomb_get_y(struct bomb *bomb) {
    assert(bomb);
    return bomb->y;
}

void bomb_set_x(struct bomb *bomb, int x) {
    assert(bomb);
    bomb->x = x;
}

void bomb_set_y(struct bomb *bomb, int y) {
    assert(bomb);
    bomb->y = y;
}

int bomb_get_range(struct bomb *bomb) {
    assert(bomb);
    return bomb->range;
}

int bomb_get_north_range(struct bomb *bomb) {
    assert(bomb);
    return bomb->north_range;
}

int bomb_get_south_range(struct bomb *bomb) {
    assert(bomb);
    return bomb->south_range;
}

int bomb_get_east_range(struct bomb *bomb) {
    assert(bomb);
    return bomb->east_range;
}

int bomb_get_west_range(struct bomb *bomb) {
    assert(bomb);
    return bomb->west_range;
}

int *bomb_get_north_range_ptr(struct bomb *bomb) {
    assert(bomb);
    return &bomb->north_range;
}

int *bomb_get_south_range_ptr(struct bomb *bomb) {
    assert(bomb);
    return &bomb->south_range;
}

int *bomb_get_east_range_ptr(struct bomb *bomb) {
    assert(bomb);
    return &bomb->east_range;
}

int *bomb_get_west_range_ptr(struct bomb *bomb) {
    assert(bomb);
    return &bomb->west_range;
}

void bomb_set_north_range(struct bomb *bomb, int north_range) {
    assert(bomb);
    bomb->north_range = north_range;
}

void bomb_set_south_range(struct bomb *bomb, int south_range) {
    assert(bomb);
    bomb->south_range = south_range;
}

void bomb_set_east_range(struct bomb *bomb, int east_range) {
    assert(bomb);
    bomb->east_range = east_range;
}

void bomb_set_west_range(struct bomb *bomb, int west_range) {
    assert(bomb);
    bomb->west_range = west_range;
}

enum bomb_type bomb_get_ttl(struct bomb *bomb) {
    assert(bomb);
    return bomb->ttl;
}

void bomb_dec_ttl(struct bomb *bomb) {
    assert(bomb);
    bomb->ttl--;
}

int bomb_get_size() {
    return sizeof(struct bomb);
}

struct timer *bomb_get_timer(struct bomb *bomb) {
    assert(bomb);
    return bomb->timer;
}

void bomb_set_timer(struct bomb *bomb, struct timer *timer) {
    assert(bomb);
    assert(timer);
    bomb->timer = timer;
}

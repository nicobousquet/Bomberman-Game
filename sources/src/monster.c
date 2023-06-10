#include "../include/monster.h"
#include "../include/sprite.h"
#include "../include/window.h"
#include "../include/constant.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @brief Structure representing a monster.
 */
struct monster {
    int x; /**< X-coordinate of the monster */
    int y; /**< Y-coordinate of the monster */
    enum direction direction; /**< Current direction of the monster */
    struct timer *timer; /**< Timer for the monster */
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

void monster_display(struct monster *monster, struct SDL_Surface *window) {
    assert(monster);
    assert(window);
    window_display_image(window, sprite_get_monster(monster->direction), monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

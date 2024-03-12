#include "../include/bomb.h"
#include "../include/timer.h"
#include "../include/cell_types.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a bomb.
 */
struct bomb {
    int x; /**< X-coordinate of the bomb */
    int y; /**< Y-coordinate of the bomb */
    enum bomb_type ttl; /**< Time to live for the bomb */
    struct timer *timer; /**< Timer of 1 second */
    int range; /**< Range of explosion */
};

struct bomb *bomb_new(int x, int y, int range) {
    assert(range > 0);

    struct bomb *bomb = malloc(sizeof(*bomb));

    if (!bomb) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(bomb, 0, sizeof(*bomb));

    bomb->x = x;
    bomb->y = y;
    bomb->ttl = TTL4;
    bomb->timer = timer_new();
    bomb->range = range;

    return bomb;
}

void bomb_free(struct bomb *bomb) {
    assert(bomb);
    assert(bomb->timer);

    timer_free(bomb->timer);
    free(bomb);
}

void bomb_write(struct bomb *bomb, FILE *file) {
    assert(bomb);
    assert(file);

    fwrite(bomb, sizeof(*bomb), 1, file);
    timer_write(bomb->timer, file);
}

void bomb_read(struct bomb *bomb, FILE *file) {
    assert(bomb);

    struct timer *timer = bomb->timer;

    fread(bomb, sizeof(*bomb), 1, file);
    bomb->timer = timer;
    timer_read(bomb->timer, file);
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

enum bomb_type bomb_get_ttl(struct bomb *bomb) {
    assert(bomb);
    return bomb->ttl;
}

void bomb_dec_ttl(struct bomb *bomb) {
    assert(bomb);
    bomb->ttl--;
}

void bomb_set_ttl(struct bomb *bomb, enum bomb_type ttl) {
    assert(bomb);
    bomb->ttl = ttl;
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

#include "../include/bomb_node.h"
#include "../include/timer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a bomb_node.
 */
struct bomb_node {
    int x; /**< X-coordinate of the bomb node */
    int y; /**< Y-coordinate of the bomb node */
    enum bomb_state state; /**< State of the bomb node (INIT, TTL4, TTL3, TTL2, TTL1, EXPLODING, DONE) */
    struct timer *timer; /**< Timer of the bomb node */
    int north_range; /**< Range of explosion in the north direction */
    int south_range; /**< Range of explosion in the south direction */
    int east_range; /**< Range of explosion in the east direction */
    int west_range; /**< Range of explosion in the west direction */
    struct bomb_node *next; /**< Pointer to the next bomb node */
};

struct bomb_node *bomb_node_new(int x, int y, int range) {
    assert(range > 0);

    struct bomb_node *bomb_node = malloc(sizeof(struct bomb_node));

    if (!bomb_node) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(bomb_node, 0, sizeof(struct bomb_node));

    bomb_node->x = x;
    bomb_node->y = y;
    bomb_node->state = INIT;
    bomb_node->timer = timer_new();
    bomb_node->north_range = 0;
    bomb_node->south_range = 0;
    bomb_node->east_range = 0;
    bomb_node->west_range = 0;
    bomb_node->next = NULL;

    return bomb_node;
}

void bomb_node_free(struct bomb_node *bomb_node) {
    assert(bomb_node);
    assert(bomb_node->timer);

    timer_free(bomb_node->timer);
    free(bomb_node);
}

void bomb_node_write(struct bomb_node *bomb_node, FILE *file) {
    assert(bomb_node);
    assert(file);

    fwrite(bomb_node, sizeof(struct bomb_node), 1, file);
    timer_write(bomb_node->timer, file);
}

struct bomb_node *bomb_node_read(FILE *file) {
    assert(file);

    struct bomb_node *bomb_node = malloc(sizeof(struct bomb_node));

    if (!bomb_node) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fread(bomb_node, sizeof(struct bomb_node), 1, file);

    bomb_node->timer = timer_read(file);

    return bomb_node;
}

struct bomb_node *bomb_node_get_next(struct bomb_node *bomb_node) {
    assert(bomb_node);
    return bomb_node->next;
}

void bomb_node_set_next(struct bomb_node *bomb_node, struct bomb_node *next) {
    assert(bomb_node);
    bomb_node->next = next;
}

int bomb_node_get_direction_range(struct bomb_node *bomb_node, enum direction direction) {
    assert(bomb_node);

    switch (direction) {

        case NORTH:
            return bomb_node->north_range;

        case SOUTH:
            return bomb_node->south_range;

        case EAST:
            return bomb_node->east_range;

        default:
            return bomb_node->west_range;

    }
}

void bomb_node_set_direction_range(struct bomb_node *bomb_node, enum direction direction, int range) {
    assert(bomb_node);

    switch (direction) {

        case NORTH:
            bomb_node->north_range = range;
            break;

        case SOUTH:
            bomb_node->south_range = range;
            break;

        case EAST:
            bomb_node->east_range = range;
            break;

        default:
            bomb_node->west_range = range;
            break;

    }
}

int bomb_node_get_x(struct bomb_node *bomb_node) {
    assert(bomb_node);
    return bomb_node->x;
}

int bomb_node_get_y(struct bomb_node *bomb_node) {
    assert(bomb_node);
    return bomb_node->y;
}

void bomb_node_set_x(struct bomb_node *bomb_node, int x) {
    assert(bomb_node);
    bomb_node->x = x;
}

void bomb_node_set_y(struct bomb_node *bomb_node, int y) {
    assert(bomb_node);
    bomb_node->y = y;
}

enum bomb_state bomb_node_get_state(struct bomb_node *bomb_node) {
    assert(bomb_node);
    return bomb_node->state;
}

void bomb_node_dec_state(struct bomb_node *bomb_node) {
    assert(bomb_node);
    bomb_node->state--;
}

void bomb_node_set_state(struct bomb_node *bomb_node, enum bomb_state state) {
    assert(bomb_node);
    bomb_node->state = state;
}

struct timer *bomb_node_get_timer(struct bomb_node *bomb_node) {
    assert(bomb_node);
    return bomb_node->timer;
}

void bomb_node_set_timer(struct bomb_node *bomb_node, struct timer *timer) {
    assert(bomb_node);
    assert(timer);

    bomb_node->timer = timer;
}

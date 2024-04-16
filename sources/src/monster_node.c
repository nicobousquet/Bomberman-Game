#include "../include/monster_node.h"
#include "../include/window.h"
#include "../include/constant.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @brief Structure representing a monster_node.
 */
struct monster_node {
    int x; /**< X-coordinate of the monster node */
    int y; /**< Y-coordinate of the monster node */
    enum direction direction; /**< Current direction of the monster node */
    struct timer *timer; /**< Timer for the monster node */
    struct monster_node *next; /**< Pointer to the next monster node */
};

struct monster_node *monster_node_new(int x, int y) {

    struct monster_node *monster_node = malloc(sizeof(struct monster_node));

    if (!monster_node) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(monster_node, 0, sizeof(struct monster_node));

    monster_node_set_x(monster_node, x);
    monster_node_set_y(monster_node, y);
    monster_node_set_direction(monster_node, WEST);
    monster_node->timer = timer_new();
    timer_start(monster_node->timer, DURATION_MONSTER_MOVE);
    monster_node->next = NULL;

    return monster_node;
}

void monster_node_free(struct monster_node *monster_node) {
    assert(monster_node);
    assert(monster_node->timer);

    timer_free(monster_node->timer);
    free(monster_node);
}

void monster_node_write(struct monster_node *monster_node, FILE *file) {
    assert(monster_node);
    assert(file);

    fwrite(monster_node, sizeof(struct monster_node), 1, file);
    timer_write(monster_node->timer, file);
}

struct monster_node *monster_node_read(FILE *file) {
    assert(file);

    struct monster_node *monster_node = malloc(sizeof(struct monster_node));

    if (!monster_node) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fread(monster_node, sizeof(struct monster_node), 1, file);

    monster_node->timer = timer_read(file);

    return monster_node;
}

int monster_node_get_x(struct monster_node *monster_node) {
    assert(monster_node);
    return monster_node->x;
}

int monster_node_get_y(struct monster_node *monster_node) {
    assert(monster_node);
    return monster_node->y;
}

void monster_node_set_x(struct monster_node *monster_node, int x) {
    assert(monster_node);
    monster_node->x = x;
}

void monster_node_set_y(struct monster_node *monster_node, int y) {
    assert(monster_node);
    monster_node->y = y;
}

struct monster_node *monster_node_get_next(struct monster_node *monster_node) {
    assert(monster_node);
    return monster_node->next;
}

void monster_node_set_next(struct monster_node *monster_node, struct monster_node *next) {
    assert(monster_node);

    monster_node->next = next;
}

enum direction monster_node_get_direction(struct monster_node *monster_node) {
    assert(monster_node);
    return monster_node->direction;
}

void monster_node_set_direction(struct monster_node *monster_node, enum direction direction) {
    assert(monster_node);
    monster_node->direction = direction;
}

void monster_node_set_timer(struct monster_node *monster_node, struct timer *timer) {
    monster_node->timer = timer;
}

struct timer *monster_node_get_timer(struct monster_node *monster_node) {
    return monster_node->timer;
}

void monster_node_display(struct monster_node *monster_node, struct SDL_Surface *window, struct sprites *sprites) {
    assert(monster_node);
    assert(window);
    assert(sprites);

    window_display_image(window, sprites_get_monster(sprites, monster_node->direction), monster_node->x * SIZE_BLOC, monster_node->y * SIZE_BLOC);
}

void monster_node_move(struct monster_node *monster_node, enum direction direction) {
    assert(monster_node);

    monster_node_set_direction(monster_node, direction);

    monster_node_set_x(monster_node, direction_get_x(direction, monster_node_get_x(monster_node), 1));
    monster_node_set_y(monster_node, direction_get_y(direction, monster_node_get_y(monster_node), 1));

    timer_start(monster_node_get_timer(monster_node), DURATION_MONSTER_MOVE);
}

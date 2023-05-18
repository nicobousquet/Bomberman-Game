/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/monster.h"
#include "../include/window.h"
#include "../include/sprite.h"
#include <assert.h>
#include <stdlib.h>
#include <SDL/SDL.h>

struct monster {
    int x, y;
    enum direction direction;
    struct timer *timer; /* timer */
};

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

void monster_set_direction(struct monster *monster, enum direction direction) {
    assert(monster);
    monster->direction = direction;
}

void monster_set_y(struct monster *monster, int y) {
    assert(monster);
    monster->y = y;
}

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

int monster_meets_player(int monster_x, int monster_y, int player_x, int player_y) {
    if (monster_x == player_x && monster_y == player_y) {
        return 1;
    }

    return 0;
}

int monster_meets_monster(struct map *map, int x, int y) {
    assert(map);
    struct monster **monsters_list = map_get_monsters_list(map);
    for (int i = 0; i < NUM_MAX_MONSTERS; i++) {
        if (monsters_list[i] != NULL) {
            if (monsters_list[i]->x == x && monsters_list[i]->y == y) {
                return 1;
            }
        }
    }
    return 0;
}

/* returns 1 if monster can move, else 0 */
static int monster_move_aux(struct map *map, struct player *player, int x, int y) {
    assert(map);
    assert(player);
    if (!map_is_inside(map, x, y)) {
        return 0;
    }

    if (monster_meets_player(x, y, player_get_x(player), player_get_y(player))) {
        struct timer *timer_invincibility = player_get_timer_invincibility(player);
        timer_update(timer_invincibility);
        if (timer_get_state(timer_invincibility) == IS_OVER) {
            player_dec_num_lives(player);
            timer_start(timer_invincibility, DURATION_INVINCIBILITY_PLAYER);
        }
        return 0;
    }
    if (monster_meets_monster(map, x, y)) {
        return 0;
    }

    switch (map_get_cell_value(map, x, y) & 0xf0) {
        case CELL_SCENERY:
            return 0;

        case CELL_BOX:
            return 0;

        case CELL_BONUS:
            return 1;

        case CELL_BOMB:
            return 0;

        case CELL_DOOR:
            return 0;

        case CELL_KEY:
            return 1;

        default:
            return 1;
    }
}

/* updating grid */
void monster_move(struct monster *monster, struct map *map, struct player *player) {
    assert(monster);
    assert(map);
    assert(player);
    int x = direction_get_x(monster_get_x(monster), monster->direction, 1);
    int y = direction_get_y(monster_get_y(monster), monster->direction, 1);

    if (monster_move_aux(map, player, x, y)) {
        monster_set_x(monster, x);
        monster_set_y(monster, y);
    }
}

void monster_update(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    /* running through monsters_list */
    struct monster **monsters_list = map_get_monsters_list(map);
    for (int i = 0; i < NUM_MAX_MONSTERS; i++) {
        if (monsters_list[i] != NULL) {
            /* getting monster in monster list */
            struct monster *monster = monsters_list[i];
            timer_update(monster->timer);
            /* monster moves randomly */
            /* changing direction every second */
            if (timer_get_state(monster->timer) == IS_OVER) {
                int random_dir = rand() % 4;
                monster->direction = (enum direction) random_dir;
                monster_move(monster, map, player);
                timer_start(monster->timer, DURATION_MONSTER_MOVE);
            }
        }
    }
}

/* displaying monster */
void monsters_display(struct map *map) {
    assert(map);
    struct monster **monsters_list = map_get_monsters_list(map);
    for (int i = 0; i < NUM_MAX_MONSTERS; i++) {
        if (monsters_list[i] != NULL) {
            window_display_image(sprite_get_monster(monsters_list[i]->direction), monster_get_x(monsters_list[i]) * SIZE_BLOC, monster_get_y(monsters_list[i]) * SIZE_BLOC);
        }
    }
}

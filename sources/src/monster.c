/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/monster.h"
#include "../include/window.h"
#include "../include/sprite.h"
#include <assert.h>
#include <stdlib.h>
#include <SDL.h>

struct monster {
    int x, y;
    enum direction direction;
    /* timer */
    int t0;
};

void monster_free(struct monster *monster) {
    assert(monster);
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

void monster_set_t0(struct monster *monster, int t0) {
    assert(monster);
    monster->t0 = t0;
}

void monster_set_y(struct monster *monster, int y) {
    assert(monster);
    monster->y = y;
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

int monster_meets_monster(struct map *map, int x, int y){
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            if (monster_array[i]->x == x && monster_array[i]->y == y) {
                return 1;
            }
        }
    }
    return 0;
}

/* returns 1 if monster can move, else 0 */
static int monster_move_aux(struct map *map, struct player *player, int x, int y) {
    if (!map_is_inside(map, x, y)) {
        return 0;
    }
    if (monster_meets_player(x, y, player_get_x(player), player_get_y(player))) {
        if (SDL_GetTicks() - player_get_t0(player) > 1000) {
            player_dec_nb_life(player);
            player_set_t0(player, SDL_GetTicks());
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

        case CELL_BOMB: {
            return 0;
        }
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
    int x = monster->x;
    int y = monster->y;

    if (monster->direction == NORTH) {
        y--;
    } else if (monster->direction == SOUTH) {
        y++;
    } else if (monster->direction == WEST) {
        x--;
    } else if (monster->direction == EAST) {
        x++;
    }

    if (monster_move_aux(map, player, x, y)) {
        monster->x = x;
        monster->y = y;
    }
}

void monster_update(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    /* running through monster_array */
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            /* getting monster in monster array */
            struct monster *monster = monster_array[i];
            int t1 = SDL_GetTicks();
            /* monster moves randomly */
            /* changing direction every second */
            if (t1 - monster->t0 > 1000) {
                int random_dir = rand() % 4;
                monster->direction = (enum direction) random_dir;
                monster_move(monster, map, player);
                /* updating timer value */
                monster->t0 = t1;
            }
        }
    }
}

/* displaying monster */
void monsters_display(struct map* map) {
    struct monster **monster_array = map_get_monster_array(map);
    for (int i =0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            window_display_image(sprite_get_monster(monster_array[i]->direction), monster_array[i]->x * SIZE_BLOC, monster_array[i]->y * SIZE_BLOC);
        }
    }
}

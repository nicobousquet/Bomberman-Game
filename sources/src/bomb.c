/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/bomb.h"
#include "../include/monster.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct bomb {
    int x; /* position x */
    int y; /* position y */
    enum bomb_type ttl; /* time to live */
    struct timer *timer; /* timer of 1 sec */
    int range; /* range of explosion */

    /* range of explosion in each direction */
    int north_range;
    int south_range;
    int east_range;
    int west_range;

    int exploded;
};

void bomb_init(struct bomb *bomb, int x, int y, int range, int timer_duration) {
    assert(bomb);
    assert(range > 0);
    bomb->x = x;
    bomb->y = y;
    bomb->ttl = TTL4;
    bomb->timer = timer_init(timer_duration);
    timer_start(bomb->timer);
    bomb->range = range;
    bomb->north_range = 0;
    bomb->south_range = 0;
    bomb->east_range = 0;
    bomb->west_range = 0;
    bomb->exploded = 0;
}

void bomb_set_x(struct bomb *bomb, int x) {
    assert(bomb);
    bomb->x = x;
}

void bomb_set_y(struct bomb *bomb, int y) {
    assert(bomb);
    bomb->y = y;
}

int bomb_get_size() {
    return sizeof(struct bomb);
}

struct timer *bomb_get_timer(struct bomb *bomb) {
    return bomb->timer;
}

void bomb_set_timer(struct bomb *bomb, struct timer *timer) {
    bomb->timer = timer;
}

void bomb_free(struct bomb *bomb) {
    assert(bomb);
    assert(bomb->timer);
    free(bomb->timer);
    free(bomb);
}

void set_bonus_monster(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));
    struct monster *monster = monster_init(x, y, TIMER_DURATION);
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] == NULL) {
            monster_array[i] = monster;
            break;
        }
    }
}

/* set bonus type after a CELL_BOX exploded */
void bomb_set_bonus(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));
    enum bonus_type bonus_type = map_get_cell_value(map, x, y) & 0x0f;
    /* if CELL_BOX is empty, sets a bonus randomly */
    if (bonus_type == EMPTY) {
        bonus_type = rand() % NUM_BONUS_TYPE + 1;
    }
    if (bonus_type == BONUS_MONSTER) {
        set_bonus_monster(map, x, y);
        map_set_cell_type(map, x, y, CELL_EMPTY);
    } else {
        map_set_cell_type(map, x, y, CELL_BONUS | bonus_type);
    }
}

int bomb_can_propag(enum cell_type cell_type) {
    if (cell_type != CELL_SCENERY && (cell_type & 0x0f) != TTL4 && (cell_type & 0x0f) != TTL3 && (cell_type & 0x0f) != TTL2 && (cell_type & 0x0f) != TTL1 && cell_type != CELL_DOOR && cell_type != CELL_KEY) {
        return 1;
    }
    return 0;
}

int bomb_meets_player(int explosion_x, int explosion_y, int player_x, int player_y) {
    if (explosion_x == player_x && explosion_y == player_y) {
        return 1;
    }
    return 0;
}

struct monster **bomb_meets_monster(struct map *map, int bomb_x, int bomb_y) {
    assert(map);
    assert(map_is_inside(map, bomb_x, bomb_y));
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] && monster_get_x(monster_array[i]) == bomb_x && monster_get_y(monster_array[i]) == bomb_y) {
            return &monster_array[i];
        }
    }
    return NULL;
}

void bomb_kill_monster(struct monster **monster, struct map *map) {
    assert(monster);
    assert(*monster);
    assert(map);
    map_set_cell_type(map, monster_get_x(*monster), monster_get_y(*monster), CELL_EMPTY);
    monster_free(*monster);
    *monster = NULL;
}

/* managing propagation of bombs */
/* return bomb range*/
int bomb_propagation(struct map *map, struct player *player, struct bomb *bomb, enum direction dir) {
    assert(map);
    assert(player);
    assert(bomb);
    int x = bomb->x;
    int y = bomb->y;
    int range;
    for (range = 1; range <= bomb->range; range++) {
        if (dir == NORTH) {
            y--;
        } else if (dir == SOUTH) {
            y++;
        } else if (dir == EAST) {
            x++;
        } else if (dir == WEST) {
            x--;
        }
        if (map_is_inside(map, x, y)) {
            enum cell_type cell_type = map_get_cell_value(map, x, y) & 0xf0;
            struct monster **dead_monster = NULL;
            /* if bomb can propagate */
            if (bomb_can_propag(cell_type)) {
                if (cell_type == CELL_BOX) {
                    /* setting cell type to BONUS_TYPE */
                    bomb_set_bonus(map, x, y);
                    /* stopping propagation */
                    return range - 1;
                } else if (bomb_meets_player(x, y, player_get_x(player), player_get_y(player))) {
                    /* player looses a life */
                    player_dec_num_lives(player);
                    return range - 1;
                } else if ((dead_monster = bomb_meets_monster(map, x, y)) != NULL) {
                    bomb_kill_monster(dead_monster, map);
                    map_set_cell_type(map, x, y, CELL_BOMB | EXPLOSION);
                    return range;
                } else {
                    /* setting cell type to BOMB_EXPLOSION */
                    map_set_cell_type(map, x, y, CELL_BOMB | EXPLOSION);
                }
            } /* if bomb cannot propagate */
            else {
                return range - 1;
            }
        } /* if cell is not inside map */
        else {
            return range - 1;
        }
    }
    return range - 1;
}

/* reset cell type to CELL_EMPTY after explosion */
void bomb_extinction(struct map *map, struct bomb *bomb) {
    assert(map);
    assert(bomb);
    int x = bomb->x;
    int y = bomb->y;
    map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
    for (int i = 1; i <= bomb->north_range; i++) {
        map_set_cell_type(map, x, y - i, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb->south_range; i++) {
        map_set_cell_type(map, x, y + i, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb->east_range; i++) {
        map_set_cell_type(map, x + i, y, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb->west_range; i++) {
        map_set_cell_type(map, x - i, y, CELL_EMPTY);
    }
}

void bomb_update(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    /* running through BOMBS_ARRAY*/
    struct bomb **map_bomb_array = map_get_bomb_array(map);
    for (int i = 0; i < NUM_MAX_BOMBS; i++) {
        if (map_bomb_array[i] != NULL) {
            /* updating bomb properties */
            struct bomb *bomb = map_bomb_array[i];
            timer_update(bomb->timer);
            if (timer_is_over(bomb->timer)) {
                bomb->ttl--;
                timer_reset(bomb->timer, TIMER_DURATION);
                timer_start(bomb->timer);
            }
            if (bomb->ttl <= TTL4 && bomb->ttl > TTL1) {
                map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB | bomb->ttl);
            } else if (bomb->ttl == TTL1) {
                if (bomb->exploded == 0) {
                    if (bomb_meets_player(bomb->x, bomb->y, player_get_x(player), player_get_y(player))) {
                        player_dec_num_lives(player);
                    }
                    /* setting bomb range  and cell type to CELL_EXPLOSION */
                    map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB | EXPLOSION);
                    bomb->north_range = bomb_propagation(map, player, bomb, NORTH);
                    bomb->south_range = bomb_propagation(map, player, bomb, SOUTH);
                    bomb->east_range = bomb_propagation(map, player, bomb, EAST);
                    bomb->west_range = bomb_propagation(map, player, bomb, WEST);
                    /* setting .exploded to 1 to enter only once in that loop */
                    bomb->exploded = 1;
                }
            } else if (bomb->ttl == EXPLOSION) {
                /* setting to CELL_EMPTY after bomb exploded */
                bomb_extinction(map, bomb);
                free(bomb->timer);
                free(bomb);
                map_bomb_array[i] = NULL;
            }
        }
    }
}

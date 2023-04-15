/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/player.h"
#include "../include/sprite.h"
#include "../include/misc.h"
#include "../include/monster.h"
#include "../include/window.h"
#include <assert.h>
#include <stdlib.h>

struct player {
    int x, y; /* player's position */
    enum direction direction;
    int bombs; /* number of bombs */
    int range; /* range of bombs */
    int lives; /* number of lives */
    int keys; /* number of keys */
    struct timer *timer_invincibility; /* timer */
};

struct player *player_init(int bombs) {
    assert(bombs >= 0 && bombs <= 9);
    struct player *player = malloc(sizeof(struct player));
    if (!player) {
        error("Memory error");
    }

    memset(player, 0, sizeof(struct player));
    player->direction = NORTH;
    player->bombs = bombs;
    player->range = 1;
    player->lives = 3;
    player->keys = 0;
    player->timer_invincibility = timer_init(TIMER_DURATION);
    player->x = 1;
    player->y = 0;
    return player;
}

struct timer *player_get_timer_invincibility(struct player *player) {
    assert(player);
    return player->timer_invincibility;
}

void player_set_timer_invincibility(struct player *player, struct timer *timer_invincibility) {
    player->timer_invincibility = timer_invincibility;
}

void player_free(struct player *player) {
    assert(player);
    assert(player->timer_invincibility);
    free(player->timer_invincibility);
    free(player);
}

int player_get_x(struct player *player) {
    assert(player);
    return player->x;
}

int player_get_y(struct player *player) {
    assert(player);
    return player->y;
}

void player_set_current_way(struct player *player, enum direction way) {
    assert(player);
    player->direction = way;
}

int player_get_num_bomb(struct player *player) {
    assert(player);
    return player->bombs;
}

void player_inc_num_bomb(struct player *player) {
    assert(player);
    if (player->bombs < 9) {
        player->bombs++;
    }
}

void player_dec_num_bomb(struct player *player) {
    assert(player);
    if (player->bombs > 0) {
        player->bombs--;
    }
}

void player_set_num_bombs(struct player *player, int num) {
    assert(player);
    if (num <= 9 && num >= 0) {
        player->bombs = num;
    }
}

int player_get_num_lives(struct player *player) {
    assert(player);
    return player->lives;
}

void player_dec_num_lives(struct player *player) {
    assert(player);
    if (player->lives > 0) {
        player->lives--;
    }
}

void player_inc_num_lives(struct player *player) {
    assert(player);
    if (player->lives < 9) {
        player->lives++;
    }
}

int player_get_size() {
    return sizeof(struct player);
}

int player_get_range(struct player *player) {
    assert(player);
    return player->range;
}

void player_inc_range(struct player *player) {
    assert(player);
    if (player->range < 9) {
        player->range++;
    }
}

void player_dec_range(struct player *player) {
    assert(player);
    if (player->range > 1) {
        player->range--;
    }
}

int player_get_num_keys(struct player *player) {
    assert(player);
    return player->keys;
}

void player_inc_num_keys(struct player *player) {
    assert(player);
    if (player->keys < 9) {
        player->keys++;
    }
}

void player_dec_num_keys(struct player *player) {
    assert(player);
    if (player->keys > 0) {
        player->keys--;
    }
}

int box_meets_monster(struct map *map, int x, int y) {
    assert(map);
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            if (monster_get_x(monster_array[i]) == x && monster_get_y(monster_array[i]) == y) {
                return 1;
            }
        }
    }
    return 0;
}

int player_can_push_box(struct map *map, struct player *player, int x_src, int y_src) {
    assert(map);
    assert(player);
    int x_dst = x_src;
    int y_dst = y_src;
    if (player->direction == NORTH) {
        y_dst = y_src - 1;
    } else if (player->direction == SOUTH) {
        y_dst = y_src + 1;
    } else if (player->direction == EAST) {
        x_dst = x_src + 1;
    } else if (player->direction == WEST) {
        x_dst = x_src - 1;
    }
    /* if box is on the side of the map, player cannot push the box */
    if (!map_is_inside(map, x_dst, y_dst)) {
        return 0;
    }
    /* if cell after CELL_BOX is CELL_EMPTY, player can push the box */
    if ((map_get_cell_value(map, x_dst, y_dst) & 0xf0) == CELL_EMPTY && !box_meets_monster(map, x_dst, y_dst)) {
        map_set_cell_type(map, x_dst, y_dst, CELL_BOX);
        map_set_cell_type(map, x_src, y_src, CELL_EMPTY);
        return 1;
    }
    /* if not CELL_EMPTY, player cannot push the box */
    return 0;
}

void player_get_bonus(struct player *player, struct map *map, int x, int y, enum cell_type type) {
    assert(player);
    assert(map);
    enum bonus_type bonus_type = type & 0x0f;
    if (bonus_type == BONUS_BOMB_RANGE_INC) {
        player_inc_range(player);
    } else if (bonus_type == BONUS_BOMB_RANGE_DEC) {
        player_dec_range(player);
    } else if (bonus_type == BONUS_BOMB_NB_DEC) {
        player_dec_num_bomb(player);
    } else if (bonus_type == BONUS_BOMB_NB_INC) {
        player_inc_num_bomb(player);
    } else if (bonus_type == BONUS_LIFE) {
        player_inc_num_lives(player);
    }

    map_set_cell_type(map, x, y, CELL_EMPTY);
}

void player_open_door(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            enum cell_type type = map_get_cell_value(map, i, j);
            if ((type & 0xf0) == CELL_DOOR && (type & 0x01) == CLOSED) {
                map_set_cell_type(map, i, j, type & 0xfe);
                player_dec_num_keys(player);
                break;
            }
        }
    }
}

int player_meets_monster(struct map *map, int x, int y) {
    assert(map);
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            if (monster_get_x(monster_array[i]) == x && monster_get_y(monster_array[i]) == y) {
                return 1;
            }
        }
    }
    return 0;
}

/* allow player to move */
static int player_move_aux(struct player *player, struct map *map, int x, int y) {
    assert(player);
    if (!map_is_inside(map, x, y)) {
        return 0;
    }

    if (player_meets_monster(map, x, y)) {
        struct timer *timer_invincibility = player_get_timer_invincibility(player);
        timer_update(timer_invincibility);
        if (timer_is_over(timer_invincibility)) {
            player_dec_num_lives(player);
            timer_reset(timer_invincibility, TIMER_DURATION);
            timer_start(timer_invincibility);
        }
        return 0;
    }
    enum cell_type cell = map_get_cell_value(map, x, y);
    switch (cell & 0xf0) {
        /* if player goes to CELL_SCENERY */
        case CELL_SCENERY:
            if ((cell & 0x0f) == SCENERY_PRINCESS) {
                return 1;
            }

            return 0;
            /* if player goes to CELL_BOX */
        case CELL_BOX:
            /* checking if player can push the box */
            if (player_can_push_box(map, player, x, y)) {
                return 1;
            }

            return 0;
            /* if player goes to CELL_BONUS */
        case CELL_BONUS:
            player_get_bonus(player, map, x, y, cell);
            return 1;
            /* if player goes to CELL_BOMB */
        case CELL_BOMB:
            if ((cell & 0x0f) == EXPLOSION) {
                player_dec_num_lives(player);
            }
            return 1;
            /* if player goes to CELL_KEY */
        case CELL_KEY:
            player_inc_num_keys(player);
            map_set_cell_type(map, x, y, CELL_EMPTY);
            /* setting door as OPENED */
            player_open_door(map, player);
            return 1;
            /* if player goes to CELL_DOOR */
        case CELL_DOOR:
            if ((cell & 0x01) == OPENED) {
                return 1;
            }
            return 0;
        default:
            break;
    }
    return 1;
}

int player_move(struct player *player, struct map *map) {
    assert(player);
    assert(map);
    int x = player_get_x(player);
    int y = player_get_y(player);

    if (player->direction == NORTH) {
        y--;
    } else if (player->direction == SOUTH) {
        y++;
    } else if (player->direction == WEST) {
        x--;
    } else if (player->direction == EAST) {
        x++;
    }

    if (player_move_aux(player, map, x, y)) {
        player->x = x;
        player->y = y;
        return 1;
    }
    return 0;
}

/* displaying player */
void player_display(struct player *player) {
    assert(player);
    window_display_image(sprite_get_player(player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


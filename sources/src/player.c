/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/player.h"
#include "../include/sprite.h"
#include "../include/game.h"
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
    int life; /* number of lives */
    int keys; /* number of keys */
    int t0; /* timer */
};

int player_get_t0(struct player *player) {
    assert(player);
    return player->t0;
}

void player_set_t0(struct player *player, int t0) {
    assert(player);
    player->t0 = t0;
}

struct player *player_init(int bombs) {
    struct player *player = malloc(sizeof(struct player));
    if (!player) {
        error("Memory error");
    }
    player->direction = NORTH;
    player->bombs = bombs;
    player->range = 1;
    player->life = 3;
    player->keys = 0;
    player->t0 = 0;
    player->x = 1;
    player->y = 0;
    return player;
}

void player_free(struct player *player) {
    assert(player);
    free(player);
}

int player_get_x(struct player *player) {
    assert(player != NULL);
    return player->x;
}

int player_get_y(struct player *player) {
    assert(player != NULL);
    return player->y;
}

void player_set_current_way(struct player *player, enum direction way) {
    assert(player);
    player->direction = way;
}

int player_get_nb_bomb(struct player *player) {
    assert(player);
    return player->bombs;
}

void player_inc_nb_bomb(struct player *player) {
    assert(player);
    if (player->bombs < 9)
        player->bombs++;
}

void player_dec_nb_bomb(struct player *player) {
    assert(player);
    if (player->bombs > 0)
        player->bombs--;
}

int player_get_nb_life(struct player *player) {
    assert(player);
    return player->life;
}

void player_dec_nb_life(struct player *player) {
    assert(player);
    if (player->life > 0)
        player->life--;
}

void player_inc_nb_life(struct player *player) {
    assert(player);
    if (player->life < 9)
        player->life++;
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
    if (player->range < 9)
        player->range++;
}

void player_dec_range(struct player *player) {
    assert(player);
    if (player->range > 1)
        player->range--;
}

int player_get_nb_keys(struct player *player) {
    assert(player);
    return player->keys;
}

void player_inc_nb_keys(struct player *player) {
    assert(player);
    if (player->keys < 9)
        player->keys++;
}

void player_dec_nb_keys(struct player *player) {
    assert(player);
    if (player->keys > 0)
        player->keys--;
}

void player_change_level(struct game *game, int level) {
    /* changing level */
    game_set_level(game, level);
    game_get_player(game)->bombs = 9;
    /* loading monsters in monster_array */
    map_set_monsters(game_get_current_map(game));
}

int box_meets_monster(struct map *map, int x, int y) {
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

int player_can_push_box(struct game *game, int x_src, int y_src) {
    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);
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

void player_get_bonus(struct game *game, int x, int y, enum cell_type type) {
    struct player *player = game_get_player(game);
    enum bonus_type bonus_type = type & 0x0f;
    if (bonus_type == BONUS_BOMB_RANGE_INC) {
        player_inc_range(player);
    } else if (bonus_type == BONUS_BOMB_RANGE_DEC) {
        player_dec_range(player);
    } else if (bonus_type == BONUS_BOMB_NB_DEC) {
        player_dec_nb_bomb(player);
    } else if (bonus_type == BONUS_BOMB_NB_INC) {
        player_inc_nb_bomb(player);
    } else if (bonus_type == BONUS_LIFE) {
        player_inc_nb_life(player);
    }

    map_set_cell_type(game_get_current_map(game), x, y, CELL_EMPTY);
}

void player_open_door(struct map *map) {
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            enum cell_type type = map_get_cell_value(map, i, j);
            if ((type & 0xf0) == CELL_DOOR && (type & 0x01) == CLOSED) {
                map_set_cell_type(map, i, j, type & 0xfe);
                break;
            }
        }
    }
}

int player_meets_monster(struct map *map, int x, int y) {
    struct monster **monster_array = map_get_monster_array(map);
    for (int i = 0; i < NUM_MONSTER_MAX; i++) {
        if (monster_array[i] != NULL) {
            if (monster_get_x(monster_array[i]) == x && monster_get_y(monster_array[i]) == y)
                return 1;
        }
    }
    return 0;
}

/* allow player to move */
static int player_move_aux(struct game *game, int x, int y) {
    struct player *player = game_get_player(game);
    struct map *map = game_get_current_map(game);

    if (!map_is_inside(map, x, y))
        return 0;

    if (player_meets_monster(map, x, y)) {
        if (SDL_GetTicks() - player->t0 > 1000) {
            player_dec_nb_life(player);
            player->t0 = SDL_GetTicks();
        }
        return 0;
    }
    enum cell_type cell = map_get_cell_value(map, x, y);
    switch (cell & 0xf0) {
        /* if player goes to CELL_SCENERY */
        case CELL_SCENERY:
            if ((cell & 0x0f) == SCENERY_PRINCESS)
                return 1;

            return 0;
            /* if player goes to CELL_BOX */
        case CELL_BOX:
            /* checking if player can push the box */
            if (player_can_push_box(game, x, y))
                return 1;

            return 0;
            /* if player goes to CELL_BONUS */
        case CELL_BONUS:
            player_get_bonus(game, x, y, cell);
            return 1;
            /* if player goes to CELL_BOMB */
        case CELL_BOMB:
            if ((cell & 0x0f) == EXPLOSION)
                player_dec_nb_life(player);

            return 1;
            /* if player goes to CELL_KEY */
        case CELL_KEY:
            player_inc_nb_keys(player);
            map_set_cell_type(map, x, y, CELL_EMPTY);
            /* setting door as OPENED */
            player_open_door(map);
            return 1;
            /* if player goes to CELL_DOOR */
        case CELL_DOOR:
            if ((cell & 0x01) == OPENED) {
                player_dec_nb_keys(player);
                /* level of next level */
                int level = (cell & 0x0e) / 2;
                player_change_level(game, level);
                return 1;
            }
            return 0;
        default:
            break;
    }
    return 1;
}

void player_move(struct game *game) {
    struct player *player = game_get_player(game);
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

    if (player_move_aux(game, x, y)) {
        player->x = x;
        player->y = y;
    }
}

/* displaying player */
void player_display(struct player *player) {
    assert(player);
    window_display_image(sprite_get_player(player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


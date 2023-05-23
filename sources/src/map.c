#include "../include/map.h"
#include "../include/monster.h"
#include "../include/bomb.h"
#include "../include/window.h"
#include "../include/sprite.h"
#include "../include/constant.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Structure representing a map.
 */
struct map {
    int width; /**< Width of the map */
    int height; /**< Height of the map */
    unsigned char *grid; /**< Grid of the map */
    struct bomb *list_bombs[NUM_BOMBS_MAX]; /**< List of bombs on the map */
    struct monster *list_monsters[NUM_MONSTERS_MAX]; /**< List of monsters on the map */
};

struct map *map_new(int width, int height) {
    assert(width > 0 && height > 0);

    struct map *map = malloc(sizeof(struct map));
    if (map == NULL) {
        perror("map_new : malloc map failed");
    }

    map->width = width;
    map->height = height;
    map->grid = malloc(height * width);
    if (!map->grid) {
        perror("map_new : malloc grid failed");
    }

    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            map->grid[CELL(i, j)] = CELL_EMPTY;
        }
    }

    return map;
}

void map_free(struct map *map) {
    assert(map);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (map->list_monsters[i] != NULL) {
            monster_free(map->list_monsters[i]);
        }
    }
    for (int j = 0; j < NUM_BOMBS_MAX; j++) {
        if (map->list_bombs[j] != NULL) {
            bomb_free(map->list_bombs[j]);
        }
    }
    free(map->grid);
    free(map);
}

int map_get_width(struct map *map) {
    assert(map);
    return map->width;
}

int map_get_height(struct map *map) {
    assert(map);
    return map->height;
}

unsigned char *map_get_grid(struct map *map) {
    assert(map);
    return map->grid;
}

void map_set_grid(struct map *map, unsigned char *grid) {
    assert(map);
    assert(grid);
    map->grid = grid;
}

struct bomb **map_get_list_bombs(struct map *map) {
    assert(map);
    return map->list_bombs;
}

struct monster **map_get_list_monsters(struct map *map) {
    assert(map);
    return map->list_monsters;
}

int map_get_size() {
    return sizeof(struct map);
}

int map_is_inside(struct map *map, int x, int y) {
    assert(map);
    if (x >= 0 && x < map->width && y >= 0 && y < map->height) {
        return 1;
    }
    return 0;
}

uint8_t map_get_cell_value(struct map *map, int x, int y) {
    assert(map);
    assert(map->grid);
    assert(map_is_inside(map, x, y));
    return map->grid[CELL(x, y)];
}

void map_set_cell_value(struct map *map, int x, int y, uint8_t value) {
    assert(map);
    assert(map_is_inside(map, x, y));
    map->grid[CELL(x, y)] = value;
}

void map_display(struct map *map) {
    assert(map);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            int x = i * SIZE_BLOC;
            int y = j * SIZE_BLOC;

            uint8_t type = map_get_cell_value(map, i, j);

            switch ((enum cell_type) (type & 0xf0)) {
                case CELL_SCENERY:
                    window_display_image(sprite_get_scenery((enum scenery_type) (type & 0x0f)), x, y);
                    break;

                case CELL_BOX:
                    window_display_image(sprite_get_box(), x, y);
                    break;

                case CELL_BONUS:
                    window_display_image(sprite_get_bonus((enum bonus_type) (type & 0x0f)), x, y);
                    break;

                case CELL_KEY:
                    window_display_image(sprite_get_key(), x, y);
                    break;

                case CELL_DOOR:
                    window_display_image(sprite_get_door((enum door_status) (type & 0x01)), x, y);
                    break;

                case CELL_BOMB:
                    window_display_image(sprite_get_bomb((type & 0x0f)), x, y);
                    break;

                default:
                    break;
            }
        }
    }
}

void map_init_list_monsters(struct map *map) {
    assert(map);
    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_MONSTER) {
                struct monster *monster = monster_init(i, j, DURATION_MONSTER_MOVE);
                timer_start(monster_get_timer(monster), DURATION_MONSTER_MOVE);
                struct monster **list_monsters = map_get_list_monsters(map);
                for (int k = 0; k < NUM_MONSTERS_MAX; k++) {
                    if (list_monsters[k] == NULL) {
                        list_monsters[k] = monster;
                        break;
                    }
                }
                map_set_cell_value(map, i, j, CELL_EMPTY);
            }
        }
    }
}

void map_set_bomb(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    if ((map_get_cell_value(map, player_get_x(player), player_get_y(player)) & 0xf0) == CELL_DOOR) {
        return;
    }

    if (player_get_num_bomb(player) > 0) {
        struct bomb *bomb = malloc(bomb_get_size());
        if (!bomb) {
            perror("malloc");
        }
        memset(bomb, 0, bomb_get_size());
        bomb_init(bomb, player_get_x(player), player_get_y(player), player_get_range_bombs(player));
        struct bomb **list_bombs = map_get_list_bombs(map);
        for (int i = 0; i < NUM_BOMBS_MAX; i++)
            if (list_bombs[i] == NULL) {
                list_bombs[i] = bomb;
                break;
            }
        player_dec_num_bomb(player);
    }
}

static void set_monster(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));
    struct monster *monster = monster_init(x, y, DURATION_MONSTER_MOVE);
    timer_start(monster_get_timer(monster), DURATION_MONSTER_MOVE);
    struct monster **list_monsters = map_get_list_monsters(map);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] == NULL) {
            list_monsters[i] = monster;
            break;
        }
    }
}

static void set_bonus(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));
    enum bonus_type bonus_type = map_get_cell_value(map, x, y) & 0x0f;
    if (bonus_type == EMPTY) {
        bonus_type = rand() % NUM_BONUS_TYPES + 1;
    }
    if (bonus_type == BONUS_MONSTER) {
        set_monster(map, x, y);
        map_set_cell_value(map, x, y, CELL_EMPTY);
    } else {
        map_set_cell_value(map, x, y, CELL_BONUS | bonus_type);
    }
}

static void kill_monster(struct map *map, struct monster **monster) {
    assert(monster);
    assert(*monster);
    assert(map);
    map_set_cell_value(map, monster_get_x(*monster), monster_get_y(*monster), CELL_EMPTY);
    monster_free(*monster);
    *monster = NULL;
}

static struct monster **is_explosion_colliding_monster(int x_explosion, int y_explosion, struct monster **list_monsters) {
    assert(list_monsters);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] && monster_get_x(list_monsters[i]) == x_explosion &&
            monster_get_y(list_monsters[i]) == y_explosion) {
            return &list_monsters[i];
        }
    }
    return NULL;
}

static int is_explosion_colliding_player(int explosion_x, int explosion_y, struct player *player) {
    assert(player);
    if (explosion_x == player_get_x(player) && explosion_y == player_get_y(player)) {
        return 1;
    }
    return 0;
}

static int can_bomb_propagate(enum cell_type cell_type) {
    if (cell_type != CELL_SCENERY && cell_type != CELL_DOOR && cell_type != CELL_KEY) {
        return 1;
    }
    return 0;
}

static void propagate_bomb_explosion(struct map *map, struct player *player, struct bomb *bomb, enum direction dir) {
    assert(map);
    assert(player);
    assert(bomb);
    int x = bomb_get_x(bomb);
    int y = bomb_get_y(bomb);
    int range;
    int *range_ptr;
    for (range = 1; range <= bomb_get_range(bomb); range++) {
        if (dir == NORTH) {
            y--;
            range_ptr = bomb_get_north_range_ptr(bomb);
        } else if (dir == SOUTH) {
            y++;
            range_ptr = bomb_get_south_range_ptr(bomb);
        } else if (dir == EAST) {
            x++;
            range_ptr = bomb_get_east_range_ptr(bomb);
        } else if (dir == WEST) {
            x--;
            range_ptr = bomb_get_west_range_ptr(bomb);
        }

        if (map_is_inside(map, x, y)) {
            enum cell_type cell_type = map_get_cell_value(map, x, y) & 0xf0;
            struct monster **dead_monster = NULL;
            if (can_bomb_propagate(cell_type)) {
                if (cell_type == CELL_BOX) {
                    set_bonus(map, x, y);
                    *range_ptr = range - 1;
                    return;
                } else if (is_explosion_colliding_player(x, y, player)) {
                    player_dec_num_lives(player);
                    *range_ptr = range - 1;
                    return;
                } else if ((dead_monster = is_explosion_colliding_monster(x, y, map_get_list_monsters(map))) != NULL) {
                    kill_monster(map, dead_monster);
                    map_set_cell_value(map, x, y, CELL_BOMB | EXPLOSION);
                    *range_ptr = range;
                    return;
                } else {
                    map_set_cell_value(map, x, y, CELL_BOMB | EXPLOSION);
                }
            }
            else {
                *range_ptr = range - 1;
                return;
            }
        }
        else {
            *range_ptr = range - 1;
            return;
        }
    }
    *range_ptr = range - 1;
}

static void clean_explosion_cells(struct map *map, struct bomb *bomb) {
    assert(map);
    assert(bomb);
    int x = bomb_get_x(bomb);
    int y = bomb_get_y(bomb);
    map_set_cell_value(map, x, y, CELL_EMPTY);
    for (int i = 1; i <= bomb_get_north_range(bomb); i++) {
        map_set_cell_value(map, x, y - i, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb_get_south_range(bomb); i++) {
        map_set_cell_value(map, x, y + i, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb_get_east_range(bomb); i++) {
        map_set_cell_value(map, x + i, y, CELL_EMPTY);
    }
    for (int i = 1; i <= bomb_get_west_range(bomb); i++) {
        map_set_cell_value(map, x - i, y, CELL_EMPTY);
    }
}

void map_update_bombs(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    struct bomb **list_bombs = map_get_list_bombs(map);
    for (int i = 0; i < NUM_BOMBS_MAX; i++) {
        if (list_bombs[i] != NULL) {
            struct bomb *bomb = list_bombs[i];
            timer_update(bomb_get_timer(bomb));
            if (timer_get_state(bomb_get_timer(bomb)) == IS_OVER) {
                if (bomb_get_ttl(bomb) <= TTL4 && bomb_get_ttl(bomb) > TTL1) {
                    map_set_cell_value(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_BOMB | bomb_get_ttl(bomb));
                } else if (bomb_get_ttl(bomb) == TTL1) {
                    if (is_explosion_colliding_player(bomb_get_x(bomb), bomb_get_y(bomb), player)) {
                        player_dec_num_lives(player);
                    }
                    map_set_cell_value(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_BOMB | EXPLOSION);
                    propagate_bomb_explosion(map, player, bomb, NORTH);
                    propagate_bomb_explosion(map, player, bomb, SOUTH);
                    propagate_bomb_explosion(map, player, bomb, EAST);
                    propagate_bomb_explosion(map, player, bomb, WEST);
                } else if (bomb_get_ttl(bomb) == EXPLOSION) {
                    clean_explosion_cells(map, bomb);
                    free(bomb_get_timer(bomb));
                    free(bomb);
                    list_bombs[i] = NULL;
                    continue;
                }
                bomb_dec_ttl(bomb);
                timer_start(bomb_get_timer(bomb), DURATION_BOMB_TTL);
            }
        }
    }
}

static int is_box_colliding_monsters(struct monster **list_monsters, int x_dest, int y_dest) {
    assert(list_monsters);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            if (monster_get_x(list_monsters[i]) == x_dest && monster_get_y(list_monsters[i]) == y_dest) {
                return 1;
            }
        }
    }
    return 0;
}

static int is_box_pushable(struct map *map, int x_dest, int y_dest) {
    assert(map);

    if (!map_is_inside(map, x_dest, y_dest)) {
        return 0;
    }
    if ((map_get_cell_value(map, x_dest, y_dest) & 0xf0) == CELL_EMPTY &&
        !is_box_colliding_monsters(map_get_list_monsters(map), x_dest, y_dest)) {
        return 1;
    }
    return 0;
}

static int is_player_colliding_monsters(int player_next_x, int player_next_y, struct monster **list_monsters) {
    assert(list_monsters);

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            if (monster_get_x(list_monsters[i]) == player_next_x && monster_get_y(list_monsters[i]) == player_next_y) {
                return 1;
            }
        }
    }
    return 0;
}

static int can_player_move(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    int next_x = direction_get_x(player_get_x(player), player_get_direction(player), 1);
    int next_y = direction_get_y(player_get_y(player), player_get_direction(player), 1);


    if (!map_is_inside(map, next_x, next_y)) {
        return 0;
    }

    if (is_player_colliding_monsters(next_x, next_y, map_get_list_monsters(map))) {
        timer_update(player_get_timer_invincibility(player));
        if (timer_get_state(player_get_timer_invincibility(player)) == IS_OVER) {
            player_dec_num_lives(player);
            timer_start(player_get_timer_invincibility(player), DURATION_PLAYER_INVINCIBILITY);
        }
        return 0;
    }
    uint8_t cell = map_get_cell_value(map, next_x, next_y);
    switch (cell & 0xf0) {
        case CELL_SCENERY:
            if ((cell & 0x0f) == SCENERY_PRINCESS) {
                return 1;
            }

            return 0;
        case CELL_BOX: {
            int x_dest = direction_get_x(player_get_x(player), player_get_direction(player), 2);
            int y_dest = direction_get_y(player_get_y(player), player_get_direction(player), 2);
            if (is_box_pushable(map, x_dest, y_dest)) {
                map_set_cell_value(map, x_dest, y_dest, CELL_BOX);
                map_set_cell_value(map, next_x, next_y, CELL_EMPTY);
                return 1;
            }
            return 0;
        }
        case CELL_BONUS:
            player_get_bonus(player, cell & 0x0f);
            map_set_cell_value(map, next_x, next_y, CELL_EMPTY);
            return 1;
        case CELL_BOMB:
            if ((cell & 0x0f) == EXPLOSION) {
                timer_update(player_get_timer_invincibility(player));
                if (timer_get_state(player_get_timer_invincibility(player)) == IS_OVER) {
                    player_dec_num_lives(player);
                    timer_start(player_get_timer_invincibility(player), DURATION_PLAYER_INVINCIBILITY);
                }
            }
            return 1;
        case CELL_KEY:
            player_inc_num_keys(player);
            map_set_cell_value(map, next_x, next_y, CELL_EMPTY);
            return 1;
        case CELL_DOOR:
            if ((cell & 0x01) == OPEN) {
                return 1;
            }
            return 0;
        default:
            break;
    }
    return 1;
}

int map_move_player(struct map *map, struct player *player) {
    assert(player);
    assert(map);

    if (can_player_move(map, player)) {
        player_set_x(player, direction_get_x(player_get_x(player), player_get_direction(player), 1));
        player_set_y(player, direction_get_y(player_get_y(player), player_get_direction(player), 1));
        return 1;
    }
    return 0;
}

void map_display_monsters(struct map *map) {
    assert(map);
    struct monster **list_monsters = map_get_list_monsters(map);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            window_display_image(sprite_get_monster(monster_get_direction(list_monsters[i])), monster_get_x(list_monsters[i]) * SIZE_BLOC, monster_get_y(list_monsters[i]) * SIZE_BLOC);
        }
    }
}

static int is_monster_colliding_player(struct monster *monster, struct player *player) {
    assert(monster);
    assert(player);
    if (direction_get_x(monster_get_x(monster), monster_get_direction(monster), 1) == player_get_x(player) && direction_get_y(monster_get_y(monster), monster_get_direction(monster), 1) == player_get_y(player)) {
        return 1;
    }

    return 0;
}

static int is_monster_colliding_monsters(struct monster **list_monsters, struct monster *monster) {
    assert(list_monsters);
    assert(monster);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            if (monster_get_x(list_monsters[i]) == direction_get_x(monster_get_x(monster), monster_get_direction(monster), 1) && monster_get_y(list_monsters[i]) == direction_get_y(monster_get_y(monster), monster_get_direction(monster), 1)) {
                return 1;
            }
        }
    }
    return 0;
}

static int can_monster_move(struct map *map, struct player *player, struct monster *monster) {
    assert(map);
    assert(player);
    assert(monster);

    int next_x = direction_get_x(monster_get_x(monster), monster_get_direction(monster), 1);
    int next_y = direction_get_y(monster_get_y(monster), monster_get_direction(monster), 1);

    if (!map_is_inside(map, next_x, next_y)) {
        return 0;
    }

    if (is_monster_colliding_player(monster, player)) {
        struct timer *timer_invincibility = player_get_timer_invincibility(player);
        timer_update(timer_invincibility);
        if (timer_get_state(timer_invincibility) == IS_OVER) {
            player_dec_num_lives(player);
            timer_start(timer_invincibility, DURATION_PLAYER_INVINCIBILITY);
        }
        return 0;
    }

    if (is_monster_colliding_monsters(map_get_list_monsters(map), monster)) {
        return 0;
    }

    switch (map_get_cell_value(map, next_x, next_y) & 0xf0) {
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

static void map_move_monster(struct map *map, struct monster *monster, struct player *player) {
    assert(monster);
    assert(map);
    assert(player);

    if (can_monster_move(map, player, monster)) {
        monster_set_x(monster, direction_get_x(monster_get_x(monster), monster_get_direction(monster), 1));
        monster_set_y(monster, direction_get_y(monster_get_y(monster), monster_get_direction(monster), 1));
    }
}

void map_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);
    struct monster **list_monsters = map_get_list_monsters(map);
    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            struct monster *monster = list_monsters[i];
            timer_update(monster_get_timer(monster));
            if (timer_get_state(monster_get_timer(monster)) == IS_OVER) {
                int random_dir = rand() % 4;
                monster_set_direction(monster, (enum direction) random_dir);
                map_move_monster(map, monster, player);
                timer_start(monster_get_timer(monster), DURATION_MONSTER_MOVE);
            }
        }
    }
}

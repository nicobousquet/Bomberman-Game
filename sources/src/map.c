#include "../include/map.h"
#include "../include/monster.h"
#include "../include/bomb.h"
#include "../include/constant.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

/**
 * @brief Structure representing a map.
 */
struct map {
    int width; /**< Width of the map */
    int height; /**< Height of the map */
    uint8_t *grid; /**< Grid of the map */
    struct bomb *list_bombs[NUM_BOMBS_MAX]; /**< List of bombs on the map */
    struct monster *list_monsters[NUM_MONSTERS_MAX]; /**< List of monsters on the map */
    enum strategy monsters_strategy; /**< The strategy of the monsters (RANDOM, DIJKSTRA) */
};

struct map *map_new(char *filename) {
    assert(filename);

    struct map *map = malloc(sizeof(*map));

    memset(map, 0, sizeof(*map));

    if (map == NULL) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filename, "rb");

    if (!fp) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);

    long size = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    char *file_content = malloc(size);
    char *file_content_copy = file_content;

    if (!file_content) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    size_t numread = fread(file_content, sizeof(*file_content), size, fp);

    if ((long) numread != size) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    map->width = (int) strtol(file_content, &file_content, 10);
    map->height = (int) strtol(file_content + 1, &file_content, 10);
    map->monsters_strategy = strtol(file_content, &file_content, 10);

    map->grid = malloc(sizeof(uint8_t) * (map->width * map->height));

    if (!map->grid) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < map->width * map->height; i++) {
        map->grid[i] = (uint8_t) strtol(file_content, &file_content, 10);
    }

    free(file_content_copy);

    for (int i = 0; i < NUM_BOMBS_MAX; i++) {
        map->list_bombs[i] = NULL;
    }

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        map->list_monsters[i] = NULL;
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

void map_write(struct map *map, FILE *file) {
    assert(map);
    assert(file);

    fwrite(map, sizeof(*map), 1, file);
    fwrite(map->grid, map->width * map->height, 1, file);


    for (int j = 0; j < NUM_BOMBS_MAX; j++) {
        if (map->list_bombs[j] != NULL) {
            bomb_write(map->list_bombs[j], file);
        }
    }

    for (int j = 0; j < NUM_MONSTERS_MAX; j++) {
        if (map->list_monsters[j] != NULL) {
            monster_write(map->list_monsters[j], file);
        }
    }
}

void map_read(struct map *map, FILE *file) {
    assert(map);

    uint8_t *grid = map->grid;

    fread(map, sizeof(*map), 1, file);
    map->grid = grid;
    fread(map->grid, map->width * map->height, 1, file);

    for (int i = 0; i < NUM_BOMBS_MAX; i++) {
        if (map->list_bombs[i] != NULL) {
            map->list_bombs[i] = bomb_new(0, 0, 1);
            bomb_read(map->list_bombs[i], file);
        }
    }

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (map->list_monsters[i] != NULL) {
            map->list_monsters[i] = monster_new(0, 0);
            monster_read(map->list_monsters[i], file);
        }
    }
}

int map_get_width(struct map *map) {
    assert(map);
    return map->width;
}

int map_get_height(struct map *map) {
    assert(map);
    return map->height;
}

uint8_t *map_get_grid(struct map *map) {
    assert(map);
    return map->grid;
}

void map_set_grid(struct map *map, uint8_t *grid) {
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

void map_display(struct map *map, struct SDL_Surface *window, struct sprites *sprites) {
    assert(map);
    assert(window);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            int x = i * SIZE_BLOC;
            int y = j * SIZE_BLOC;

            uint8_t type = map_get_cell_value(map, i, j);

            switch ((enum cell_type) (type & 0xf0)) {
                case CELL_SCENERY:
                    window_display_image(window, sprites_get_scenery(sprites, (enum scenery_type) (type & 0x0f)), x, y);
                    break;

                case CELL_BOX:
                    window_display_image(window, sprites_get_box(sprites), x, y);
                    break;

                case CELL_BONUS:
                    window_display_image(window, sprites_get_bonus(sprites, (enum bonus_type) (type & 0x0f)), x, y);
                    break;

                case CELL_KEY:
                    window_display_image(window, sprites_get_key(sprites), x, y);
                    break;

                case CELL_DOOR:
                    window_display_image(window, sprites_get_door(sprites, (enum door_status) (type & 0x01)), x, y);
                    break;

                case CELL_BOMB:
                    window_display_image(window, sprites_get_bomb(sprites, (type & 0x0f)), x, y);
                    break;

                default:
                    break;
            }
        }
    }

    struct monster **list_monsters = map_get_list_monsters(map);

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            monster_display(list_monsters[i], window, sprites);
        }
    }
}

static void add_monster(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {

        struct monster **list_monsters = map_get_list_monsters(map);

        if (list_monsters[i] == NULL) {
            list_monsters[i] = monster_new(x, y);

            break;
        }
    }
}

void map_init_list_monsters(struct map *map) {
    assert(map);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {

            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_MONSTER) {

                add_monster(map, i, j);

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

        struct bomb **list_bombs = map_get_list_bombs(map);

        for (int i = 0; i < NUM_BOMBS_MAX; i++)
            if (list_bombs[i] == NULL) {
                list_bombs[i] =  bomb_new(player_get_x(player), player_get_y(player), player_get_range_bombs(player));
                player_dec_num_bomb(player);

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
        add_monster(map, x, y);
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

static struct monster **is_explosion_reaching_monster(int x_explosion, int y_explosion, struct monster **list_monsters) {
    assert(list_monsters);

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] && monster_get_x(list_monsters[i]) == x_explosion && monster_get_y(list_monsters[i]) == y_explosion) {
            return &list_monsters[i];
        }
    }

    return NULL;
}

static int is_explosion_reaching_player(int explosion_x, int explosion_y, struct player *player) {
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

static void propagate_bomb_explosion(struct map *map, struct player *player, struct bomb *list_bombs[NUM_BOMBS_MAX], struct bomb *current_bomb, enum direction dir) {
    assert(map);
    assert(player);
    assert(current_bomb);

    for (int range = 1; range <= bomb_get_range(current_bomb); range++) {

        int x = direction_get_x(dir, bomb_get_x(current_bomb), range);
        int y = direction_get_y(dir, bomb_get_y(current_bomb), range);

        if (map_is_inside(map, x, y)) {

            uint8_t cell_value = map_get_cell_value(map, x, y);

            if (can_bomb_propagate(cell_value & 0xf0)) {

                struct monster **dead_monster = NULL;

                if ((cell_value & 0xf0) == CELL_BOX) {
                    set_bonus(map, x, y);
                    return;

                } else if ((cell_value & 0xf0) == CELL_BOMB && cell_value != (CELL_BOMB | EXPLOSING)) {
                    for (int i = 0; i < NUM_BOMBS_MAX; i++) {
                        if (list_bombs[i]) {
                            if (list_bombs[i] != current_bomb && bomb_get_x(list_bombs[i]) == x && bomb_get_y(list_bombs[i]) == y) {
                                bomb_set_ttl(list_bombs[i], TTL1);
                                timer_start(bomb_get_timer(list_bombs[i]), 60);
                            }
                        }
                    }
                } else if (is_explosion_reaching_player(x, y, player)) {
                    player_dec_num_lives(player);
                    return;

                } else if ((dead_monster = is_explosion_reaching_monster(x, y, map_get_list_monsters(map))) != NULL) {
                    kill_monster(map, dead_monster);
                    map_set_cell_value(map, x, y, CELL_BOMB | EXPLOSING);
                    return;

                } else {
                    map_set_cell_value(map, x, y, CELL_BOMB | EXPLOSING);
                }

            } else {
                return;

            }
        } else {
            return;

        }
    }
}

static void clean_explosion_cells(struct map *map, struct bomb *bomb, enum direction explosion_direction) {
    assert(map);
    assert(bomb);

    for (int range = 1; range <= bomb_get_range(bomb); range++) {
        if (map_is_inside(map, direction_get_x(explosion_direction, bomb_get_x(bomb), range), direction_get_y(explosion_direction, bomb_get_y(bomb), range))) {
            if (map_get_cell_value(map, direction_get_x(explosion_direction, bomb_get_x(bomb), range), direction_get_y(explosion_direction, bomb_get_y(bomb), range)) == (CELL_BOMB | EXPLOSING)) {
                map_set_cell_value(map, direction_get_x(explosion_direction, bomb_get_x(bomb), range), direction_get_y(explosion_direction, bomb_get_y(bomb), range), CELL_EMPTY);
                continue;
            }
            return;
        }
        return;
    }
}

void map_update_bombs(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    struct bomb **list_bombs = map_get_list_bombs(map);

    for (int i = 0; i < NUM_BOMBS_MAX; i++) {
        if (list_bombs[i] != NULL) {
            struct bomb *current_bomb = list_bombs[i];

            timer_update(bomb_get_timer(current_bomb));

            if (timer_get_state(bomb_get_timer(current_bomb)) == IS_OVER) {

                bomb_dec_ttl(current_bomb);

                if (bomb_get_ttl(current_bomb) <= TTL4 && bomb_get_ttl(current_bomb) >= TTL1) {
                    map_set_cell_value(map, bomb_get_x(current_bomb), bomb_get_y(current_bomb), CELL_BOMB | bomb_get_ttl(current_bomb));

                } else if (bomb_get_ttl(current_bomb) == EXPLOSING) {

                    if (is_explosion_reaching_player(bomb_get_x(current_bomb), bomb_get_y(current_bomb), player)) {
                        player_dec_num_lives(player);
                    }

                    map_set_cell_value(map, bomb_get_x(current_bomb), bomb_get_y(current_bomb), CELL_BOMB | EXPLOSING);

                    propagate_bomb_explosion(map, player, list_bombs, current_bomb, NORTH);
                    propagate_bomb_explosion(map, player, list_bombs, current_bomb, SOUTH);
                    propagate_bomb_explosion(map, player, list_bombs, current_bomb, EAST);
                    propagate_bomb_explosion(map, player, list_bombs, current_bomb, WEST);

                } else {
                    map_set_cell_value(map, bomb_get_x(current_bomb), bomb_get_y(current_bomb), CELL_EMPTY);

                    clean_explosion_cells(map, current_bomb, NORTH);
                    clean_explosion_cells(map, current_bomb, SOUTH);
                    clean_explosion_cells(map, current_bomb, EAST);
                    clean_explosion_cells(map, current_bomb, WEST);

                    bomb_free(current_bomb);
                    list_bombs[i] = NULL;

                    continue;
                }

                timer_start(bomb_get_timer(current_bomb), DURATION_BOMB_PERIOD);
            }
        }
    }
}

static int will_box_be_blocked_by_monsters(struct monster **list_monsters, int x_dest, int y_dest) {
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

    if ((map_get_cell_value(map, x_dest, y_dest) & 0xf0) == CELL_EMPTY && !will_box_be_blocked_by_monsters(map_get_list_monsters(map), x_dest, y_dest)) {
        return 1;
    }

    return 0;
}

static int will_player_meet_a_monster(int player_next_x, int player_next_y, struct monster **list_monsters) {
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

static int can_player_move(struct map *map, struct player *player, enum direction direction) {
    assert(map);
    assert(player);

    int next_x = direction_get_x(direction, player_get_x(player), 1);
    int next_y = direction_get_y(direction, player_get_y(player), 1);


    if (!map_is_inside(map, next_x, next_y)) {
        return 0;
    }

    if (will_player_meet_a_monster(next_x, next_y, map_get_list_monsters(map))) {
        player_dec_num_lives(player);
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

            int x_dest = direction_get_x(direction, player_get_x(player), 2);
            int y_dest = direction_get_y(direction, player_get_y(player), 2);

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

            if ((cell & 0x0f) == EXPLOSING) {
                player_dec_num_lives(player);
            }

            return 1;

        case CELL_KEY:
            player_inc_num_keys(player);
            map_set_cell_value(map, next_x, next_y, CELL_EMPTY);
            return 1;

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

int map_move_player(struct map *map, struct player *player, enum direction direction) {
    assert(player);
    assert(map);

    if (can_player_move(map, player, direction)) {
        player_move(player, direction);
        return 1;
    }

    return 0;
}

static int will_monster_meet_player(int monster_x, int monster_y, struct player *player) {
    assert(player);

    if (monster_x == player_get_x(player) && monster_y == player_get_y(player)) {
        return 1;
    }

    return 0;
}

static int will_monster_meet_other_monsters(int monster_x, int monster_y, struct monster **list_monsters) {
    assert(list_monsters);

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            if (monster_get_x(list_monsters[i]) == monster_x && monster_get_y(list_monsters[i]) == monster_y) {
                return 1;
            }
        }
    }

    return 0;
}

static int can_monster_move(struct map *map, struct player *player, struct monster *monster, enum direction direction) {
    assert(map);
    assert(player);
    assert(monster);

    int next_x = direction_get_x(direction, monster_get_x(monster), 1);
    int next_y = direction_get_y(direction, monster_get_y(monster), 1);

    if (!map_is_inside(map, next_x, next_y)) {
        return 0;
    }

    if (will_monster_meet_other_monsters(next_x, next_y, map_get_list_monsters(map))) {
        return 0;
    }

    switch (map_get_cell_value(map, next_x, next_y) & 0xf0) {

        case CELL_SCENERY:
        case CELL_BOMB:
        case CELL_DOOR:
        case CELL_BOX:
            return 0;

        default:
            return 1;
    }
}

static void monster_meeting_player(struct monster *monster, struct player *player, enum direction monster_direction) {
    monster_set_direction(monster, monster_direction);

    player_dec_num_lives(player);

    timer_start(monster_get_timer(monster), DURATION_MONSTER_MOVE);
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

                int grid[map_get_width(map) * map_get_height(map)];

                for (int j = 0; j < map_get_width(map); j++) {
                    for (int k = 0; k < map_get_height(map); k++) {
                        switch (map_get_cell_value(map, j, k) & 0xf0) {
                            case CELL_SCENERY:
                            case CELL_BOMB:
                            case CELL_DOOR:
                            case CELL_BOX:
                                grid[CELL(j, k)] = 1;
                                break;

                            default:
                                grid[CELL(j, k)] = 0;
                                break;
                        }
                    }
                }

                enum direction monster_direction;

                if (map->monsters_strategy == RANDOM_STRATEGY) {
                    monster_direction = direction_get_random(monster_get_x(monster), monster_get_y(monster), grid, map_get_width(map), map_get_height(map));
                }

                if (can_monster_move(map, player, monster, monster_direction)) {
                    if (will_monster_meet_player(direction_get_x(monster_direction, monster_get_x(monster), 1), direction_get_y(monster_direction, monster_get_y(monster), 1), player)) {
                        monster_meeting_player(monster, player, monster_direction);
                    } else {
                        monster_move(monster, monster_direction);
                    }
                }
            }
        }
    }
}

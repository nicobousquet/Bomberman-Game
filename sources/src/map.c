#include "../include/map.h"
#include "../include/constant.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Macro to calculate the index of a cell in the map given its row and column.
 */
#define CELL(i, j) ((i) + (j) * map->width)

/**
 * @brief Structure representing a map.
 */
struct map {
    int width; /**< Width of the map */
    int height; /**< Height of the map */
    unsigned char *grid; /**< Grid of the map */
    struct bomb_node *bomb_head; /**< Head of the bombs' linked list */
    struct monster_node *monster_head; /**< Head of the monsters' linked list */
    enum strategy monsters_strategy; /**< The strategy of the monsters (RANDOM, DIJKSTRA) */
};

struct map *map_new(char *filename) {
    assert(filename);

    struct map *map = malloc(sizeof(struct map));

    if (!map) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(map, 0, sizeof(struct map));

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen map file");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fp, "%i:%i\n%hhu\n", &(map->width), &(map->height), (unsigned char *) &(map->monsters_strategy)) != 3) {
        perror("Error reading map dimensions and monsters strategy");
        exit(EXIT_FAILURE);
    }

    map->grid = malloc(sizeof(unsigned char) * (map->width * map->height));

    if (!map->grid) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < map->width * map->height; i++) {
        if (fscanf(fp, "%hhu", &(map->grid[i])) != 1) {
            perror("Error reading map grid");
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);

    map->bomb_head = NULL;
    map->monster_head = NULL;

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {

            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_BOMB) {
                map_add_bomb_node(map, bomb_node_new(i, j, 1));
                map_set_cell_value(map, i, j, CELL_EMPTY);
            }

            if ((map_get_cell_value(map, i, j) & 0xf0) == CELL_MONSTER) {
                map_add_monster_node(map, monster_node_new(i, j));
                map_set_cell_value(map, i, j, CELL_EMPTY);
            }
        }
    }

    return map;
}

void map_free(struct map *map) {
    assert(map);

    struct monster_node *current_monster = map->monster_head;

    while (current_monster != NULL) {
        struct monster_node *next = monster_node_get_next(current_monster);
        map_remove_monster_node(map, current_monster);
        current_monster = next;
    }

    struct bomb_node *current_bomb = map->bomb_head;

    while (current_bomb != NULL) {
        struct bomb_node *next = bomb_node_get_next(current_bomb);
        map_remove_bomb_node(map, current_bomb);
        current_bomb = next;
    }

    free(map->grid);
    free(map);
}

void map_write(struct map *map, FILE *file) {
    assert(map);
    assert(file);

    fwrite(map, sizeof(struct map), 1, file);
    fwrite(map->grid, map->width * map->height, 1, file);

    for (struct bomb_node *current = map->bomb_head; current != NULL; current = bomb_node_get_next(current)) {
        bomb_node_write(current, file);
    }

    for (struct monster_node *current = map->monster_head; current != NULL; current = monster_node_get_next(current)) {
        monster_node_write(current, file);
    }
}

struct map *map_read(FILE *file) {
    assert(file);

    struct map *map = malloc(sizeof(struct map));

    if (!map) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fread(map, sizeof(struct map), 1, file);

    map->grid = malloc(sizeof(unsigned char) * map->width * map->height);

    if (!map->grid) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fread(map->grid, map->width * map->height, 1, file);

    if (map->bomb_head != NULL) {

        map->bomb_head = bomb_node_read(file);

        for (struct bomb_node *current = map->bomb_head; bomb_node_get_next(current) != NULL; current = bomb_node_get_next(current)) {
            bomb_node_set_next(current, bomb_node_read(file));
        }
    }

    if (map->monster_head != NULL) {

        map->monster_head = monster_node_read(file);

        for (struct monster_node *current = map->monster_head; monster_node_get_next(current) != NULL; current = monster_node_get_next(current)) {
            monster_node_set_next(current, monster_node_read(file));
        }
    }

    return map;
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

void map_add_bomb_node(struct map *map, struct bomb_node *to_add) {
    assert(map);
    assert(to_add);

    bomb_node_set_next(to_add, map->bomb_head);
    map->bomb_head = to_add;
}

void map_remove_bomb_node(struct map *map, struct bomb_node *to_remove) {
    assert(map);
    assert(to_remove);

    if (map->bomb_head == to_remove) {
        map->bomb_head = bomb_node_get_next(to_remove);
        bomb_node_free(to_remove);

        return;
    }

    for (struct bomb_node *current = map->bomb_head; current != NULL; current = bomb_node_get_next(current)) {

        if (bomb_node_get_next(current) == to_remove) {
            bomb_node_set_next(current, bomb_node_get_next(to_remove));
            bomb_node_free(to_remove);

            return;
        }
    }
}

void map_add_monster_node(struct map *map, struct monster_node *to_add) {
    assert(map);
    assert(to_add);

    monster_node_set_next(to_add, map->monster_head);
    map->monster_head = to_add;
}

void map_remove_monster_node(struct map *map, struct monster_node *to_remove) {
    assert(map);
    assert(to_remove);

    if (map->monster_head == to_remove) {
        map->monster_head = monster_node_get_next(to_remove);
        monster_node_free(to_remove);
        return;
    }

    for (struct monster_node *current = map->monster_head; current != NULL; current = monster_node_get_next(current)) {

        if (monster_node_get_next(current) == to_remove) {
            monster_node_set_next(current, monster_node_get_next(to_remove));
            monster_node_free(to_remove);

            return;
        }
    }
}

struct bomb_node *map_get_bomb_head(struct map *map) {
    assert(map);
    return map->bomb_head;
}

struct monster_node *map_get_monster_head(struct map *map) {
    assert(map);
    return map->monster_head;
}

enum strategy map_get_monsters_strategy(struct map *map) {
    assert(map);
    return map->monsters_strategy;
}

int map_is_inside(struct map *map, int x, int y) {
    assert(map);

    if (x >= 0 && x < map->width && y >= 0 && y < map->height) {
        return 1;
    }

    return 0;
}

unsigned char map_get_cell_value(struct map *map, int x, int y) {
    assert(map);
    assert(map->grid);
    assert(map_is_inside(map, x, y));

    return map->grid[CELL(x, y)];
}

void map_set_cell_value(struct map *map, int x, int y, unsigned char value) {
    assert(map);
    assert(map->grid);
    assert(map_is_inside(map, x, y));

    map->grid[CELL(x, y)] = value;
}

void map_display(struct map *map, SDL_Surface *window, struct sprites *sprites) {
    assert(map);
    assert(window);
    assert(sprites);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            int x = i * SIZE_BLOC;
            int y = j * SIZE_BLOC;

            unsigned char type = map_get_cell_value(map, i, j);

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

    for (struct monster_node *current = map->monster_head; current != NULL; current = monster_node_get_next(current)) {
        monster_node_display(current, window, sprites);
    }
}

void map_set_bomb(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    if ((map_get_cell_value(map, player_get_x(player), player_get_y(player)) & 0xf0) == CELL_DOOR) {
        return;
    }

    if (player_get_num_bomb(player) > 0) {
        map_add_bomb_node(map, bomb_node_new(player_get_x(player), player_get_y(player), player_get_range_bombs(player)));
        player_dec_num_bomb(player);
    }
}

static void set_bonus(struct map *map, int x, int y) {
    assert(map);
    assert(map_is_inside(map, x, y));

    enum bonus_type bonus_type = map_get_cell_value(map, x, y) & 0x0f;

    if (bonus_type == RANDOM) {
        static int i = 0;

        srand(time(NULL) + i++);

        bonus_type = rand() % NUM_BONUS_TYPES;
    }

    if (bonus_type == BONUS_MONSTER) {
        map_add_monster_node(map, monster_node_new(x, y));
        map_set_cell_value(map, x, y, CELL_EMPTY);

    } else {
        map_set_cell_value(map, x, y, CELL_BONUS | bonus_type);
    }
}

static struct monster_node *is_explosion_reaching_monster(int x_explosion, int y_explosion, struct monster_node *monster_head) {

    for (struct monster_node *current = monster_head; current != NULL; current = monster_node_get_next(current)) {
        if (monster_node_get_x(current) == x_explosion && monster_node_get_y(current) == y_explosion) {
            return current;
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

static void propagate_bomb_explosion(struct map *map, struct player *player, struct bomb_node *current_bomb, enum direction dir) {
    assert(map);
    assert(player);
    assert(current_bomb);

    for (int range = 1; range <= player_get_range_bombs(player); range++) {

        int x = direction_get_x(dir, bomb_node_get_x(current_bomb), range);
        int y = direction_get_y(dir, bomb_node_get_y(current_bomb), range);

        if (!map_is_inside(map, x, y)) {
            bomb_node_set_direction_range(current_bomb, dir, range - 1);
            return;
        }

        unsigned char cell_value = map_get_cell_value(map, x, y);

        if (!can_bomb_propagate(cell_value & 0xf0)) {
            bomb_node_set_direction_range(current_bomb, dir, range - 1);
            return;
        }

        struct monster_node *dead_monster = NULL;

        if ((cell_value & 0xf0) == CELL_BOX) {
            set_bonus(map, x, y);
            bomb_node_set_direction_range(current_bomb, dir, range - 1);

            return;

        }

        if ((cell_value & 0xf0) == CELL_BOMB && cell_value != (CELL_BOMB | EXPLODING)) {

            for (struct bomb_node *current = map->bomb_head; current != NULL; current = bomb_node_get_next(current)) {
                if (current != current_bomb && bomb_node_get_x(current) == x && bomb_node_get_y(current) == y) {
                    bomb_node_set_state(current, TTL1);
                    timer_start(bomb_node_get_timer(current), 60);
                }
            }

            bomb_node_set_direction_range(current_bomb, dir, range - 1);

            return;

        }

        if (is_explosion_reaching_player(x, y, player)) {
            player_dec_num_lives(player);
            map_set_cell_value(map, x, y, CELL_BOMB | EXPLODING);
            bomb_node_set_direction_range(current_bomb, dir, range);

            return;

        }

        if ((dead_monster = is_explosion_reaching_monster(x, y, map->monster_head)) != NULL) {
            map_remove_monster_node(map, dead_monster);
            map_set_cell_value(map, x, y, CELL_BOMB | EXPLODING);
            bomb_node_set_direction_range(current_bomb, dir, range);

            return;

        }

        map_set_cell_value(map, x, y, CELL_BOMB | EXPLODING);
    }

    bomb_node_set_direction_range(current_bomb, dir, player_get_range_bombs(player));
}

static void clean_explosion_cells(struct map *map, struct bomb_node *bomb, enum direction direction) {
    assert(map);
    assert(bomb);

    for (int i = 1; i <= bomb_node_get_direction_range(bomb, direction); i++) {
        map_set_cell_value(map, direction_get_x(direction, bomb_node_get_x(bomb), i), direction_get_y(direction, bomb_node_get_y(bomb), i), CELL_EMPTY);
    }
}

void map_update_bombs(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    struct bomb_node *current = map->bomb_head;

    while (current != NULL) {

        timer_update(bomb_node_get_timer(current));

        if (timer_is_over(bomb_node_get_timer(current)) == 0) {
            current = bomb_node_get_next(current);
            continue;
        }

        bomb_node_dec_state(current);

        switch (bomb_node_get_state(current)) {

            case TTL4:
            case TTL3:
            case TTL2:
            case TTL1:
                map_set_cell_value(map, bomb_node_get_x(current), bomb_node_get_y(current), CELL_BOMB | bomb_node_get_state(current));
                break;

            case EXPLODING:
                if (is_explosion_reaching_player(bomb_node_get_x(current), bomb_node_get_y(current), player)) {
                    player_dec_num_lives(player);
                }

                for (struct bomb_node *current_bomb = map->bomb_head; current_bomb != NULL; current_bomb = bomb_node_get_next(current_bomb)) {
                    if (current_bomb != current && bomb_node_get_x(current_bomb) == bomb_node_get_x(current) && bomb_node_get_y(current_bomb) == bomb_node_get_y(current) && bomb_node_get_state(current_bomb) != EXPLODING) {
                        bomb_node_set_state(current_bomb, TTL1);
                        timer_start(bomb_node_get_timer(current_bomb), 60);
                    }
                }

                map_set_cell_value(map, bomb_node_get_x(current), bomb_node_get_y(current), CELL_BOMB | bomb_node_get_state(current));

                propagate_bomb_explosion(map, player, current, NORTH);
                propagate_bomb_explosion(map, player, current, SOUTH);
                propagate_bomb_explosion(map, player, current, EAST);
                propagate_bomb_explosion(map, player, current, WEST);

                break;

            default:
                map_set_cell_value(map, bomb_node_get_x(current), bomb_node_get_y(current), CELL_EMPTY);

                clean_explosion_cells(map, current, NORTH);
                clean_explosion_cells(map, current, SOUTH);
                clean_explosion_cells(map, current, EAST);
                clean_explosion_cells(map, current, WEST);

                struct bomb_node *next = bomb_node_get_next(current);
                map_remove_bomb_node(map, current);
                current = next;

                continue;
        }

        timer_start(bomb_node_get_timer(current), DURATION_BOMB_PERIOD);
        current = bomb_node_get_next(current);
    }
}

static int will_box_be_blocked_by_monsters(struct monster_node *monster_head, int x_dest, int y_dest) {

    for (struct monster_node *current = monster_head; current != NULL; current = monster_node_get_next(current)) {
        if (monster_node_get_x(current) == x_dest && monster_node_get_y(current) == y_dest) {
            return 1;
        }
    }

    return 0;
}

static int is_box_pushable(struct map *map, int x_dest, int y_dest) {
    assert(map);

    if (!map_is_inside(map, x_dest, y_dest)) {
        return 0;
    }

    if ((map_get_cell_value(map, x_dest, y_dest) & 0xf0) == CELL_EMPTY && !will_box_be_blocked_by_monsters(map->monster_head, x_dest, y_dest)) {
        return 1;
    }

    return 0;
}

static int will_player_meet_a_monster(int player_next_x, int player_next_y, struct monster_node *monster_head) {

    for (struct monster_node *current = monster_head; current != NULL; current = monster_node_get_next(current)) {
        if (monster_node_get_x(current) == player_next_x && monster_node_get_y(current) == player_next_y) {
            return 1;
        }
    }

    return 0;
}

int map_move_player(struct map *map, struct player *player, enum direction direction) {
    assert(player);
    assert(map);

    int next_x = direction_get_x(direction, player_get_x(player), 1);
    int next_y = direction_get_y(direction, player_get_y(player), 1);

    if (!map_is_inside(map, next_x, next_y)) {
        return 0;
    }

    if (will_player_meet_a_monster(next_x, next_y, map->monster_head)) {
        player_dec_num_lives(player);
        return 0;
    }

    unsigned char cell = map_get_cell_value(map, next_x, next_y);

    switch (cell & 0xf0) {
        case CELL_SCENERY:

            if ((cell & 0x0f) == SCENERY_PRINCESS) {
                break;
            }

            return 0;

        case CELL_BOX: {

            int x_box_dest = direction_get_x(direction, player_get_x(player), 2);
            int y_box_dest = direction_get_y(direction, player_get_y(player), 2);

            if (is_box_pushable(map, x_box_dest, y_box_dest)) {
                map_set_cell_value(map, x_box_dest, y_box_dest, cell);
                map_set_cell_value(map, next_x, next_y, CELL_EMPTY);

                break;
            }

            return 0;
        }

        case CELL_BONUS:
            player_get_bonus(player, cell & 0x0f);
            map_set_cell_value(map, next_x, next_y, CELL_EMPTY);

            break;

        case CELL_BOMB:
            if ((cell & 0x0f) == EXPLODING) {
                player_dec_num_lives(player);
            }

            break;

        case CELL_KEY:
            player_inc_num_keys(player);
            map_set_cell_value(map, next_x, next_y, CELL_EMPTY);

            break;

        case CELL_DOOR:
            if ((cell & 0x01) == OPENED) {
                break;
            }

            return 0;

        default:
            break;
    }

    player_move(player, direction);

    return 1;
}

int map_will_monster_meet_other_monsters(struct monster_node *current_monster, struct monster_node *monster_head, enum direction current_monster_direction) {
    assert(current_monster);
    assert(monster_head);

    for (struct monster_node *current = monster_head; current != NULL; current = monster_node_get_next(current)) {
        if (monster_node_get_x(current) == direction_get_x(current_monster_direction, monster_node_get_x(current_monster), 1) && monster_node_get_y(current) == direction_get_y(current_monster_direction, monster_node_get_y(current_monster), 1)) {
            return 1;
        }
    }

    return 0;
}

int map_can_monster_move(struct map *map, struct player *player, struct monster_node *monster, enum direction monster_direction) {
    assert(map);
    assert(player);
    assert(monster);

    int x = direction_get_x(monster_direction, monster_node_get_x(monster), 1);
    int y = direction_get_y(monster_direction, monster_node_get_y(monster), 1);

    if (!map_is_inside(map, x, y)) {
        return 0;
    }

    if (map_will_monster_meet_other_monsters(monster, map->monster_head, monster_direction)) {
        return 0;
    }

    switch (map_get_cell_value(map, x, y) & 0xf0) {

        case CELL_SCENERY:
        case CELL_BOMB:
        case CELL_DOOR:
        case CELL_BOX:
            return 0;

        default:
            return 1;
    }
}

int map_will_monster_meet_player(struct monster_node *monster, struct player *player, enum direction monster_direction) {
    assert(player);
    assert(monster);

    if (direction_get_x(monster_direction, monster_node_get_x(monster), 1) == player_get_x(player) && direction_get_y(monster_direction, monster_node_get_y(monster), 1) == player_get_y(player)) {
        return 1;
    }

    return 0;
}

void map_monster_meeting_player(struct monster_node *monster, struct player *player, enum direction monster_direction) {
    assert(monster);
    assert(player);

    monster_node_set_direction(monster, monster_direction);

    player_dec_num_lives(player);

    timer_start(monster_node_get_timer(monster), DURATION_MONSTER_MOVE);
}

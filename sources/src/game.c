#include "../include/game.h"
#include "../include/random.h"
#include "../include/constant.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct game
 * @brief Structure representing the game.
 */
struct game {
    struct sprites *sprites; /**< Sprites of the game */
    SDL_Surface *window; /**< The window containing the game */
    struct map **list_maps; /**< List of game maps */
    int num_levels; /**< Number of game maps */
    int current_level; /**< Current level */
    struct player *player; /**< Player of the game */
    int is_paused; /**< Is the game paused ? */
};

struct game *game_new(void) {

    struct game *game = malloc(sizeof(*game));

    if (!game) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(game, 0, sizeof(*game));

    game->num_levels = NUM_LEVELS;
    game->current_level = 0;
    game->is_paused = 0;
    game->player = player_new(NUM_BOMBS_MAX);
    game->sprites = sprites_new();
    game->list_maps = malloc(game->num_levels * sizeof(struct map *));

    if (!game->list_maps) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    char *maps_filenames[] = {
            "maps/map_0",
            "maps/map_1",
            "maps/map_2",
            "maps/map_3",
            "maps/map_4",
            "maps/map_5",
            "maps/map_6",
            "maps/map_7"
    };

    for (int i = 0; i < game->num_levels; i++) {
        game->list_maps[i] = map_new(maps_filenames[i]);
    }

    game->window = window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)), SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);

    FILE *backup_file = fopen(FILENAME_BACKUP, "rb");

    if (!backup_file) {
        map_init_list_monsters(game->list_maps[game->current_level]);
        return game;
    }

    fclose(backup_file);

    return game;
}

void game_free(struct game *game) {
    assert(game);
    assert(game->list_maps);
    assert(game->player);

    player_free(game->player);

    for (int i = 0; i < game->num_levels; i++) {
        map_free(game->list_maps[i]);
    }

    free(game->list_maps);
    sprites_free(game->sprites);
    free(game->sprites);
    SDL_FreeSurface(game->window);
    free(game);
}

void game_write(struct game *game, FILE *file) {
    fwrite(game, sizeof(*game), 1, file);

    player_write(game->player, file);

    for (int i = 0; i < game->num_levels; i++) {
        map_write(game->list_maps[i], file);
    }
}

void game_read(struct game *game, FILE *file) {

    struct player *player = game->player;
    struct map **list_maps = game->list_maps;
    struct sprites *sprites = game->sprites;
    SDL_Surface *window = game->window;

    fread(game, sizeof(*game), 1, file);

    game->player = player;
    game->list_maps = list_maps;
    game->sprites = sprites;
    game->window = window;

    player_read(game->player, file);

    for (int i = 0; i < game->num_levels; i++) {
        map_read(game->list_maps[i], file);
    }
}

struct map *game_get_current_map(struct game *game) {
    assert(game);
    assert(game->list_maps);
    assert(game->list_maps[game->current_level]);

    return game->list_maps[game->current_level];
}

struct player *game_get_player(struct game *game) {
    assert(game);
    assert(game->player);

    return game->player;
}

void game_set_current_level(struct game *game, int level) {
    assert(game);
    assert(level >= 0 && level < game->num_levels);

    game->current_level = level;
}

int game_get_current_level(struct game *game) {
    assert(game);
    assert(game->current_level >= 0 && game->current_level <= game->num_levels - 1);

    return game->current_level;
}

static void display_banner(struct game *game) {
    assert(game);

    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);

    int y = (map_get_height(map)) * SIZE_BLOC;

    for (int i = 0; i < map_get_width(map); i++) {
        window_display_image(game->window, sprites_get_banner_line(game->sprites), i * SIZE_BLOC, y);
    }

    int white_bloc = 0.5 * SIZE_BLOC;
    int x = 0;

    y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_number(game->sprites, game_get_current_level(game) + 1), x, y);

    x = SIZE_BLOC;
    y = (map_get_height(map)) * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_banner_vertical_line(game->sprites), x, y);

    x = white_bloc + SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_banner_life(game->sprites), x, y);

    x = white_bloc + 2 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_number(game->sprites, player_get_num_lives(player)), x, y);

    x = 2 * white_bloc + 3 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_banner_bomb(game->sprites), x, y);

    x = 2 * white_bloc + 4 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_number(game->sprites, player_get_num_bomb(game_get_player(game))), x, y);

    x = 3 * white_bloc + 5 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_banner_range(game->sprites), x, y);

    x = 3 * white_bloc + 6 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_number(game->sprites, player_get_range_bombs(player)), x, y);

    x = 4 * white_bloc + 7 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_key(game->sprites), x, y);

    x = 4 * white_bloc + 8 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(game->window, sprites_get_number(game->sprites, player_get_num_keys(player)), x, y);
}

void game_display(struct game *game) {
    assert(game);

    window_clear(game->window);

    map_display(game_get_current_map(game), game->window, game->sprites);
    display_banner(game);
    player_display(game_get_player(game), game->window, game->sprites);

    window_refresh(game->window);
}

static void change_current_level(struct game *game, int level) {
    assert(game);

    game_set_current_level(game, level);

    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);

    player_set_num_bombs(player, NUM_BOMBS_MAX);
    map_init_list_monsters(map);
    game->window = window_create(SIZE_BLOC * map_get_width(map), SIZE_BLOC * map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);
}

static void save_game(struct game *game) {

    FILE *file = fopen(FILENAME_BACKUP, "wb");

    if (file) {
        game_write(game, file);

        fclose(file);

        printf("#########################################\n");
        printf("  Current game saved in backup/data.bin\n");
        printf("#########################################\n");
    }
}

static int input_keyboard(struct game *game) {
    assert(game);

    SDL_Event event = {0};
    int has_moved = 0;
    struct player *player = game_get_player(game);
    struct map *map = game_get_current_map(game);

    while (SDL_PollEvent(&event)) {

        if (game->is_paused) {

            switch (event.type) {

                case SDL_QUIT:
                    return 1;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {

                        case SDLK_s:
                            if (event.key.keysym.mod & KMOD_CTRL) {
                                save_game(game);
                                return 1;
                            }

                            break;

                        case SDLK_p:
                            game->is_paused = !game->is_paused;
                            break;

                        default:
                            break;
                    }
                }

                default:
                    break;
            }
        } else {

            switch (event.type) {

                case SDL_QUIT:
                    return 1;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {

                        case SDLK_s:
                            if (event.key.keysym.mod & KMOD_CTRL) {
                                save_game(game);
                                return 1;
                            }

                            break;

                        case SDLK_UP:

                            if (map_move_player(map, player, NORTH)) {
                                has_moved = 1;
                            }

                            break;

                        case SDLK_DOWN:

                            if (map_move_player(map, player, SOUTH)) {
                                has_moved = 1;
                            }

                            break;

                        case SDLK_RIGHT:

                            if (map_move_player(map, player, EAST)) {
                                has_moved = 1;
                            }

                            break;

                        case SDLK_LEFT:

                            if (map_move_player(map, player, WEST)) {
                                has_moved = 1;
                            }

                            break;

                        case SDLK_SPACE:
                            map_set_bomb(map, player);
                            break;

                        case SDLK_p:
                            game->is_paused = !game->is_paused;
                            break;

                        case SDLK_RETURN: {

                            int x_next_player = direction_get_x(player_get_direction(player), player_get_x(player), 1);
                            int y_next_player = direction_get_y(player_get_direction(player), player_get_y(player), 1);

                            if (map_is_inside(map, x_next_player, y_next_player)) {
                                uint8_t type = map_get_cell_value(map, x_next_player, y_next_player);

                                if (((type & 0xf1) == (CELL_DOOR | CLOSED)) && player_get_num_keys(player)) {
                                    map_set_cell_value(map, x_next_player, y_next_player, type & 0xfe);
                                    player_dec_num_keys(player);
                                }
                            }

                            break;
                        }

                        default:
                            break;
                    }

                    if (has_moved) {

                        uint8_t cell = map_get_cell_value(map, player_get_x(player), player_get_y(player));

                        if ((cell & 0xf0) == CELL_DOOR) {
                            int level = (cell & 0x0e) / 2;
                            change_current_level(game, level);

                        } else if (cell == (CELL_SCENERY | SCENERY_PRINCESS)) {

                            printf("==========================================\n");
                            printf(" >>>>>>>>>>>>>  YOU WON!!!  <<<<<<<<<<<<<\n");
                            printf("==========================================\n");

                            return 1;
                        }
                    }

                    break;
                }

                default:
                    break;
            }
        }
    }

    return 0;
}

int game_update(struct game *game) {
    assert(game);

    struct map *map = game_get_current_map(game);

    assert(map);

    struct player *player = game_get_player(game);

    assert(player);

    if (input_keyboard(game)) {
        return 1;
    }

    if (!game->is_paused) {
        if (map_get_monster_strategy(map) == RANDOM_STRATEGY) {
            random_update_monsters(map, player);
        }

        map_update_bombs(map, player);
    }

    if (player_get_num_lives(player) == 0) {
        printf("===========================================\n");
        printf(" >>>>>>>>>>>>>  YOU LOST!!!  <<<<<<<<<<<<<\n");
        printf("===========================================\n");

        return 1;
    }

    return 0;
}
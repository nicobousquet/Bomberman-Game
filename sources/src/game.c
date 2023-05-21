#include "../include/game.h"
#include "../include/sprite.h"
#include "../include/bomb.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/timer.h"
#include "../include/monster.h"
#include "../include/window.h"
#include "../include/constant.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct game {
    struct map **maps_list; /* the game's maps */
    short num_levels; /* game's number of maps */
    short current_level; /* current level */
    struct player *player; /* player of the game */
};

/* launching backup or new game */
struct game *game_new(void) {
    /* load sprites into process memory */
    sprite_load();
    struct game *game = malloc(sizeof(struct game));
    if (!game) {
        perror("malloc");
    }
    memset(game, 0, sizeof(struct game));
    const char *filename = "backup/data.bin";
    FILE *file = fopen(filename, "rb");
    if (file != NULL) {
        /* loading game structure */
        fread(game, sizeof(struct game), 1, file);
        /* loading player */
        game->player = malloc(player_get_size());
        if (!game->player) {
            perror("malloc");
        }

        fread(game->player, player_get_size(), 1, file);

        struct timer *timer_invincibility = malloc(timer_get_size());
        if (!timer_invincibility) {
            perror("malloc");
        }

        fread(timer_invincibility, timer_get_size(), 1, file);
        player_set_timer_invincibility(game->player, timer_invincibility);

        /* loading maps */
        game->maps_list = malloc(game->num_levels * sizeof(struct map *));
        if (!game->maps_list) {
            perror("malloc");
        }

        for (int i = 0; i < game->num_levels; i++) {
            game->maps_list[i] = malloc(map_get_size());
            if (!game->maps_list[i]) {
                perror("malloc");
            }

            fread(game->maps_list[i], map_get_size(), 1, file);
            /* loading maps' grid */
            unsigned char *grid = malloc(map_get_width(game->maps_list[i]) * map_get_height(game->maps_list[i]));
            if (!grid) {
                perror("malloc");
            }

            fread(grid, map_get_width(game->maps_list[i]) * map_get_height(game->maps_list[i]), 1, file);
            map_set_grid(game->maps_list[i], grid);
            /* loading maps' bomb */
            struct bomb **bombs_list = map_get_bombs_list(game->maps_list[i]);
            for (int j = 0; j < NUM_BOMBS_MAX; j++) {
                if (bombs_list[j] != NULL) {
                    bombs_list[j] = malloc(bomb_get_size());
                    fread(bombs_list[j], bomb_get_size(), 1, file);
                    struct timer *timer = malloc(timer_get_size());

                    if (!timer) {
                        perror("malloc");
                    }
                    fread(timer, timer_get_size(), 1, file);
                    timer_set_start_time(timer, SDL_GetTicks() - (timer_get_duration(timer) - timer_get_remaining(timer)));
                    bomb_set_timer(bombs_list[j], timer);
                }
            }
            struct monster **monsters_list = map_get_monsters_list(game->maps_list[i]);
            for (int j = 0; j < NUM_MONSTERS_MAX; j++) {
                if (monsters_list[j] != NULL) {
                    monsters_list[j] = malloc(monster_get_size());
                    fread(monsters_list[j], monster_get_size(), 1, file);
                    struct timer *timer = malloc(timer_get_size());

                    if (!timer) {
                        perror("malloc");
                    }
                    fread(timer, timer_get_size(), 1, file);
                    timer_set_start_time(timer, SDL_GetTicks() - (timer_get_duration(timer) - timer_get_remaining(timer)));
                    monster_set_timer(monsters_list[j], timer);
                }
            }
        }
        /* once backup loaded, delete backup file */
        fclose(file);
        remove(filename);
    } else { /* loading new game */
        game->num_levels = NUM_LEVELS;
        game->current_level = 0;
        game->player = player_init(NUM_BOMBS_MAX);
        game->maps_list = malloc(game->num_levels * sizeof(struct map *));
        if (!game->maps_list) {
            perror("malloc");
        }

        char *maps_name[] = {
                "map/map_0",
                "map/map_1",
                "map/map_2",
                "map/map_3",
                "map/map_4",
                "map/map_5",
                "map/map_6",
                "map/map_7"
        };
        /* loading maps */
        for (int i = 0; i < game->num_levels; i++) {
            game->maps_list[i] = map_read_new_map(maps_name[i]);
            /* initializing maps' bomb list  */
            struct bomb **bombs_list = map_get_bombs_list(game->maps_list[i]);
            for (int j = 0; j < NUM_BOMBS_MAX; j++) {
                bombs_list[j] = NULL;
            }

            struct monster **monsters_list = map_get_monsters_list(game->maps_list[i]);
            for (int j = 0; j < NUM_MONSTERS_MAX; j++) {
                monsters_list[j] = NULL;
            }
        }
    }
    /* setting monsters on current map */
    map_init_monsters_list(game->maps_list[game->current_level]);
    return game;
}

/* freeing game */
void game_free(struct game *game) {
    assert(game);
    assert(game->maps_list);
    for (int i = 0; i < game->num_levels; i++) {
        assert(game->maps_list[i]);
    }
    assert(game->player);

    player_free(game->player);
    for (int i = 0; i < game->num_levels; i++) {
        map_free(game->maps_list[i]);
    }

    free(game->maps_list);
    free(game);

    sprite_free();
}

struct map *game_get_current_map(struct game *game) {
    assert(game);
    assert(game->maps_list);
    assert(game->maps_list[game->current_level]);
    return game->maps_list[game->current_level];
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

/* displaying banner at the bottom of the window */
static void display_banner(struct game *game) {
    assert(game);

    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);

    int y = (map_get_height(map)) * SIZE_BLOC;
    for (int i = 0; i < map_get_width(map); i++) {
        window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);
    }

    int white_bloc = 0.5 * SIZE_BLOC;
    int x = 0;
    /* displaying number of lives */
    y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
    window_display_image(sprite_get_number(game_get_current_level(game) + 1), x, y);

    x = SIZE_BLOC;
    y = (map_get_height(map)) * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_banner_vertical_line(), x, y);

    /* displaying number of bombs */
    x = white_bloc + SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_banner_life(), x, y);

    x = white_bloc + 2 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_number(player_get_num_lives(player)), x, y);

    /* displaying bomb range */
    x = 2 * white_bloc + 3 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_banner_bomb(), x, y);

    x = 2 * white_bloc + 4 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_number(player_get_num_bomb(game_get_player(game))), x, y);

    /* displaying number of keys */
    x = 3 * white_bloc + 5 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_banner_range(), x, y);

    x = 3 * white_bloc + 6 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_number(player_get_bombs_range(player)), x, y);

    x = 4 * white_bloc + 7 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_key(), x, y);

    x = 4 * white_bloc + 8 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_number(player_get_num_keys(player)), x, y);
}

void game_display(struct game *game) {
    assert(game);
    window_clear();
    map_display(game_get_current_map(game));
    display_banner(game);
    player_display(game_get_player(game));
    map_display_monsters((game_get_current_map(game)));

    window_refresh();
}

static void backup_game(struct game *game) {
    assert(game);
    struct player *player = game_get_player(game);
    FILE *file = fopen("backup/data.bin", "wb");
    if (!file) {
        perror("fopen");
        return;
    }

    fwrite(game, sizeof(struct game), 1, file);
    fwrite(player, player_get_size(), 1, file);
    fwrite(player_get_timer_invincibility(player), timer_get_size(), 1, file);

    for (int i = 0; i < game->num_levels; i++) {
        fwrite(game->maps_list[i], map_get_size(), 1, file);
        fwrite(map_get_grid(game->maps_list[i]), map_get_width(game->maps_list[i]) * map_get_height(game->maps_list[i]), 1, file);

        struct bomb **bombs_list = map_get_bombs_list(game->maps_list[i]);
        for (int j = 0; j < NUM_BOMBS_MAX; j++) {
            if (bombs_list[j] != NULL) {
                fwrite(bombs_list[j], bomb_get_size(), 1, file);
                fwrite(bomb_get_timer(bombs_list[j]), timer_get_size(), 1, file);
            }
        }

        struct monster **monsters_list = map_get_monsters_list(game->maps_list[i]);
        for (int j = 0; j < NUM_MONSTERS_MAX; j++) {
            if (monsters_list[j] != NULL) {
                fwrite(monsters_list[j], monster_get_size(), 1, file);
                fwrite(monster_get_timer(monsters_list[j]), timer_get_size(), 1, file);
            }
        }
    }

    fclose(file);

    printf("#########################################\n");
    printf("  Current game saved in backup/data.bin\n");
    printf("#########################################\n");
}

static void pause_game(SDL_Event *event) {
    assert(event);
    SDL_WaitEvent(event);
    do {
        /* game paused until next time we type P */
        SDL_WaitEvent(event);
    } while (event->key.keysym.sym != SDLK_p);
}

static void change_current_level(struct game *game, int level) {
    assert(game);
    /* changing level */
    game_set_current_level(game, level);
    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);
    player_set_num_bombs(player, 9);
    /* loading monsters in monsters_list */
    map_init_monsters_list(map);
    window_create(SIZE_BLOC * map_get_width(map), SIZE_BLOC * map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);
}

static short input_keyboard(struct game *game) {
    assert(game);
    SDL_Event event = {0};
    int move = 0;
    struct player *player = game_get_player(game);
    struct map *map = game_get_current_map(game);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /* quit game with no backup */
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    /* Ctrl + S ==> quit game with backup */
                    case SDLK_s:
                        if (event.key.keysym.mod & KMOD_CTRL) {
                            backup_game(game);
                            return 1;
                        }
                        break;
                        /* going north */
                    case SDLK_UP:
                        player_set_current_way(player, NORTH);
                        if (map_move_player(map, player)) {
                            move = 1;
                        }
                        break;
                        /* going south */
                    case SDLK_DOWN:
                        player_set_current_way(player, SOUTH);
                        if (map_move_player(map, player)) {
                            move = 1;
                        }
                        break;
                        /* going east */
                    case SDLK_RIGHT:
                        player_set_current_way(player, EAST);
                        if (map_move_player(map, player)) {
                            move = 1;
                        }
                        break;
                        /* going west */
                    case SDLK_LEFT:
                        player_set_current_way(player, WEST);
                        if (map_move_player(map, player)) {
                            move = 1;
                        }
                        break;
                        /* set a bomb */
                    case SDLK_SPACE:
                        map_set_bomb(map, player);
                        break;
                        /* pause game */
                    case SDLK_p:
                        pause_game(&event);
                        break;
                    case SDLK_RETURN: {
                        /* open door */
                        int x_next_player = direction_get_x(player_get_x(player), player_get_direction(player), 1);
                        int y_next_player = direction_get_y(player_get_y(player), player_get_direction(player), 1);

                        if (map_is_inside(map, x_next_player, y_next_player)) {
                            uint8_t type = map_get_cell_value(map, x_next_player, y_next_player);
                            if ((type & 0xf1) == (CELL_DOOR | CLOSED)) {
                                map_set_cell_value(map, x_next_player, y_next_player, type & 0xfe);
                                player_dec_num_keys(player);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }

                if (move) {
                    uint8_t cell = map_get_cell_value(map, player_get_x(player), player_get_y(player));
                    if ((cell & 0xf0) == CELL_DOOR) {
                        /* level of next level */
                        int level = (cell & 0x0e) / 2;
                        change_current_level(game, level);
                    }
                }
                break;
            }
            default:
                break;
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
        /* exit game */
        return 1;
    } /* if player is dead */
    else if (player_get_num_lives(player) == 0) {
        printf("===========================================\n");
        printf(" >>>>>>>>>>>>>  YOU LOST!!!  <<<<<<<<<<<<<\n");
        printf("===========================================\n");
        return 1;
    } /* if player found the princess */
    else if (map_get_cell_value(map, player_get_x(player), player_get_y(player)) == (CELL_SCENERY | SCENERY_PRINCESS)) {
        printf("==========================================\n");
        printf(" >>>>>>>>>>>>>  YOU WON!!!  <<<<<<<<<<<<<\n");
        printf("==========================================\n");
        return 1;
    }
    /* updating bombs */
    map_update_bombs(map, player);
    /* updating monsters */
    map_update_monsters(map, player);

    return 0;
}

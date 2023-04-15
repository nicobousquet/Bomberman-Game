/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/game.h"
#include "../include/sprite.h"
#include "../include/player.h"
#include "../include/bomb.h"
#include "../include/timer.h"
#include "../include/monster.h"
#include "../include/window.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct game {
    struct map **maps; /* the game's maps */
    short levels; /* game's number of maps */
    short current_level; /* current level */
    struct player *player; /* player of the game */
};

struct map *game_get_current_map(struct game *game) {
    assert(game);
    assert(game->maps);
    assert(game->maps[game->current_level]);
    return game->maps[game->current_level];
}


struct player *game_get_player(struct game *game) {
    assert(game);
    assert(game->player);
    return game->player;
}

void game_set_current_level(struct game *game, int level) {
    assert(game);
    assert(level >= 0 && level < game->levels);
    game->current_level = level;
}

int game_get_current_level(struct game *game) {
    assert(game);
    assert(game->current_level >= 0 && game->current_level <= game->levels - 1);
    return game->current_level;
}

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
        game->maps = malloc(game->levels * sizeof(struct map *));
        if (!game->maps) {
            perror("malloc");
        }

        for (int i = 0; i < game->levels; i++) {
            game->maps[i] = malloc(map_get_size());
            if (!game->maps[i]) {
                perror("malloc");
            }

            fread(game->maps[i], map_get_size(), 1, file);
            /* loading maps' grid */
            unsigned char *grid = malloc(map_get_width(game->maps[i]) * map_get_height(game->maps[i]));
            if (!grid) {
                perror("malloc");
            }

            fread(grid, map_get_width(game->maps[i]) * map_get_height(game->maps[i]), 1, file);
            map_set_grid(game->maps[i], grid);
            /* loading maps' bomb */
            struct bomb **map_bomb_array = map_get_bomb_array(game->maps[i]);
            for (int j = 0; j < NUM_MAX_BOMBS; j++) {
                if (map_bomb_array[j] != NULL) {
                    map_bomb_array[j] = malloc(bomb_get_size());
                    fread(map_bomb_array[j], bomb_get_size(), 1, file);
                    struct timer *timer = malloc(timer_get_size());

                    if (!timer) {
                        perror("malloc");
                    }
                    fread(timer, timer_get_size(), 1, file);
                    timer_set_start_time(timer, SDL_GetTicks() - (timer_get_duration(timer) - timer_get_remaining(timer)));
                    bomb_set_timer(map_bomb_array[j], timer);
                }
            }
            struct monster **monster_array = map_get_monster_array(game->maps[i]);
            for (int j = 0; j < NUM_MONSTER_MAX; j++) {
                if (monster_array[j] != NULL) {
                    monster_array[j] = malloc(monster_get_size());
                    fread(monster_array[j], monster_get_size(), 1, file);
                    struct timer *timer = malloc(timer_get_size());

                    if (!timer) {
                        perror("malloc");
                    }
                    fread(timer, timer_get_size(), 1, file);
                    timer_set_start_time(timer, SDL_GetTicks() - (timer_get_duration(timer) - timer_get_remaining(timer)));
                    monster_set_timer(monster_array[j], timer);
                }
            }
        }
        /* once backup loaded, delete backup file */
        fclose(file);
        remove(filename);
    } else { /* loading new game */
        game->levels = 8;
        game->current_level = 0;
        game->player = player_init(9);
        game->maps = malloc(game->levels * sizeof(struct map *));
        if (!game->maps) {
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
        for (int i = 0; i < game->levels; i++) {
            game->maps[i] = map_get_map(maps_name[i]);
            /* initializing maps' bomb array  */
            struct bomb **map_bomb_array = map_get_bomb_array(game->maps[i]);
            for (int j = 0; j < NUM_MAX_BOMBS; j++) {
                map_bomb_array[j] = NULL;
            }

            struct monster **monster_array = map_get_monster_array(game->maps[i]);
            for (int j = 0; j < NUM_MONSTER_MAX; j++) {
                monster_array[j] = NULL;
            }
        }
    }
    /* setting monsters on current map */
    map_set_monsters(game->maps[game->current_level]);
    return game;
}

/* freeing game */
void game_free(struct game *game) {
    assert(game);
    assert(game->maps);
    for (int i = 0; i < game->levels; i++) {
        assert(game->maps[i]);
    }
    assert(game->player);

    player_free(game->player);
    for (int i = 0; i < game->levels; i++) {
        map_free(game->maps[i]);
    }

    free(game->maps);
    free(game);

    sprite_free();
}

/* displaying banner at the bottom of the window */
void game_banner_display(struct game *game) {
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
    window_display_image(sprite_get_banner_line_vert(), x, y);

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
    window_display_image(sprite_get_number(player_get_range(player)), x, y);

    x = 4 * white_bloc + 7 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_key(), x, y);

    x = 4 * white_bloc + 8 * SIZE_BLOC + LINE_HEIGHT;
    window_display_image(sprite_get_number(player_get_num_keys(player)), x, y);
}

void game_display(struct game *game) {
    assert(game);
    window_clear();
    map_display(game_get_current_map(game));
    game_banner_display(game);
    player_display(game_get_player(game));
    monsters_display(game_get_current_map(game));

    window_refresh();
}

void game_backup(struct game *game) {
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

    for (int i = 0; i < game->levels; i++) {
        fwrite(game->maps[i], map_get_size(), 1, file);
        fwrite(map_get_grid(game->maps[i]), map_get_width(game->maps[i]) * map_get_height(game->maps[i]), 1, file);

        struct bomb **map_bomb_array = map_get_bomb_array(game->maps[i]);
        for (int j = 0; j < NUM_MAX_BOMBS; j++) {
            if (map_bomb_array[j] != NULL) {
                fwrite(map_bomb_array[j], bomb_get_size(), 1, file);
                fwrite(bomb_get_timer(map_bomb_array[j]), timer_get_size(), 1, file);
            }
        }

        struct monster **monster_array = map_get_monster_array(game->maps[i]);
        for (int j = 0; j < NUM_MONSTER_MAX; j++) {
            if (monster_array[j] != NULL) {
                fwrite(monster_array[j], monster_get_size(), 1, file);
                fwrite(monster_get_timer(monster_array[j]), timer_get_size(), 1, file);
            }
        }
    }

    fclose(file);

    printf("#########################################\n");
    printf("  Current game saved in backup/data.bin\n");
    printf("#########################################\n");
}

void game_set_bomb(struct game *game) {
    assert(game);

    struct player *player = game_get_player(game);
    /* cannot destroy a door */
    if ((map_get_cell_value(game_get_current_map(game), player_get_x(player), player_get_y(player)) & 0xf0) == CELL_DOOR) {
        return;
    }

    if (player_get_num_bomb(player) > 0) {
        struct bomb *bomb = malloc(bomb_get_size());
        if (!bomb) {
            perror("malloc");
        }
        memset(bomb, 0, bomb_get_size());
        /* initializing bomb properties */
        bomb_init(bomb, player_get_x(player), player_get_y(player), player_get_range(player), DURATION_BOMB_TTL);
        timer_start(bomb_get_timer(bomb));
        /* adding bomb in BOMBS_ARRAY */
        struct bomb **map_bomb_array = map_get_bomb_array(game->maps[game->current_level]);
        for (int i = 0; i < NUM_MAX_BOMBS; i++)
            if (map_bomb_array[i] == NULL) {
                map_bomb_array[i] = bomb;
                break;
            }
        /* decrease player number of bombs */
        player_dec_num_bomb(player);
    }
}

void game_pause(SDL_Event *event) {
    assert(event);
    SDL_WaitEvent(event);
    do {
        /* game paused until next time we type P */
        SDL_WaitEvent(event);
    } while (event->key.keysym.sym != SDLK_p);
}

void game_change_current_level(struct game *game, int level) {
    assert(game);
    /* changing level */
    game_set_current_level(game, level);
    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);
    player_set_num_bombs(player, 9);
    /* loading monsters in monster_array */
    map_set_monsters(map);
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
                            game_backup(game);
                            return 1;
                        }
                        break;
                        /* going north */
                    case SDLK_UP:
                        player_set_current_way(player, NORTH);
                        if (player_move(player, map)) {
                            move = 1;
                        }
                        break;
                        /* going south */
                    case SDLK_DOWN:
                        player_set_current_way(player, SOUTH);
                        if (player_move(player, map)) {
                            move = 1;
                        }
                        break;
                        /* going east */
                    case SDLK_RIGHT:
                        player_set_current_way(player, EAST);
                        if (player_move(player, map)) {
                            move = 1;
                        }
                        break;
                        /* going west */
                    case SDLK_LEFT:
                        player_set_current_way(player, WEST);
                        if (player_move(player, map)) {
                            move = 1;
                        }
                        break;
                        /* set a bomb */
                    case SDLK_SPACE:
                        game_set_bomb(game);
                        break;
                        /* pause game */
                    case SDLK_p:
                        game_pause(&event);
                        break;
                    default:
                        break;
                }

                if (move) {
                    enum cell_type cell = map_get_cell_value(map, player_get_x(player), player_get_y(player));
                    if ((cell & 0xf0) == CELL_DOOR) {
                        /* level of next level */
                        int level = (cell & 0x0e) / 2;
                        game_change_current_level(game, level);
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
    struct player *player = game_get_player(game);
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
    bomb_update(map, game->player);
    /* updating monsters */
    monster_update(map, game->player);

    return 0;
}

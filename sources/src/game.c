/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include "../include/game.h"
#include "../include/sprite.h"
#include "../include/player.h"
#include "../include/bomb.h"
#include "../include/monster.h"
#include "../include/window.h"
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct game {
    struct map **maps; /* the game's maps */
    short levels; /* game's number of maps */
    short level; /* current level */
    struct player *player; /* player of the game */
};

void game_set_level(struct game *game, int level) {
    assert(game);
    game->level = level;
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
    /* trying to open file backup/data.bin */
    int fd = open("backup/data.bin", O_RDWR);
    /* if file exists, loading the backup */
    if (fd != -1) {
        /* loading game structure */
        read(fd, game, sizeof(struct game));
        /* loading player */
        game->player = malloc(player_get_size());
        if (!game->player)
            perror("malloc");

        read(fd, game->player, player_get_size());
        /* loading maps */
        game->maps = malloc(game->levels * map_get_size_ptr());
        if (!game->maps)
            perror("malloc");

        for (int i = 0; i < game->levels; i++) {
            game->maps[i] = malloc(map_get_size());
            if (!game->maps[i])
                perror("malloc");

            read(fd, game->maps[i], map_get_size());
            /* loading maps' grid */
            unsigned char *grid = malloc(STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT);
            if (!grid)
                perror("malloc");

            read(fd, grid, STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT);
            map_set_grid(game->maps[i], grid);
            /* loading maps' bomb */
            struct bomb **map_bomb_array = map_get_bomb_array(game->maps[i]);
            for (int j = 0; j < NUM_MAX_BOMBS; j++) {
                if (map_bomb_array[j] != NULL) {
                    map_bomb_array[j] = malloc(bomb_get_size());
                    read(fd, map_bomb_array[j], bomb_get_size());
                    bomb_set_t0(map_bomb_array[j], 0);
                }
            }
            struct monster **monster_array = map_get_monster_array(game->maps[i]);
            for (int j = 0; j < NUM_MONSTER_MAX; j++) {
                if (monster_array[j] != NULL) {
                    monster_array[j] = malloc(monster_get_size());
                    read(fd, monster_array[j], monster_get_size());
                    monster_set_t0(monster_array[j], 0);
                }
            }
        }
        /* once backup loaded, delete backup file */
        remove("backup/data.bin");
    } else { /* loading new game */
        game->levels = 8;
        game->level = 0;
        game->player = player_init(9);
        game->maps = malloc(game->levels * map_get_size_ptr());
        if (!game->maps)
            perror("malloc");

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
            for (int j = 0; j < NUM_MAX_BOMBS; j++)
                map_bomb_array[j] = NULL;

            struct monster **monster_array = map_get_monster_array(game->maps[i]);
            for (int j = 0; j < NUM_MONSTER_MAX; j++)
                monster_array[j] = NULL;

        }
    }
    /* setting monsters on current map */
    map_set_monsters(game->maps[game->level]);
    return game;
}

/* freeing game */
void game_free(struct game *game) {
    assert(game);
    player_free(game->player);
    for (int i = 0; i < game->levels; i++)
        map_free(game->maps[i]);

    free(game->maps);
    free(game);
}

struct map *game_get_current_map(struct game *game) {
    assert(game);
    return game->maps[game->level];
}


struct player *game_get_player(struct game *game) {
    assert(game);
    return game->player;
}

/* displaying banner at the bottom of the window */
void game_banner_display(struct game *game) {
    assert(game);

    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);

    int y = (map_get_height(map)) * SIZE_BLOC;
    for (int i = 0; i < map_get_width(map); i++)
        window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

    int white_bloc = SIZE_BLOC;
    int x = 0;
    /* displaying number of life */
    y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
    window_display_image(sprite_get_banner_life(), x, y);

    x = SIZE_BLOC;
    window_display_image(sprite_get_number(player_get_nb_life(player)), x, y);

    /* displaying number of bombs */
    x = white_bloc + 2 * SIZE_BLOC;
    window_display_image(sprite_get_banner_bomb(), x, y);

    x = white_bloc + 3 * SIZE_BLOC;
    window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

    /* displaying bomb range */
    x = 2 * white_bloc + 4 * SIZE_BLOC;
    window_display_image(sprite_get_banner_range(), x, y);

    x = 2 * white_bloc + 5 * SIZE_BLOC;
    window_display_image(sprite_get_number(player_get_range(player)), x, y);

    /* displaying number of keys */
    x = 3 * white_bloc + 6 * SIZE_BLOC;
    window_display_image(sprite_get_key(), x, y);

    x = 3 * white_bloc + 7 * SIZE_BLOC;
    window_display_image(sprite_get_number(player_get_nb_keys(player)), x, y);
}

void game_display(struct game *game) {
    assert(game);
    window_clear();
    map_display(game_get_current_map(game));
    game_banner_display(game);
    player_display(game->player);
    monsters_display(game_get_current_map(game));

    window_refresh();
}

/* saving game's data in file backup/data.bin */
void game_backup(struct game *game) {
    struct player *player = game_get_player(game);
    int fd = open("backup/data.bin", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, game, sizeof(struct game));
    write(fd, player, player_get_size());
    for (int i = 0; i < game->levels; i++) {
        write(fd, game->maps[i], map_get_size());
        write(fd, map_get_grid(game->maps[i]), STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT);
        struct bomb **map_bomb_array = map_get_bomb_array(game->maps[i]);
        for (int j = 0; j < NUM_MAX_BOMBS; j++) {
            if (map_bomb_array[j] != NULL)
                write(fd, map_bomb_array[j], bomb_get_size());
        }
        struct monster **monster_array = map_get_monster_array(game->maps[i]);
        for (int j = 0; j < NUM_MAX_BOMBS; j++) {
            if (monster_array[j] != NULL)
                write(fd, monster_array[j], monster_get_size());
        }
    }
    printf("#########################################\n");
    printf("  Current game saved in backup/data.bin\n");
    printf("#########################################\n");
}

void game_set_bomb(struct game *game) {
    struct player *player = game_get_player(game);
    /* cannot destroy a door */
    if ((map_get_cell_value(game_get_current_map(game), player_get_x(player), player_get_y(player)) & 0xf0) ==
        CELL_DOOR)
        return;

    if (player_get_nb_bomb(player) > 0) {
        struct bomb *bomb = malloc(bomb_get_size());
        if (!bomb)
            perror("malloc");

        /* initializing bomb properties */
        bomb_init(bomb, player_get_x(player), player_get_y(player), TTL4, SDL_GetTicks(), player_get_range(player), 0,
                  0, 0, 0, 0);
        /* adding bomb in BOMBS_ARRAY */
        struct bomb **map_bomb_array = map_get_bomb_array(game->maps[game->level]);
        for (int i = 0; i < NUM_MAX_BOMBS; i++)
            if (map_bomb_array[i] == NULL) {
                map_bomb_array[i] = bomb;
                break;
            }
        /* decrease player number of bombs */
        player_dec_nb_bomb(player);
    }
}

void game_pause(SDL_Event *event) {
    SDL_WaitEvent(event);
    do {
        /* game paused until next time we type P */
        SDL_WaitEvent(event);
    } while (event->key.keysym.sym != SDLK_p);
}

static short input_keyboard(struct game *game) {
    SDL_Event event = {0};
    struct player *player = game_get_player(game);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /* quit game with no backup */
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    /* quit game with backup */
                    case SDLK_ESCAPE:
                        /* backup game's data */
                        game_backup(game);
                        return 1;
                        /* going north */
                    case SDLK_UP:
                        player_set_current_way(player, NORTH);
                        player_move(game);
                        break;
                        /* going south */
                    case SDLK_DOWN:
                        player_set_current_way(player, SOUTH);
                        player_move(game);
                        break;
                        /* going east */
                    case SDLK_RIGHT:
                        player_set_current_way(player, EAST);
                        player_move(game);
                        break;
                        /* going west */
                    case SDLK_LEFT:
                        player_set_current_way(player, WEST);
                        player_move(game);
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
                break;
            }
            default:
                break;
        }
    }
    return 0;
}

int game_update(struct game *game) {
    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);
    if (input_keyboard(game)) {
        /* exit game */
        return 1;
    } /* if player is dead */
    else if (player_get_nb_life(player) == 0) {
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

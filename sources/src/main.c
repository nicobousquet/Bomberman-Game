/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/

#include "../include/game.h"
#include "../include/misc.h"
#include "../include/window.h"
#include "../include/constant.h"
#include <stdlib.h>
#include <SDL/SDL.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("Can't init SDL:  %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window_create(SIZE_BLOC * STATIC_MAP_WIDTH, SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    /* initializing game structure */
    struct game *game = game_new();

    /* to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms */
    int ideal_speed = 1000 / DEFAULT_GAME_FPS;
    int timer, execution_speed;

    /* game loop */
    /* static time rate implementation */
    int done = 0;
    while (!done) {
        timer = SDL_GetTicks();

        done = game_update(game);
        game_display(game);

        execution_speed = SDL_GetTicks() - timer;
        if (execution_speed < ideal_speed) {
            /* we are ahead of ideal time. let's wait */
            SDL_Delay(ideal_speed - execution_speed);
        }
    }

    game_free(game);

    SDL_Quit();

    return EXIT_SUCCESS;
}




#include "../include/game.h"
#include "../include/map.h"
#include "../include/misc.h"
#include "../include/window.h"
#include "../include/constant.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("Can't init SDL:  %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    struct game *game = game_new();

    window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)), SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    int ideal_speed = 1000 / DEFAULT_GAME_FPS;
    int timer, execution_speed;

    int done = 0;
    while (!done) {
        timer = SDL_GetTicks();

        done = game_update(game);
        game_display(game);

        execution_speed = SDL_GetTicks() - timer;
        if (execution_speed < ideal_speed) {
            SDL_Delay(ideal_speed - execution_speed);
        }
    }

    game_free(game);

    SDL_Quit();

    return EXIT_SUCCESS;
}



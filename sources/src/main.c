#include "../include/game.h"
#include "../include/misc.h"
#include "../include/constant.h"
#include "../include/timer.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("Can't init SDL:  %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    struct game *game = game_new();

    FILE *backup_file = fopen(FILENAME_BACKUP, "rb");

    if (backup_file) {
        game_read(game, backup_file);
        fclose(backup_file);
        remove(FILENAME_BACKUP);
    }

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    int ideal_duration = 1000 / DEFAULT_GAME_FPS;

    struct timer *timer = timer_new();
    bool done = false;

    while (!done) {
        timer_start(timer, ideal_duration);

        done = game_update(game);
        game_display(game);

        timer_update(timer);
        if (timer_get_state(timer) != IS_OVER) {
            SDL_Delay(timer_get_remaining(timer));
        }
    }

    timer_free(timer);
    game_free(game);

    SDL_Quit();

    return EXIT_SUCCESS;
}



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

    struct game *game = NULL;

    FILE *backup_file = fopen(FILENAME_BACKUP, "rb");

    if (backup_file) {
        game = game_read(backup_file);
        fclose(backup_file);
        remove(FILENAME_BACKUP);
    } else {
        game = game_new();
    }

    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    int ideal_duration = 1000 / DEFAULT_GAME_FPS;

    struct timer *timer = timer_new();
    int done = 0;

    while (!done) {
        timer_start(timer, ideal_duration);

        done = game_update(game);
        game_display(game);

        timer_update(timer);
        if (timer_is_over(timer) == 0) {
            SDL_Delay(timer_get_remaining(timer));
        }
    }

    timer_free(timer);
    game_free(game);

    SDL_Quit();

    return EXIT_SUCCESS;
}

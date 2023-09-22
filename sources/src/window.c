#include "../include/window.h"
#include "../include/constant.h"
#include "../include/misc.h"
#include <assert.h>
#include <stdlib.h>

struct SDL_Surface *window_create(int width, int height) {
    assert(width > 0 && height > 0);

    SDL_WM_SetCaption(WINDOW_NAME, NULL);
    SDL_Surface *window = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE);

    if (window == NULL) {
        error("Can't set video mode: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return window;
}

void window_display_image(SDL_Surface *window, SDL_Surface *sprite, int x, int y) {
    assert(window);
    assert(sprite);

    SDL_Rect place;

    place.x = x;
    place.y = y;

    SDL_BlitSurface(sprite, NULL, window, &place);
}

void window_clear(SDL_Surface *window) {
    assert(window);
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_refresh(SDL_Surface *window) {
    assert(window);
    SDL_Flip(window);
}

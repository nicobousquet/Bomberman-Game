#include "../include/misc.h"
#include <SDL/SDL_image.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void error(const char *s, ...) {
    va_list ap;
    va_start(ap, s);

    vfprintf(stderr, s, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

SDL_Surface *image_load(const char *filename) {
    SDL_Surface *img = IMG_Load(filename);

    if (!img) {
        error("IMG_Load: %s\n", IMG_GetError());
    }

    return img;
}

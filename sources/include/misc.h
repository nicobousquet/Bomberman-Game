#ifndef MISC_H
#define MISC_H

#include <SDL/SDL.h>

/**
 * @brief Print an error message on stderr and exit.
 * @param s The error message format string.
 * @param ... Additional arguments for the error message.
 */
void error(const char *s, ...);

/**
 * @brief Load an image from the specified filename.
 * @param filename The path to the image file.
 * @return The loaded SDL surface.
 * @note If the image loading fails, it raises an error.
 */
SDL_Surface *image_load(const char *filename);

#endif /* MISC_H */
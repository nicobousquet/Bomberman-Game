#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>

/**
 * @brief Create the game window with the specified width and height.
 * @param width The width of the window.
 * @param height The height of the window.
 */
struct SDL_Surface *window_create(int width, int height);

/**
 * @brief Free the game window.
 */
void window_free(SDL_Surface *window);

/**
 * @brief Refresh the game window.
 */
void window_refresh(SDL_Surface *window);

/**
 * @brief Display an SDL surface at the specified location.
 * @param surface The SDL surface to display.
 * @param x The x-coordinate of the location.
 * @param y The y-coordinate of the location.
 */
void window_display_image(SDL_Surface *window, SDL_Surface *surface, int x, int y);

/**
 * @brief Set every pixel of the window to white.
 */
void window_clear(SDL_Surface *window);

#endif /* WINDOW_H */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

/**
 * @brief Create the game window with the specified width and height.
 * @param width The width of the window.
 * @param height The height of the window.
 */
void window_create(int width, int height);

/**
 * @brief Free the game window.
 */
void window_free();

/**
 * @brief Refresh the game window.
 */
void window_refresh();

/**
 * @brief Display an SDL surface at the specified location.
 * @param surface The SDL surface to display.
 * @param x The x-coordinate of the location.
 * @param y The y-coordinate of the location.
 */
void window_display_image(SDL_Surface *surface, int x, int y);

/**
 * @brief Set every pixel of the window to white.
 */
void window_clear();

#endif /* WINDOW_H_ */
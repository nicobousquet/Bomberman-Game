#ifndef DIRECTION_H
#define DIRECTION_H

#include <stdint.h>

/**
 * @file direction.h
 * @brief This file contains the declaration of the direction enumeration and related functions.
 */

/**
 * @enum direction
 * @brief Represents a direction.
 */
enum direction {
    NORTH = 0, /**< North direction */
    EAST,      /**< East direction */
    SOUTH,     /**< South direction */
    WEST       /**< West direction */
};

/**
 * @brief Get the x-coordinate based on the source x-coordinate, direction, and delta.
 *
 * This function calculates the resulting x-coordinate by applying the given delta value
 * in the specified direction to the source x-coordinate.
 *
 * @param direction The direction.
 * @param x_src The source x-coordinate.
 * @param delta The delta value to apply.
 * @return The resulting x-coordinate.
 */
int direction_get_x(enum direction, int x_src, int delta);

/**
 * @brief Get the y-coordinate based on the source y-coordinate, direction, and delta.
 *
 * This function calculates the resulting y-coordinate by applying the given delta value
 * in the specified direction to the source y-coordinate.
 *
 * @param direction The direction.
 * @param y_src The source y-coordinate.
 * @param delta The delta value to apply.
 * @return The resulting y-coordinate.
 */
int direction_get_y(enum direction, int y_src, int delta);

/**
 * @brief Get a random direction on the grid.
 * @param x_src x_coordinate of the source position.
 * @param y_src y_coordinate of the source position.
 * @param grid The grid, where 1 is an obstacle, 0 otherwise.
 * @param grid_width The grid_width of the grid.
 * @param grid_height The grid_height of the grid.
 * @return A random direction on the grid.
 */
enum direction direction_get_random(int x_src, int y_src, const int *grid, int grid_width, int grid_height);

#endif // DIRECTION_H
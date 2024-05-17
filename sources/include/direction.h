#ifndef DIRECTION_H
#define DIRECTION_H

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

enum direction direction_get_from_coordinates(int x_src, int y_src, int x_dest, int y_dest);

#endif // DIRECTION_H

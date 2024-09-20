#include "../include/direction.h"
#include <stdio.h>
#include <stdlib.h>

int direction_get_x(enum direction direction, int x_src, int delta) {
    int x_dest = x_src;

    if (direction == EAST) {
        x_dest = x_src + delta;
    } else if (direction == WEST) {
        x_dest = x_src - delta;
    }

    return x_dest;
}

int direction_get_y(enum direction direction, int y_src, int delta) {
    int y_dest = y_src;

    if (direction == NORTH) {
        y_dest = y_src - delta;
    } else if (direction == SOUTH) {
        y_dest = y_src + delta;
    }

    return y_dest;
}

enum direction direction_get_from_coordinates(int x_src, int y_src, int x_dest, int y_dest) {
    int delta_x = x_dest - x_src;
    int delta_y = y_dest - y_src;

    if (delta_x >= 1 && delta_y == 0) {
        return EAST;
    }

    if (delta_x <= -1 && delta_y == 0) {
        return WEST;
    }

    if (delta_x == 0 && delta_y <= -1) {
        return NORTH;
    }

    if (delta_x == 0 && delta_y >= 1) {
        return SOUTH;
    }

    perror("No direction");
    exit(EXIT_FAILURE);
}

#include "../include/direction.h"

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
#include "../include/direction.h"

int direction_get_x(int x_src, enum direction direction, int delta) {
    int x_dst = x_src;

    if (direction == EAST) {
        x_dst = x_src + delta;
    } else if (direction == WEST) {
        x_dst = x_src - delta;
    }

    return x_dst;
}

int direction_get_y(int y_src, enum direction direction, int delta) {
    int y_dst = y_src;

    if (direction == NORTH) {
        y_dst = y_src - delta;
    } else if (direction == SOUTH) {
        y_dst = y_src + delta;
    }

    return y_dst;
}

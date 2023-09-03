#include "../include/direction.h"
#include <stdlib.h>

int direction_get_x(int x_src, enum direction direction, int delta) {
    int x_dest = x_src;

    if (direction == EAST) {
        x_dest = x_src + delta;
    } else if (direction == WEST) {
        x_dest = x_src - delta;
    }

    return x_dest;
}

int direction_get_y(int y_src, enum direction direction, int delta) {
    int y_dest = y_src;

    if (direction == NORTH) {
        y_dest = y_src - delta;
    } else if (direction == SOUTH) {
        y_dest = y_src + delta;
    }

    return y_dest;
}

enum direction direction_get_random() {
    enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};

    return directions[rand() % 4];
}

#include "../include/direction.h"
#include <stdlib.h>

/**
 * @brief Macro to calculate the index of a cell in the map given its row and column.
 */
#define CELL(i, j) ((i) + (j) * grid_width)

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

static int is_inside(int x, int y, int grid_width, int grid_height) {

    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
        return 1;
    }

    return 0;
}

enum direction direction_get_random(int x_src, int y_src, const int *grid, int grid_width, int grid_height) {
    int visited_directions[4] = {0, 0, 0, 0};

    while (visited_directions[NORTH] != 1 || visited_directions[SOUTH] != 1 || visited_directions[EAST] != 1 || visited_directions[WEST] != 1) {
        enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};
        enum direction direction = directions[rand() % 4];

        if (visited_directions[direction] != 1) {
            int next_x = direction_get_x(direction, x_src, 1);
            int next_y = direction_get_y(direction, y_src, 1);

            if (is_inside(next_x, next_y, grid_width, grid_height)) {
                if (grid[CELL(next_x, next_y)] == 0) {
                    return direction;
                }
            }

            visited_directions[direction] = 1;
        }
    }

    return NORTH;
}

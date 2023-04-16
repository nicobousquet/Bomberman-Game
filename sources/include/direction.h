#ifndef SOURCES_DIRECTION_H
#define SOURCES_DIRECTION_H

enum direction {
    NORTH = 0,
    EAST,  // 1
    SOUTH, // 2
    WEST   // 3
};

int direction_get_x(int x_src, enum direction, int delta);

int direction_get_y(int y_src, enum direction, int delta);

#endif //SOURCES_DIRECTION_H

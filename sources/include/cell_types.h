#ifndef CELL_TYPES_H_
#define CELL_TYPES_H_

enum bomb_type {
    EXPLOSION = 1,
    TTL1,
    TTL2,
    TTL3,
    TTL4
};

enum bonus_type {
    EMPTY = 0,
    BONUS_BOMB_RANGE_DEC,
    BONUS_BOMB_RANGE_INC,
    BONUS_BOMB_NB_DEC,
    BONUS_BOMB_NB_INC,
    BONUS_LIFE,
    BONUS_MONSTER
};

enum scenery_type {
    SCENERY_STONE = 1,    // 0001
    SCENERY_TREE = 2,    // 0010
    SCENERY_PRINCESS = 4  // 0100
};

enum door_status {
    OPENED = 0,
    CLOSED
};

enum levels {
    LEVEL1 = 0x00,
    LEVEL2 = 0x02,
    LEVEL3 = 0x04,
    LEVEL4 = 0x06,
    LEVEL5 = 0x08,
    LEVEL6 = 0x0a,
    LEVEL7 = 0x0c,
    LEVEL8 = 0x0e
};

enum cell_type {
    CELL_EMPTY = 0x00,        //  0000 0000
    CELL_SCENERY = 0x10,        //  0001 0000
    CELL_BOX = 0x20,        //  0010 0000 = 32
    CELL_DOOR = 0x30,        //  0011 0000
    CELL_KEY = 0x40,        //  0100 0000
    CELL_BONUS = 0x50,        // 	0101 0000
    CELL_MONSTER = 0x60,        // 	0110 0000 = 96
    CELL_BOMB = 0x70,        // 	0111 0000
};

#endif //CELL_TYPES_H_

#ifndef CELL_TYPES_H_
#define CELL_TYPES_H_

/**
 * @brief Enumeration of bomb types.
 */
enum bomb_type {
    EXPLOSION = 1, /**< Explosion type. */
    TTL1, /**< Time-to-live type 1. */
    TTL2, /**< Time-to-live type 2. */
    TTL3, /**< Time-to-live type 3. */
    TTL4 /**< Time-to-live type 4. */
};

/**
 * @brief Enumeration of bonus types.
 */
enum bonus_type {
    EMPTY = 0, /**< Empty bonus type. */
    BONUS_BOMB_RANGE_DEC, /**< Bonus type for decreasing bomb range. */
    BONUS_BOMB_RANGE_INC, /**< Bonus type for increasing bomb range. */
    BONUS_BOMB_NB_DEC, /**< Bonus type for decreasing bomb count. */
    BONUS_BOMB_NB_INC, /**< Bonus type for increasing bomb count. */
    BONUS_LIFE, /**< Bonus type for extra life. */
    BONUS_MONSTER /**< Bonus type for summoning a monster. */
};

/**
 * @brief Enumeration of scenery types.
 */
enum scenery_type {
    SCENERY_STONE = 1, /**< Stone scenery type. */
    SCENERY_TREE = 2, /**< Tree scenery type. */
    SCENERY_PRINCESS = 4 /**< Princess scenery type. */
};

/**
 * @brief Enumeration of door statuses.
 */
enum door_status {
    OPENED = 0, /**< Opened door status. */
    CLOSED /**< Closed door status. */
};

/**
 * @brief Enumeration of levels.
 */
enum levels {
    LEVEL1 = 0x00, /**< Level 1. */
    LEVEL2 = 0x02, /**< Level 2. */
    LEVEL3 = 0x04, /**< Level 3. */
    LEVEL4 = 0x06, /**< Level 4. */
    LEVEL5 = 0x08, /**< Level 5. */
    LEVEL6 = 0x0a, /**< Level 6. */
    LEVEL7 = 0x0c, /**< Level 7. */
    LEVEL8 = 0x0e /**< Level 8. */
};

/**
 * @brief Enumeration of cell types.
 */
enum cell_type {
    CELL_EMPTY = 0x00, /**< Empty cell type. */
    CELL_SCENERY = 0x10, /**< Scenery cell type. */
    CELL_BOX = 0x20, /**< Box cell type. */
    CELL_DOOR = 0x30, /**< Door cell type. */
    CELL_KEY = 0x40, /**< Key cell type. */
    CELL_BONUS = 0x50, /**< Bonus cell type. */
    CELL_MONSTER = 0x60, /**< Monster cell type. */
    CELL_BOMB = 0x70 /**< Bomb cell type. */
};

#endif //CELL_TYPES_H_
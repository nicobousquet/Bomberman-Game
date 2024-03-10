#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * @brief Name of the game window.
 */
#define WINDOW_NAME "Bombeirb"

/**
 * @brief Size (number of pixels) of a cell on the map.
 */
#define SIZE_BLOC 60

/**
 * @brief Size (number of pixels) of the banner line.
 */
#define LINE_HEIGHT 6

/**
 * @brief Size (number of pixels) of the banner height.
 */
#define BANNER_HEIGHT 60

/**
 * @brief Default frames per second for the game.
 */
#define DEFAULT_GAME_FPS 30

/**
 * @brief Maximum number of monsters per map allowed.
 */
#define NUM_MONSTERS_MAX 10

/**
 * @brief Maximum number of bombs per map allowed.
 */
#define NUM_BOMBS_MAX 9

/**
 * @brief Maximum number of keys allowed.
 */
#define NUM_KEYS_MAX 9

/**
 * @brief Maximum number of lives allowed.
 */
#define NUM_LIVES_MAX 9

/**
 * @brief Duration (in milliseconds) of a bomb state.
 */
#define DURATION_BOMB_TTL 1000

/**
 * @brief Duration (in milliseconds) of a monster's movement.
 */
#define DURATION_MONSTER_MOVE 1000

/**
 * @brief Duration (in milliseconds) of player's invincibility.
 */
#define DURATION_PLAYER_INVINCIBILITY 1000

/**
 * @brief Number of levels in the game.
 */
#define NUM_LEVELS 8

/**
 * @brief Macro to calculate the index of a cell in the map given its row and column.
 */
#define CELL(i, j) ((i) + (j) * map->width)

/**
 * @brief Number of different bonus types.
 */
#define NUM_BONUS_TYPES 6

/**
 * @brief File where game is saved.
 */
#define FILENAME_BACKUP "backup/data.bin"

#endif /* CONSTANT_H */
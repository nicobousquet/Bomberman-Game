#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "window.h"

/**
 * @enum strategy
 * @brief Represents the moving strategy of the monsters.
 */
enum strategy {
    RANDOM_STRATEGY,
    DIJKSTRA_STRATEGY
};

/**
 * @file map.h
 * @brief This file contains the declarations of functions and data types for the game map.
 */
struct map;

/**
 * @brief Create a new map from the file filename.
 * @param filename The filename to read the map from.
 * @return A pointer to the newly created map.
 */
struct map *map_new(char *filename);

/**
 * @brief Free the memory occupied by a map.
 * @param map A pointer to the map to be freed.
 */
void map_free(struct map *map);

/**
 * @brief Write a map to a file.
 * @param map The map to write.
 * @param file The file to write the map to.
 */
void map_write(struct map *map, FILE *file);

/**
 * @brief Read a map from a file.
 * @param map The map to read.
 * @param file The file to read the map from.
 */
void map_read(struct map *map, FILE *file);

/**
 * @brief Get the width of the map.
 * @param map A pointer to the map.
 * @return The width of the map.
 */
int map_get_width(struct map *map);

/**
 * @brief Get the height of the map.
 * @param map A pointer to the map.
 * @return The height of the map.
 */
int map_get_height(struct map *map);

/**
 * @brief Get the grid of the map.
 * @param map A pointer to the map.
 * @return A pointer to the grid.
 */
uint8_t *map_get_grid(struct map *map);

/**
 * @brief Set the grid data of the map.
 * @param map A pointer to the map.
 * @param grid A pointer to the grid.
 */
void map_set_grid(struct map *map, uint8_t *grid);

/**
 * @brief Get the list of bombs on the map.
 * @param map A pointer to the map.
 * @return A pointer to the list of bombs.
 */
struct bomb **map_get_list_bombs(struct map *map);

/**
 * @brief Get the list of monsters on the map.
 * @param map A pointer to the map.
 * @return A pointer to the list of monsters.
 */
struct monster **map_get_list_monsters(struct map *map);

/**
 * @brief Test if the specified coordinates (x, y) are within the map boundaries.
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return 1 if the coordinates are inside the map, 0 otherwise.
 */
int map_is_inside(struct map *map, int x, int y);

/**
 * @brief Get the value of the cell at the specified coordinates (x, y).
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return The value of the cell.
 */
uint8_t map_get_cell_value(struct map *map, int x, int y);

/**
 * @brief Set the value of the cell at the specified coordinates (x, y).
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param value The value to set for the cell.
 */
void map_set_cell_value(struct map *map, int x, int y, uint8_t value);

/**
 * @brief Display the map on the screen.
 * @param map A pointer to the map.
 */
void map_display(struct map *map, struct SDL_Surface *window, struct sprites *sprites);

/**
 * @brief Initialize the list of monsters on the map.
 * @param map A pointer to the map.
 */
void map_init_list_monsters(struct map *map);

/**
@brief Set a bomb on the map at the player's current position.
@param map A pointer to the map.
@param player A pointer to the player.
*/
void map_set_bomb(struct map *map, struct player *player);

/**
@brief Update the state of bombs on the map.
@param map A pointer to the map.
@param player A pointer to the player.
*/
void map_update_bombs(struct map *map, struct player *player);

/**
@brief Move the player on the map.
@param map A pointer to the map.
@param player A pointer to the player.
@param direction The direction where the player is moving.
@return 1 if player really moved, 0 otherwise.
*/
int map_move_player(struct map *map, struct player *player, enum direction direction);

/**
@brief Update the state of monsters and their position on the map.
@param map A pointer to the map.
@param player A pointer to the player.
*/
void map_update_monsters(struct map *map, struct player *player);

#endif /* MAP_H */
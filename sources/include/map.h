#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "window.h"
#include "bomb_node.h"
#include "monster_node.h"

/**
 * @enum strategy
 * @brief Represents the moving strategy of the monsters.
 */
enum strategy {
    RANDOM_STRATEGY,
    DIJKSTRA_STRATEGY
};

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
 * @param file The file to read the map from.
 * @return A pointer to the map read.
 */
struct map *map_read(FILE *file);

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
unsigned char *map_get_grid(struct map *map);

/**
 * @brief Set the grid data of the map.
 * @param map A pointer to the map.
 * @param grid A pointer to the grid.
 */
void map_set_grid(struct map *map, unsigned char *grid);

/**
 * @brief Add a bomb on the map.
 * @param map A pointer to the map.
 * @param to_add A pointer to the bomb to add.
 */
void map_add_bomb_node(struct map *map, struct bomb_node *to_add);

/**
 * @brief Add a bomb on the map.
 * @param map A pointer to the map.
 * @param to_remove A pointer to the bomb to remove.
 */
void map_remove_bomb_node(struct map *map, struct bomb_node *to_remove);

/**
 * @brief Add a monster on the map.
 * @param map A pointer to the map.
 * @param to_add A pointer to the monster to add.
 */
void map_add_monster_node(struct map *map, struct monster_node *to_add);

/**
 * @brief Remove a monster on the map.
 * @param map A pointer to the map.
 * @param to_remove A pointer to the monster to remove.
 */
void map_remove_monster_node(struct map *map, struct monster_node *to_remove);

/**
 * @brief Get the head of the bombs' linked list on the map.
 * @param map A pointer to the map.
 * @return A pointer to the list of bombs.
 */
struct bomb_node *map_get_bomb_head(struct map *map);

/**
 * @brief Get the head of the monsters' linked list on the map.
 * @param map A pointer to the map.
 * @return A pointer to the list of monsters.
 */
struct monster_node *map_get_monster_head(struct map *map);

/**
 * @brief Test if the specified coordinates (x, y) are within the map boundaries.
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return 1 if the coordinates are inside the map, 0 otherwise.
 */
int map_is_inside(struct map *map, int x, int y);

/**
 * @brief Get the movement strategy of the monsters.
 * @param map A pointer to the map.
 * @return The strategy of the monsters.
 */
enum strategy map_get_monster_strategy(struct map *map);

/**
 * @brief Get the value of the cell at the specified coordinates (x, y).
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return The value of the cell.
 */
unsigned char map_get_cell_value(struct map *map, int x, int y);

/**
 * @brief Set the value of the cell at the specified coordinates (x, y).
 * @param map A pointer to the map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param value The value to set for the cell.
 */
void map_set_cell_value(struct map *map, int x, int y, unsigned char value);

/**
 * @brief Display the map on the screen.
 * @param map A pointer to the map.
 */
void map_display(struct map *map, struct SDL_Surface *window, struct sprites *sprites);

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
@brief Checks if a monster_node will meet the player on the next move.
@param monster A pointer to the monster_node.
@param monster_direction The direction of the monster_node.
@return 1 if current monster_node will meet other monsters, 0 otherwise.
*/
int map_will_monster_meet_player(struct monster_node *monster, struct player *player, enum direction monster_direction);

int map_will_monster_meet_other_monsters(struct monster_node *current_monster, struct monster_node *monster_head, enum direction current_monster_direction);

/**
@brief Checks if a monster_node can move.
@param A pointer to the player.
@param monster A pointer to the monster_node.
@param monster_direction The direction of the monster_node.
@return 1 if current monster_node can move, 0 otherwise.
*/
int map_can_monster_move(struct map *map, struct player *player, struct monster_node *monster, enum direction monster_direction);

/**
@brief Meeting between a monster_node and the player.
@param monster A pointer to the monster_node.
@param player A pointer to the player.
@param monster_direction The direction of the monster_node.
*/
void map_monster_meeting_player(struct monster_node *monster, struct player *player, enum direction monster_direction);

#endif /* MAP_H */
#ifndef SOURCES_BOMB_NODE_H
#define SOURCES_BOMB_NODE_H

#include "cell_types.h"
#include "../include/direction.h"
#include <stdio.h>

/**
 * @brief Initialize a bomb node with the specified coordinates and range.
 * @param bomb A pointer to the bomb node.
 * @param x The x-coordinate of the bomb node.
 * @param y The y-coordinate of the bomb node.
 * @param range The range of the bomb node's explosion.
 */
struct bomb_node *bomb_node_new(int x, int y, int range);

/**
 * @brief Free the memory occupied by a bomb node.
 * @param bomb_node A pointer to the bomb node to be freed.
 */
void bomb_node_free(struct bomb_node *bomb_node);

/**
 * @brief Write a bomb node to a file.
 * @param bomb_node The bomb node to write.
 * @param file The file to write the bomb node to.
 */
void bomb_node_write(struct bomb_node *bomb_node, FILE *file);

/**
 * @brief Read a bomb node from a file.
 * @param bomb_node The bomb node to read.
 * @param file The file to read the bomb node from.
 */
void bomb_node_read(struct bomb_node *bomb_node, FILE *file);

/**
 * @brief Get the next bomb node in the linked list of bombs.
 * @param bomb_node A pointer to the bomb node.
 * @return The next bomb node in the linked list.
 */
struct bomb_node *bomb_node_get_next(struct bomb_node *bomb_node);

/**
 * @brief Set the next bomb node in the linked list of bombs.
 * @param bomb_node A pointer to the bomb node.
 * @param next A pointer to the next bomb node.
 */
void bomb_node_set_next(struct bomb_node *bomb_node, struct bomb_node *next);

/**
 * @brief Get the range of a bomb node in a direction.
 * @param bomb_node A pointer to the bomb node.
 * @param direction The direction to get the range from.
 * @return The range of the bomb node in the direction.
 */
int bomb_node_get_direction_range(struct bomb_node *bomb_node, enum direction direction);

/**
 * @brief Set the range of a bomb node in a direction.
 * @param bomb_node A pointer to the bomb node.
 * @param direction The direction to set the range from.
 * @param range The range of the bomb node to set.
 */
void bomb_node_set_direction_range(struct bomb_node *bomb_node, enum direction direction, int range);

/**
 * @brief Get the x-coordinate of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @return The x-coordinate of the bomb node.
 */
int bomb_node_get_x(struct bomb_node *bomb_node);

/**
 * @brief Get the y-coordinate of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @return The y-coordinate of the bomb node.
 */
int bomb_node_get_y(struct bomb_node *bomb_node);

/**
 * @brief Set the x-coordinate of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @param x The x-coordinate to set.
 */
void bomb_node_set_x(struct bomb_node *bomb_node, int x);

/**
 * @brief Set the y-coordinate of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @param y The y-coordinate to set.
 */
void bomb_node_set_y(struct bomb_node *bomb_node, int y);

/**
 * @brief Get the time to live (TTL) of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @return The time to live of the bomb node.
 */
enum bomb_type bomb_node_get_ttl(struct bomb_node *bomb_node);

/**
 * @brief Decrement the time to live (TTL) of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 */
void bomb_node_dec_ttl(struct bomb_node *bomb_node);

/**
 * @brief Set the time to live (TTL) of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @param ttl The time to live to set.
 */
void bomb_node_set_ttl(struct bomb_node *bomb_node, enum bomb_type ttl);

/**
 * @brief Get the timer of the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @return A pointer to the timer of the bomb node.
 */
struct timer *bomb_node_get_timer(struct bomb_node *bomb_node);

/**
 * @brief Set the timer for the bomb node.
 * @param bomb_node A pointer to the bomb node.
 * @param timer A pointer to the timer to set.
 */
void bomb_node_set_timer(struct bomb_node *bomb_node, struct timer *timer);

#endif // SOURCES_BOMB_NODE_H

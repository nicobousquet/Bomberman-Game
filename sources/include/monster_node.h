#ifndef MONSTER_NODE_H
#define MONSTER_NODE_H

#include "timer.h"
#include "sprites.h"

/**
 * @brief Initialize a monster node with the specified coordinates and timer duration.
 * @param x The x-coordinate of the monster node.
 * @param y The y-coordinate of the monster node.
 * @param timer_duration The duration of the monster node's timer.
 * @return A pointer to the initialized monster node.
 */
struct monster_node *monster_node_new(int x, int y);

/**
 * @brief Free the memory occupied by a monster node.
 * @param monster_node A pointer to the monster node to be freed.
 */
void monster_node_free(struct monster_node *monster_node);

/**
 * @brief Write a monster node to a file.
 * @param monster_node The monster node to write.
 * @param file The file to write the monster node to.
 */
void monster_node_write(struct monster_node *monster_node, FILE *file);

/**
 * @brief Read a monster node from a file.
 * @param file The file to read the monster node from.
 * @return A pointer to the monster read.
 */
struct monster_node *monster_node_read(FILE *file);

/**
 * @brief Set the x-coordinate of the monster node.
 * @param monster_node A pointer to the monster node.
 * @param x The x-coordinate to set.
 */
void monster_node_set_x(struct monster_node *monster_node, int x);

/**
 * @brief Set the y-coordinate of the monster node.
 * @param monster_node A pointer to the monster node.
 * @param y The y-coordinate to set.
 */
void monster_node_set_y(struct monster_node *monster_node, int y);

/**
 * @brief Get the monster node.
 * @param monster_node A pointer to the monster node.
 * @return The next monster node.
 */
struct monster_node *monster_node_get_next(struct monster_node *monster_node);

/**
 * @brief Set the next monster node.
 * @param monster_node A pointer to the monster node.
 * @param next A pointer to the next monster node.
 */
void monster_node_set_next(struct monster_node *monster_node, struct monster_node *next);

/**
 * @brief Get the x-coordinate of the monster node.
 * @param monster_node A pointer to the monster node.
 * @return The x-coordinate of the monster node.
 */
int monster_node_get_x(struct monster_node *monster_node);

/**
 * @brief Get the y-coordinate of the monster node.
 * @param monster_node A pointer to the monster node.
 * @return The y-coordinate of the monster node.
 */
int monster_node_get_y(struct monster_node *monster_node);

/**
 * @brief Get the current direction of the monster node.
 * @param monster_node A pointer to the monster node.
 * @return The current direction of the monster node.
 */
enum direction monster_node_get_direction(struct monster_node *monster_node);

/**
 * @brief Set the direction of the monster node.
 * @param monster_node A pointer to the monster node.
 * @param direction The direction to set for the monster node.
 */
void monster_node_set_direction(struct monster_node *monster_node, enum direction direction);

/**
 * @brief Get the timer of the monster node.
 * @param monster_node A pointer to the monster node.
 * @return A pointer to the timer of the monster node.
 */
struct timer *monster_node_get_timer(struct monster_node *monster_node);

/**
 * @brief Set the timer for the monster node.
 * @param monster_node A pointer to the monster node.
 * @param timer A pointer to the timer to set.
 */
void monster_node_set_timer(struct monster_node *monster_node, struct timer *timer);

/**
 * @brief Display the sprite of the monster node.
 * @param monster_node A pointer to the monster node.
 */
void monster_node_display(struct monster_node *monster_node, SDL_Surface *window, struct sprites *sprites);

/**
 * @brief Move the monster node.
 * @param monster_node A pointer to the monster node.
 * @param direction The direction where the monster node is moving.
 */
void monster_node_move(struct monster_node *monster_node, enum direction direction);

#endif // MONSTER_NODE_H

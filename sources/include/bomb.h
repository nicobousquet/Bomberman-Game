#ifndef SOURCES_BOMB_H
#define SOURCES_BOMB_H

#include "../include/cell_types.h"
#include <stdio.h>

/**
 * @brief Initialize a bomb with the specified coordinates and range.
 * @param bomb A pointer to the bomb.
 * @param x The x-coordinate of the bomb.
 * @param y The y-coordinate of the bomb.
 * @param range The range of the bomb's explosion.
 */
struct bomb *bomb_new(int x, int y, int range);

/**
 * @brief Free the memory occupied by a bomb.
 * @param bomb A pointer to the bomb to be freed.
 */
void bomb_free(struct bomb *bomb);

/**
 * @brief Write a bomb to a file.
 * @param bomb The bomb to write.
 * @param file The file to write the bomb to.
 */
void bomb_write(struct bomb *bomb, FILE *file);

/**
 * @brief Read a bomb from a file.
 * @param bomb The bomb to read.
 * @param file The file to read the bomb from.
 */
void bomb_read(struct bomb *bomb, FILE *file);

/**
 * @brief Get the x-coordinate of the bomb.
 * @param bomb A pointer to the bomb.
 * @return The x-coordinate of the bomb.
 */
int bomb_get_x(struct bomb *bomb);

/**
 * @brief Get the y-coordinate of the bomb.
 * @param bomb A pointer to the bomb.
 * @return The y-coordinate of the bomb.
 */
int bomb_get_y(struct bomb *bomb);

/**
 * @brief Set the x-coordinate of the bomb.
 * @param bomb A pointer to the bomb.
 * @param x The x-coordinate to set.
 */
void bomb_set_x(struct bomb *bomb, int x);

/**
 * @brief Set the y-coordinate of the bomb.
 * @param bomb A pointer to the bomb.
 * @param y The y-coordinate to set.
 */
void bomb_set_y(struct bomb *bomb, int y);

/**
 * @brief Get the time to live (TTL) of the bomb.
 * @param bomb A pointer to the bomb.
 * @return The time to live of the bomb.
 */
enum bomb_type bomb_get_ttl(struct bomb *bomb);

/**
 * @brief Decrement the time to live (TTL) of the bomb.
 * @param bomb A pointer to the bomb.
 */
void bomb_dec_ttl(struct bomb *bomb);

/**
 * @brief Set the time to live (TTL) of the bomb.
 * @param bomb A pointer to the bomb.
 * @param ttl The time to live to set.
 */
void bomb_set_ttl(struct bomb *bomb, enum bomb_type ttl);

/**
 * @brief Get the range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The range of the bomb's explosion.
 */
int bomb_get_range(struct bomb *bomb);

/**
 * @brief Get the timer of the bomb.
 * @param bomb A pointer to the bomb.
 * @return A pointer to the timer of the bomb.
 */
struct timer *bomb_get_timer(struct bomb *bomb);

/**
 * @brief Set the timer for the bomb.
 * @param bomb A pointer to the bomb.
 * @param timer A pointer to the timer to set.
 */
void bomb_set_timer(struct bomb *bomb, struct timer *timer);

#endif // SOURCES_BOMB_H

#ifndef SOURCES_BOMB_H_
#define SOURCES_BOMB_H_

#include <stdio.h>

/**
 * @file bomb.h
 * @brief This file contains the declarations of functions and data types for bombs.
 */

struct bomb;

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

void bomb_write(struct bomb *bomb, FILE *file);

void bomb_read(struct bomb *bomb, FILE *file);

/**
 * @brief Get the size of the bomb structure.
 * @return The size of the bomb structure.
 */
int bomb_get_size();

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
 * @brief Get the range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The range of the bomb's explosion.
 */
int bomb_get_range(struct bomb *bomb);

/**
 * @brief Get the north range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The north range of the bomb's explosion.
 */
int bomb_get_north_range(struct bomb *bomb);

/**
 * @brief Get the south range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The south range of the bomb's explosion.
 */
int bomb_get_south_range(struct bomb *bomb);

/**
 * @brief Get the east range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The east range of the bomb's explosion.
 */
int bomb_get_east_range(struct bomb *bomb);

/**
 * @brief Get the west range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return The west range of the bomb's explosion.
 */
int bomb_get_west_range(struct bomb *bomb);

/**
 * @brief Set the north range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @param north_range The north range to set.
 */
void bomb_set_north_range(struct bomb *bomb, int north_range);

/**
 * @brief Set the south range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @param south_range The south range to set.
 */
void bomb_set_south_range(struct bomb *bomb, int south_range);

/**
 * @brief Set the east range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @param east_range The east range to set.
 */
void bomb_set_east_range(struct bomb *bomb, int east_range);

/**
 * @brief Set the west range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @param west_range The west range to set.
 */
void bomb_set_west_range(struct bomb *bomb, int west_range);

/**
 * @brief Get a pointer to the north range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return A pointer to the north range of the bomb's explosion.
 */
int *bomb_get_north_range_ptr(struct bomb *bomb);

/**
 * @brief Get a pointer to the south range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return A pointer to the south range of the bomb's explosion.
 */
int *bomb_get_south_range_ptr(struct bomb *bomb);

/**
 * @brief Get a pointer to the east range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return A pointer to the east range of the bomb's explosion.
 */
int *bomb_get_east_range_ptr(struct bomb *bomb);

/**
 * @brief Get a pointer to the west range of the bomb's explosion.
 * @param bomb A pointer to the bomb.
 * @return A pointer to the west range of the bomb's explosion.
 */
int *bomb_get_west_range_ptr(struct bomb *bomb);

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

#endif // SOURCES_BOMB_H_

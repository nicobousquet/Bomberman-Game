#ifndef MONSTER_H_
#define MONSTER_H_

#include "direction.h"
#include "timer.h"
#include "sprites.h"
#include <SDL/SDL.h>
#include <stdio.h>

/**
 * @file monster.h
 * @brief This file contains the declarations of functions and data types for monsters.
 */
struct monster;

/**
 * @brief Initialize a monster with the specified coordinates and timer duration.
 * @param x The x-coordinate of the monster.
 * @param y The y-coordinate of the monster.
 * @param timer_duration The duration of the monster's timer.
 * @return A pointer to the initialized monster.
 */
struct monster *monster_new(int x, int y, int timer_duration);

/**
 * @brief Free the memory occupied by a monster.
 * @param monster A pointer to the monster to be freed.
 */
void monster_free(struct monster *monster);

/**
 * @brief Write a monster to a file.
 * @param monster The monster to write.
 * @param file The file to write the monster to.
 */
void monster_write(struct monster *monster, FILE *file);

/**
 * @brief Read a monster from a file.
 * @param monster The monster to read.
 * @param file The file to read the monster from.
 */
void monster_read(struct monster *monster, FILE *file);

/**
 * @brief Get the size of the monster structure.
 * @return The size of the monster structure.
 */
int monster_get_size();

/**
 * @brief Set the x-coordinate of the monster.
 * @param monster A pointer to the monster.
 * @param x The x-coordinate to set.
 */
void monster_set_x(struct monster *monster, int x);

/**
 * @brief Set the y-coordinate of the monster.
 * @param monster A pointer to the monster.
 * @param y The y-coordinate to set.
 */
void monster_set_y(struct monster *monster, int y);

/**
 * @brief Get the x-coordinate of the monster.
 * @param monster A pointer to the monster.
 * @return The x-coordinate of the monster.
 */
int monster_get_x(struct monster *monster);

/**
 * @brief Get the y-coordinate of the monster.
 * @param monster A pointer to the monster.
 * @return The y-coordinate of the monster.
 */
int monster_get_y(struct monster *monster);

/**
 * @brief Get the current direction of the monster.
 * @param monster A pointer to the monster.
 * @return The current direction of the monster.
 */
enum direction monster_get_direction(struct monster *monster);

/**
 * @brief Set the direction of the monster.
 * @param monster A pointer to the monster.
 * @param direction The direction to set for the monster.
 */
void monster_set_direction(struct monster *monster, enum direction direction);

/**
 * @brief Get the timer of the monster.
 * @param monster A pointer to the monster.
 * @return A pointer to the timer of the monster.
 */
struct timer *monster_get_timer(struct monster *monster);

/**
 * @brief Set the timer for the monster.
 * @param monster A pointer to the monster.
 * @param timer A pointer to the timer to set.
 */
void monster_set_timer(struct monster *monster, struct timer *timer);

/**
 * @brief Display the sprite of the monster.
 * @param monster A pointer to the monster.
 */
void monster_display(struct monster *monster, struct SDL_Surface *window, struct sprites *sprites);

/**
 * @brief Move the monster.
 * @param monster A pointer to the monster.
 */
void monster_move(struct monster *monster);

#endif // MONSTER_H_
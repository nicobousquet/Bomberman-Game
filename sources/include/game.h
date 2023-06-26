#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

/**
 * @file game.h
 * @brief This file contains the declarations of functions and data types for the game.
 */

struct game;

/**
 * @brief Create a new game.
 * @return A pointer to the newly created game.
 */
struct game *game_new(void);

/**
 * @brief Free the memory occupied by the game.
 * @param game A pointer to the game to be freed.
 */
void game_free(struct game *game);

void game_write(struct game *game, FILE *file);

void game_read(struct game *game, FILE *file);

/**
 * @brief Get the player of the game.
 * @param game A pointer to the game.
 * @return A pointer to the player of the game.
 */
struct player *game_get_player(struct game *game);

/**
 * @brief Get the current map of the game.
 * @param game A pointer to the game.
 * @return A pointer to the current map.
 */
struct map *game_get_current_map(struct game *game);

/**
 * @brief Set the current level of the game.
 * @param game A pointer to the game.
 * @param level The level to be set.
 */
void game_set_current_level(struct game *game, int level);

/**
 * @brief Display the game on the screen.
 * @param game A pointer to the game.
 */
void game_display(struct game *game);

/**
 * @brief Update the game state.
 * @param game A pointer to the game.
 * @return 1 if game is over, 0 otherwise.
 */
int game_update(struct game *game);

#endif /* GAME_H_ */

#ifndef GAME_H
#define GAME_H

#include <stdio.h>

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

/**
 * @brief Write the game to a file.
 * @param game The game to write.
 * @param file The file to write the game to.
 */
void game_write(struct game *game, FILE *file);

/**
 * @brief Read the game from a file.
 * @param file The file to read the game from.
 * @return A pointer to the game read.
 */
struct game *game_read(FILE *file);

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

#endif /* GAME_H */

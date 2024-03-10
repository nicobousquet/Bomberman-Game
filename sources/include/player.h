#ifndef PLAYER_H
#define PLAYER_H

#include "sprites.h"

/**
 * @file player.h
 * @brief This file contains the declaration of the player structure and related functions.
 */
struct player;

/**
 * @brief Creates a new player with a given number of available bombs.
 * @param num_bombs The number of bombs the player can initially have.
 * @return A pointer to the created player.
 */
struct player *player_new(int num_bombs);

/**
 * @brief Free the memory occupied by the player.
 * @param player A pointer to the player.
 */
void player_free(struct player *player);

/**
 * @brief Write the player to a file.
 * @param player The player to write.
 * @param file The file to write the player to.
 */
void player_write(struct player *player, FILE *file);

/**
 * @brief Read the player from a file.
 * @param player The player to read.
 * @param file The file to read the player from.
 */
void player_read(struct player *player, FILE *file);

/**
 * @brief Get the current x-coordinate of the player's position.
 * @param player A pointer to the player.
 * @return The current x-coordinate of the player's position.
 */
int player_get_x(struct player *player);

/**
 * @brief Get the current y-coordinate of the player's position.
 * @param player A pointer to the player.
 * @return The current y-coordinate of the player's position.
 */
int player_get_y(struct player *player);

/**
 * @brief Set the current x-coordinate of the player's position.
 * @param player A pointer to the player.
 * @param x The x-coordinate to set.
 */
void player_set_x(struct player *player, int x);

/**
 * @brief Set the current y-coordinate of the player's position.
 * @param player A pointer to the player.
 * @param y The y-coordinate to set.
 */
void player_set_y(struct player *player, int y);

/**
 * @brief Get the range of bombs for the player.
 * @param player A pointer to the player.
 * @return The range of bombs for the player.
 */
int player_get_range_bombs(struct player *player);

/**
 * @brief Increase the range of bombs explosion.
 * @param player A pointer to the player.
 */
void player_inc_range_bombs(struct player *player);

/**
 * @brief Decrease the range of bombs explosion.
 * @param player A pointer to the player.
 */
void player_dec_range_bombs(struct player *player);

/**
 * @brief Get the direction of the player.
 * @param player A pointer to the player.
 * @return The direction of the player.
 */
enum direction player_get_direction(struct player *player);

/**
 * @brief Set the direction of the next move for the player.
 * @param player A pointer to the player.
 * @param direction The direction to set.
 */
void player_set_direction(struct player *player, enum direction direction);

/**
 * @brief Get the number of bombs the player has.
 * @param player A pointer to the player.
 * @return The number of bombs the player has.
 */
int player_get_num_bomb(struct player *player);

/**
 * @brief Set the number of bombs the player has.
 * @param player A pointer to the player.
 * @param num_bombs The number of bombs to set.
 */
void player_set_num_bombs(struct player *player, int num_bombs);

/**
 * @brief Increase the number of bombs the player has.
 * @param player A pointer to the player.
 */
void player_inc_num_bomb(struct player *player);

/**
 * @brief Decrease the number of bombs the player has.
 * @param player A pointer to the player.
 */
void player_dec_num_bomb(struct player *player);

/**
 * @brief Get the number of keys the player has.
 * @param player A pointer to the player.
 * @return The number of keys the player has.
 */
int player_get_num_keys(struct player *player);

/**
 * @brief Increase the number of keys the player has.
 * @param player A pointer to the player.
 */
void player_inc_num_keys(struct player *player);

/**
 * @brief Decrease the number of keys the player has.
 * @param player A pointer to the player.
 */
void player_dec_num_keys(struct player *player);

/**
 * @brief Display the player on the screen.
 * @param player A pointer to the player.
 */
void player_display(struct player *player, struct SDL_Surface *window, struct sprites *sprites);

/**
 * @brief Get the number of lives the player has.
 * @param player A pointer to the player.
 * @return The number of lives the player has.
 */
int player_get_num_lives(struct player *player);

/**
 * @brief Increase the number of lives for the player.
 * @param player A pointer to the player.
 */
void player_inc_num_lives(struct player *player);

/**
 * @brief Decrease the number of lives for the player.
 * @param player A pointer to the player.
 */
void player_dec_num_lives(struct player *player);

/**
 * @brief Get the timer for invincibility of the player.
 * @param player A pointer to the player.
 * @return A pointer to the timer for invincibility of the player.
 */
struct timer *player_get_timer_invincibility(struct player *player);

/**
 * @brief Set the timer for invincibility of the player.
 * @param player A pointer to the player.
 * @param timer_invincibility A pointer to the timer for invincibility to set.
 */
void player_set_timer_invincibility(struct player *player, struct timer *timer_invincibility);

/**
 * @brief Get the bonus of the specified type for the player.
 * @param player A pointer to the player.
 * @param bonus_type The type of bonus to get.
 */
void player_get_bonus(struct player *player, enum bonus_type bonus_type);

/**
 * @brief Move the player.
 * @param player A pointer to the player.
 * @param direction The direction where the player is moving.
 */
void player_move(struct player *player, enum direction direction);

#endif /* PLAYER_H */

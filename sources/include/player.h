#ifndef PLAYER_H_
#define PLAYER_H_

#include "direction.h"
#include "cell_types.h"

struct player;

/**
 * @file player.h
 * @brief This file contains the declaration of the player structure and related functions.
 */

/**
 * @brief Creates a new player with a given number of available bombs.
 * @param num_bombs The number of bombs the player can initially have.
 * @return A pointer to the created player.
 */
struct player *player_init(int num_bombs);

/**
 * @brief Get the direction of the player.
 * @param player A pointer to the player.
 * @return The direction of the player.
 */
enum direction player_get_direction(struct player *player);

/**
 * @brief Free the memory occupied by the player.
 * @param player A pointer to the player.
 */
void player_free(struct player *player);

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
 * @brief Increase the range of bombs for the player.
 * @param player A pointer to the player.
 */
void player_inc_bombs_range(struct player *player);

/**
 * @brief Decrease the range of bombs for the player.
 * @param player A pointer to the player.
 */
void player_dec_bombs_range(struct player *player);

/**
 * @brief Increase the number of lives for the player.
 * @param player A pointer to the player.
 */
void player_inc_num_lives(struct player *player);

/**
 * @brief Set the direction of the next move for the player.
 * @param player A pointer to the player.
 * @param direction The direction to set.
 */
void player_set_current_way(struct player *player, enum direction direction);

/**
 * @brief Get the number of bombs the player can put.
 * @param player A pointer to the player.
 * @return The number of bombs the player can put.
 */
int player_get_num_bomb(struct player *player);

/**
 * @brief Increase the number of bombs the player can put.
 * @param player A pointer to the player.
 */
void player_inc_num_bomb(struct player *player);

/**
 * @brief Decrease the number of bombs the player can put.
 * @param player A pointer to the player.
 */
void player_dec_num_bomb(struct player *player);

/**
 * @brief Set the number of bombs the player can put.
 * @param player A pointer to the player.
 * @param num_bombs The number of bombs to set.
 */
void player_set_num_bombs(struct player *player, int num_bombs);

/**
 * @brief Decrease the number of keys the player has.
 * @param player A pointer to the player.
 */
void player_dec_num_keys(struct player *player);

/**
 * @brief Display the player on the screen.
 * @param player A pointer to the player.
 */
void player_display(struct player *player);

/**
 * @brief Get the number of lives the player has.
 * @param player A pointer to the player.
 * @return The number of lives the player has.
 */
int player_get_num_lives(struct player *player);

/**
 * @brief Decrease the number of lives for the player.
 * @param player A pointer to the player.
 */
void player_dec_num_lives(struct player *player);

/**
 * @brief Increase the number of keys the player has.
 * @param player A pointer to the player.
 */
void player_inc_num_keys(struct player *player);

/**
 * @brief Get the size of the player structure.
 * @return The size of the player structure in bytes.
 */
int player_get_size();

/**
 * @brief Get the direction of the player.
 * @param player A pointer to the player.
 * @return The direction of the player.
 */
enum direction player_get_direction(struct player *player);

/**
 * @brief Get the range of bombs for the player.
 * @param player A pointer to the player.
 * @return The range of bombs for the player.
 */
int player_get_bombs_range(struct player *player);

/**
 * @brief Get the number of keys the player has.
 * @param player A pointer to the player.
 * @return The number of keys the player has.
 */
int player_get_num_keys(struct player *player);

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

#endif /* PLAYER_H_ */

#ifndef SPRITES_H_
#define SPRITES_H_

#include <SDL/SDL.h>
#include "direction.h"
#include "cell_types.h"

struct sprites;

int sprites_get_size();

/**
 * @brief Load the game's sprites.
 */
void sprites_load(struct sprites *sprites);

/**
 * @brief Free the loaded sprites.
 */
void sprites_free(struct sprites *sprites);

/**
 * @brief Get the sprite for the princess.
 * @return The SDL surface for the princess sprite.
 */
SDL_Surface *sprites_get_princess(struct sprites *sprites);

/**
 * @brief Get the sprite for the player in a specific direction.
 * @param direction The direction of the player.
 * @return The SDL surface for the player sprite in the specified direction.
 */
SDL_Surface *sprites_get_player(struct sprites *sprites, enum direction direction);

/**
 * @brief Get the sprite for a bonus of a specific type.
 * @param bonus_type The type of bonus.
 * @return The SDL surface for the bonus sprite of the specified type.
 */
SDL_Surface *sprites_get_bonus(struct sprites *sprites, enum bonus_type bonus_type);

/**
 * @brief Get the sprite for a tree.
 * @return The SDL surface for the tree sprite.
 */
SDL_Surface *sprites_get_tree(struct sprites *sprites);

/**
 * @brief Get the sprite for a box.
 * @return The SDL surface for the box sprite.
 */
SDL_Surface *sprites_get_box(struct sprites *sprites);

/**
 * @brief Get the sprite for a key.
 * @return The SDL surface for the key sprite.
 */
SDL_Surface *sprites_get_key(struct sprites *sprites);

/**
 * @brief Get the sprite for a stone.
 * @return The SDL surface for the stone sprite.
 */
SDL_Surface *sprites_get_stone(struct sprites *sprites);

/**
 * @brief Get the sprite for a door with a specific status.
 * @param status The status of the door.
 * @return The SDL surface for the door sprite with the specified status.
 */
SDL_Surface *sprites_get_door(struct sprites *sprites, enum door_status status);

/**
 * @brief Get the sprite for a number.
 * @param number The number to display.
 * @return The SDL surface for the number sprite.
 */
SDL_Surface *sprites_get_number(struct sprites *sprites, short number);

/**
 * @brief Get the sprite for the life banner.
 * @return The SDL surface for the life banner sprite.
 */
SDL_Surface *sprites_get_banner_life(struct sprites *sprites);

/**
 * @brief Get the sprite for the bomb banner.
 * @return The SDL surface for the bomb banner sprite.
 */
SDL_Surface *sprites_get_banner_bomb(struct sprites *sprites);

/**
 * @brief Get the sprite for the line banner.
 * @return The SDL surface for the line banner sprite.
 */
SDL_Surface *sprites_get_banner_line(struct sprites *sprites);

/**
 * @brief Get the sprite for the vertical line banner.
 * @return The SDL surface for the vertical line banner sprite.
 */
SDL_Surface *sprites_get_banner_vertical_line(struct sprites *sprites);

/**
 * @brief Get the sprite for the range banner.
 * @return The SDL surface for the range banner sprite.
 */
SDL_Surface *sprites_get_banner_range(struct sprites *sprites);

/**
 * @brief Get the sprite for a bomb with a specific time to live.
 * @param ttl The time to live of the bomb.
 * @return The SDL surface for the bomb sprite with the specified time to live.
 */
SDL_Surface *sprites_get_bomb(struct sprites *sprites, uint8_t ttl);

/**
 * @brief Get the sprite for a monster in a specific direction.
 * @param direction The direction of the monster.
 * @return The SDL surface for the monster sprite in the specified direction.
 */
SDL_Surface *sprites_get_monster(struct sprites *sprites, enum direction direction);

/**
 * @brief Get the sprite for a scenery of a specific type.
 * @param type The type of scenery.
 * @return The SDL surface for the scenery sprite of the specified type.
 */
SDL_Surface *sprites_get_scenery(struct sprites *sprites, enum scenery_type type);

#endif /* SPRITES_H_ */
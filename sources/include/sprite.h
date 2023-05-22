#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include "direction.h"
#include "cell_types.h"

/**
 * @brief Load the game's sprites.
 */
void sprite_load();

/**
 * @brief Free the loaded sprites.
 */
void sprite_free();

/**
 * @brief Get the sprite for the princess.
 * @return The SDL surface for the princess sprite.
 */
SDL_Surface *sprite_get_princess();

/**
 * @brief Get the sprite for the player in a specific direction.
 * @param direction The direction of the player.
 * @return The SDL surface for the player sprite in the specified direction.
 */
SDL_Surface *sprite_get_player(enum direction direction);

/**
 * @brief Get the sprite for a bonus of a specific type.
 * @param bonus_type The type of bonus.
 * @return The SDL surface for the bonus sprite of the specified type.
 */
SDL_Surface *sprite_get_bonus(enum bonus_type bonus_type);

/**
 * @brief Get the sprite for a tree.
 * @return The SDL surface for the tree sprite.
 */
SDL_Surface *sprite_get_tree();

/**
 * @brief Get the sprite for a box.
 * @return The SDL surface for the box sprite.
 */
SDL_Surface *sprite_get_box();

/**
 * @brief Get the sprite for a key.
 * @return The SDL surface for the key sprite.
 */
SDL_Surface *sprite_get_key();

/**
 * @brief Get the sprite for a stone.
 * @return The SDL surface for the stone sprite.
 */
SDL_Surface *sprite_get_stone();

/**
 * @brief Get the sprite for a door with a specific status.
 * @param status The status of the door.
 * @return The SDL surface for the door sprite with the specified status.
 */
SDL_Surface *sprite_get_door(enum door_status status);

/**
 * @brief Get the sprite for a number.
 * @param number The number to display.
 * @return The SDL surface for the number sprite.
 */
SDL_Surface *sprite_get_number(short number);

/**
 * @brief Get the sprite for the life banner.
 * @return The SDL surface for the life banner sprite.
 */
SDL_Surface *sprite_get_banner_life();

/**
 * @brief Get the sprite for the bomb banner.
 * @return The SDL surface for the bomb banner sprite.
 */
SDL_Surface *sprite_get_banner_bomb();

/**
 * @brief Get the sprite for the line banner.
 * @return The SDL surface for the line banner sprite.
 */
SDL_Surface *sprite_get_banner_line();

/**
 * @brief Get the sprite for the vertical line banner.
 * @return The SDL surface for the vertical line banner sprite.
 */
SDL_Surface *sprite_get_banner_vertical_line();

/**
 * @brief Get the sprite for the range banner.
 * @return The SDL surface for the range banner sprite.
 */
SDL_Surface *sprite_get_banner_range();

/**
 * @brief Get the sprite for a bomb with a specific time to live.
 * @param ttl The time to live of the bomb.
 * @return The SDL surface for the bomb sprite with the specified time to live.
 */
SDL_Surface *sprite_get_bomb(int ttl);

/**
 * @brief Get the sprite for a monster in a specific direction.
 * @param direction The direction of the monster.
 * @return The SDL surface for the monster sprite in the specified direction.
 */
SDL_Surface *sprite_get_monster(enum direction direction);

/**
 * @brief Get the sprite for a scenery of a specific type.
 * @param type The type of scenery.
 * @return The SDL surface for the scenery sprite of the specified type.
 */
SDL_Surface *sprite_get_scenery(enum scenery_type type);

#endif /* SPRITE_H_ */
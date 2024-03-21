#ifndef RANDOM_H
#define RANDOM_H

#include "map.h"

/**
 * @file random.h
 * @brief This file contains the declaration of the function responsible for the random movement of the monsters on the map.
 */

/**
@brief Update the state of monsters and their position randomly on the map.
@param map A pointer to the map.
@param player A pointer to the player.
*/
void random_update_monsters(struct map *map, struct player *player);

#endif /* RANDOM_H */
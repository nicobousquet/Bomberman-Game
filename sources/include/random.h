#ifndef RANDOM_H
#define RANDOM_H

#include "map.h"

void random_move_monster(struct map *map, struct monster_node *monster, struct player *player);

/**
@brief Update the state of monsters and their position randomly on the map.
@param map A pointer to the map.
@param player A pointer to the player.
*/
void random_update_monsters(struct map *map, struct player *player);

#endif /* RANDOM_H */
#ifndef MAP_H_
#define MAP_H_

#include <stdint.h>
#include "player.h"
#include "cell_types.h"

struct map;

// Create a new empty map
struct map *map_new(int width, int height);

void map_free(struct map *map);

// Return the height and width of a map
int map_get_width(struct map *map);

int map_get_height(struct map *map);

unsigned char *map_get_grid(struct map *map);

void map_set_grid(struct map *map, unsigned char *grid);

struct bomb **map_get_bombs_list(struct map *map);

struct monster **map_get_monsters_list(struct map *map);

int map_get_size();

// Test if (x,y) is within the map
int map_is_inside(struct map *map, int x, int y);

// Return the type of cell
uint8_t map_get_cell_value(struct map *map, int x, int y);

// Set the type of cell
void map_set_cell_value(struct map *map, int x, int y, uint8_t value);

// Display the map on the screen
void map_display(struct map *map);

struct map *map_read_new_map(char *filename);

void map_init_monsters_list(struct map *map);

void map_set_bomb(struct map *map, struct player *player);

void map_update_bombs(struct map *map, struct player *player);

void map_display_monsters(struct map *map);

int map_move_player(struct map *map, struct player *player);

void map_update_monsters(struct map *map, struct player *player);

#endif /* MAP_H_ */

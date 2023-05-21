#ifndef GAME_H_
#define GAME_H_

// Abstract data type
struct game;

// Create a new game
struct game *game_new(void);

// Free a game
void game_free(struct game *game);

// Return the player of the current game
struct player *game_get_player(struct game *game);

// Return the current map
struct map *game_get_current_map(struct game *game);

void game_set_current_level(struct game *game, int level);

// Display the game on the screen
void game_display(struct game *game);

// update
int game_update(struct game *game);

#endif /* GAME_H_ */

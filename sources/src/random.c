#include "../include/random.h"
#include <assert.h>
#include <time.h>

void random_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    static int i = 0;

    srand(time(NULL) + i++);

    struct monster_node *current = map_get_monster_head(map);

    while (current != NULL) {
        timer_update(monster_node_get_timer(current));

        if (timer_get_state(monster_node_get_timer(current)) == IS_OVER) {

            int visited_directions[4] = {0, 0, 0, 0};
            while (visited_directions[NORTH] != 1 || visited_directions[SOUTH] != 1 || visited_directions[EAST] != 1 || visited_directions[WEST] != 1) {
                enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};
                enum direction direction = directions[rand() % 4];

                if (visited_directions[direction] != 1) {
                    if (map_can_monster_move(map, player, current, direction)) {
                        if (map_will_monster_meet_player(current, player, direction)) {
                            map_monster_meeting_player(current, player, direction);
                            break;
                        } else {
                            monster_node_move(current, direction);
                            break;
                        }
                    }

                    visited_directions[direction] = 1;
                }
            }
        }

        current = monster_node_get_next(current);
    }
}
#include "../include/random.h"
#include "../include/constant.h"
#include <assert.h>
#include <time.h>

void random_move_monster(struct map *map, struct monster_node *monster, struct player *player) {
    assert(map);
    assert(monster);
    assert(player);

    timer_update(monster_node_get_timer(monster));

    if (timer_get_state(monster_node_get_timer(monster)) == IS_OVER) {

        static int i = 0;

        srand(time(NULL) + i++);

        int visited_directions[NUM_DIRECTIONS] = {0, 0, 0, 0};
        while (visited_directions[NORTH] != 1 || visited_directions[SOUTH] != 1 || visited_directions[EAST] != 1 || visited_directions[WEST] != 1) {
            enum direction directions[NUM_DIRECTIONS] = {NORTH, SOUTH, EAST, WEST};
            enum direction direction = directions[rand() % NUM_DIRECTIONS];

            if (visited_directions[direction] != 1) {
                if (map_can_monster_move(map, player, monster, direction)) {
                    if (map_will_monster_meet_player(monster, player, direction)) {
                        map_monster_meeting_player(monster, player, direction);
                        break;
                    }

                    monster_node_move(monster, direction);
                    break;
                }

                visited_directions[direction] = 1;
            }
        }
    }
}

void random_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    for (struct monster_node *current = map_get_monster_head(map); current != NULL; current = monster_node_get_next(current)) {
        random_move_monster(map, current, player);
    }
}

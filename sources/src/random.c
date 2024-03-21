#include "../include/random.h"
#include "../include/constant.h"
#include "../include/monster.h"
#include <assert.h>

void random_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    struct monster **list_monsters = map_get_list_monsters(map);

    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {

            struct monster *monster = list_monsters[i];

            timer_update(monster_get_timer(monster));

            if (timer_get_state(monster_get_timer(monster)) == IS_OVER) {

                int visited_directions[4] = {0, 0, 0, 0};

                while (visited_directions[NORTH] != 1 || visited_directions[SOUTH] != 1 || visited_directions[EAST] != 1 || visited_directions[WEST] != 1) {
                    enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};
                    enum direction direction = directions[rand() % 4];

                    if (visited_directions[direction] != 1) {
                        if (map_can_monster_move(map, player, monster, direction)) {
                            if (map_will_monster_meet_player(monster, player, direction)) {
                                map_monster_meeting_player(monster, player, direction);
                                break;
                            } else {
                                monster_move(monster, direction);
                                break;
                            }
                        }

                        visited_directions[direction] = 1;
                    }
                }
            }
        }
    }
}
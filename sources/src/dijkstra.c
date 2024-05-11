#include "../include/dijkstra.h"
#include "../include/constant.h"
#include "../include/random.h"
#include <limits.h>
#include <assert.h>

struct adj_vertex {
    enum direction direction;
    struct adj_vertex *next;
};

struct vertex {
    int distance;
    int is_visited;
    struct adj_vertex *adj_vertex_head;
    enum direction parent_dir;
};

static int is_obstacle(struct map *map, int x, int y) {
    assert(map);

    if (!map_is_inside(map, x, y)) {
        return 1;
    }

    switch (map_get_cell_value(map, x, y) & 0xf0) {

        case CELL_SCENERY:
        case CELL_DOOR:
        case CELL_BOX:
        case CELL_BOMB:
            return 1;

        default:
            return 0;
    }
}

static struct adj_vertex *adj_vertex_new(enum direction direction) {
    struct adj_vertex *new_adj_vertex = malloc(sizeof(struct adj_vertex));

    assert(new_adj_vertex);

    new_adj_vertex->direction = direction;

    return new_adj_vertex;
}

static void adj_vertex_free(struct adj_vertex *to_free) {
    assert(to_free);

    free(to_free);
}

static void vertex_add_adj_vertex(struct vertex *vertex, struct adj_vertex *to_add) {
    assert(vertex);
    assert(to_add);

    to_add->next = vertex->adj_vertex_head;
    vertex->adj_vertex_head = to_add;
}

static struct vertex vertex_new(struct map *map, int x, int y) {
    struct vertex vertex;
    vertex.distance = INT_MAX;
    vertex.is_visited = 0;
    vertex.parent_dir = NONE;
    vertex.adj_vertex_head = NULL;

    if (is_obstacle(map, x, y)) {
        return vertex;
    }

    enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};

    struct monster_node *monster = monster_node_new(x, y);

    for (int i = 0; i < 4; i++) {
        if (!is_obstacle(map, direction_get_x(directions[i], x, 1), direction_get_y(directions[i], y, 1))) {
            struct adj_vertex *adj_vertex = adj_vertex_new(directions[i]);
            vertex_add_adj_vertex(&vertex, adj_vertex);
        }
    }

    monster_node_free(monster);

    return vertex;
}

static void vertex_free(struct vertex *to_free) {
    assert(to_free);

    struct adj_vertex *current = to_free->adj_vertex_head;

    while (current != NULL) {
        struct adj_vertex *tmp = current;
        current = current->next;
        adj_vertex_free(tmp);
    }
}

static enum direction opposite_direction(enum direction dir) {
    switch (dir) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
        default:
            return NONE;
    }
}

static struct vertex *graph_new(struct map *map) {
    assert(map);

    struct vertex *graph = malloc(map_get_height(map) * map_get_width(map) * sizeof(struct vertex));

    assert(graph);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            graph[CELL(i, j)] = vertex_new(map, i, j);
        }
    }

    return graph;
}

static void graph_free(struct vertex *graph, struct map *map) {
    assert(graph);
    assert(map);

    for (int i = 0; i < map_get_width(map); i++) {
        for (int j = 0; j < map_get_height(map); j++) {
            vertex_free(&graph[CELL(i, j)]);
        }
    }

    free(graph);
}

void dijkstra_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    for (struct monster_node *current = map_get_monster_head(map); current != NULL; current = monster_node_get_next(current)) {
        timer_update(monster_node_get_timer(current));

        if (timer_get_state(monster_node_get_timer(current)) == IS_OVER) {

            struct vertex *graph = graph_new(map);

            int start_x = monster_node_get_x(current);
            int start_y = monster_node_get_y(current);
            graph[CELL(start_x, start_y)].distance = 0;

            for (int count = 0; count < map_get_width(map) * map_get_height(map); count++) {

                int min_distance = INT_MAX;
                int min_index = -1;

                for (int i = 0; i < map_get_width(map) * map_get_height(map); i++) {
                    if (!graph[i].is_visited && graph[i].distance < min_distance) {
                        min_distance = graph[i].distance;
                        min_index = i;
                    }
                }

                if (min_index == -1) {
                    break;
                }

                graph[min_index].is_visited = 1;

                if (min_index == CELL(player_get_x(player), player_get_y(player))) {
                    break;
                }

                for (struct adj_vertex *current_adj = graph[min_index].adj_vertex_head; current_adj != NULL; current_adj = current_adj->next) {
                    int direction_x = direction_get_x(current_adj->direction, min_index % map_get_width(map), 1);
                    int direction_y = direction_get_y(current_adj->direction, min_index / map_get_width(map), 1);
                    int adjacent_index = CELL(direction_x, direction_y);

                    if (!graph[adjacent_index].is_visited && graph[min_index].distance != INT_MAX && graph[min_index].distance + 1 < graph[adjacent_index].distance) {
                        graph[adjacent_index].distance = graph[min_index].distance + 1;

                        enum direction parent_dir = opposite_direction(current_adj->direction);
                        graph[adjacent_index].parent_dir = parent_dir;
                    }
                }
            }

            int x = player_get_x(player);
            int y = player_get_y(player);
            struct vertex *v = &graph[CELL(x, y)];

            if (v->parent_dir == NONE) {
                graph_free(graph, map);

                random_move_monster(map, current, player);

                continue;
            }

            while (direction_get_x(v->parent_dir, x, 1) != monster_node_get_x(current) || direction_get_y(v->parent_dir, y, 1) != monster_node_get_y(current)) {
                x = direction_get_x(v->parent_dir, x, 1);
                y = direction_get_y(v->parent_dir, y, 1);
                v = &graph[CELL(x, y)];
            }

            enum direction next_dir = opposite_direction(v->parent_dir);

            if (map_can_monster_move(map, player, current, next_dir)) {
                if (map_will_monster_meet_player(current, player, next_dir)) {
                    map_monster_meeting_player(current, player, next_dir);
                } else {
                    monster_node_move(current, next_dir);
                }
            }

            graph_free(graph, map);
        }
    }
}
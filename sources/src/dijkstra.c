#include "../include/dijkstra.h"
#include "../include/random.h"
#include <limits.h>
#include <assert.h>

#define VERTEX(i, j) ((i) + (j) * graph->width)

struct adj_vertex_node {
    int x;
    int y;
    struct adj_vertex_node *next;
};

struct vertex {
    int x;
    int y;
    int distance;
    int is_visited;
    struct adj_vertex_node *adj_vertex_head;
    int x_prev;
    int y_prev;
};

struct graph {
    int width;
    int height;
    struct vertex *list_vertices;
    int x_start;
    int y_start;
    int x_dest;
    int y_dest;
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

static struct adj_vertex_node *adj_vertex_node_new(int x, int y) {
    struct adj_vertex_node *new_adj_vertex = malloc(sizeof(struct adj_vertex_node));

    assert(new_adj_vertex);

    new_adj_vertex->x = x;
    new_adj_vertex->y = y;
    new_adj_vertex->next = NULL;

    return new_adj_vertex;
}

static void adj_vertex_node_free(struct adj_vertex_node *to_free) {
    assert(to_free);
    free(to_free);
}

static void vertex_add_adj_vertex_node(struct vertex *vertex, struct adj_vertex_node *to_add) {
    assert(vertex);
    assert(to_add);

    to_add->next = vertex->adj_vertex_head;
    vertex->adj_vertex_head = to_add;
}

static void vertex_remove_adj_vertex_node(struct vertex *vertex, struct adj_vertex_node *to_remove) {
    assert(vertex);
    assert(to_remove);

    if (vertex->adj_vertex_head == to_remove) {
        vertex->adj_vertex_head = to_remove->next;
        adj_vertex_node_free(to_remove);

        return;
    }

    for (struct adj_vertex_node *current = vertex->adj_vertex_head; current != NULL; current = current->next) {

        if (current->next == to_remove) {
            current->next = to_remove->next;
            adj_vertex_node_free(to_remove);

            return;
        }
    }
}

static struct vertex vertex_new(struct map *map, int x, int y) {
    assert(map);

    struct vertex vertex;
    vertex.x = x;
    vertex.y = y;
    vertex.distance = INT_MAX;
    vertex.is_visited = 0;
    vertex.x_prev = -1;
    vertex.y_prev = -1;
    vertex.adj_vertex_head = NULL;

    if (is_obstacle(map, x, y)) {
        return vertex;
    }

    enum direction directions[4] = {NORTH, SOUTH, EAST, WEST};

    struct monster_node *monster = monster_node_new(x, y);

    for (int i = 0; i < 4; i++) {
        if (!is_obstacle(map, direction_get_x(directions[i], x, 1), direction_get_y(directions[i], y, 1))) {
            struct adj_vertex_node *adj_vertex = adj_vertex_node_new(direction_get_x(directions[i], x, 1), direction_get_y(directions[i], y, 1));
            vertex_add_adj_vertex_node(&vertex, adj_vertex);
        }
    }

    monster_node_free(monster);

    return vertex;
}

static void vertex_free(struct vertex *to_free) {
    assert(to_free);

    struct adj_vertex_node *current = to_free->adj_vertex_head;

    while (current != NULL) {
        struct adj_vertex_node *next = current->next;
        vertex_remove_adj_vertex_node(to_free, current);
        current = next;
    }
}

static struct vertex *graph_get_vertex(struct graph *graph, int x, int y) {
    assert(graph);

    return &graph->list_vertices[VERTEX(x, y)];
}

static struct graph *graph_new(struct map *map, struct monster_node *monster, struct player *player) {
    assert(map);
    assert(monster);
    assert(player);

    struct graph *graph = malloc(sizeof(struct graph));

    assert(graph);

    graph->width = map_get_width(map);
    graph->height = map_get_height(map);

    graph->list_vertices = malloc(graph->width * graph->height * sizeof(struct vertex));

    assert(graph->list_vertices);

    for (int i = 0; i < graph->width; i++) {
        for (int j = 0; j < graph->height; j++) {
            graph->list_vertices[VERTEX(i, j)] = vertex_new(map, i, j);
        }
    }

    graph->x_start = monster_node_get_x(monster);
    graph->y_start = monster_node_get_y(monster);

    graph->x_dest = player_get_x(player);
    graph->y_dest = player_get_y(player);

    return graph;
}

static void graph_free(struct graph *graph) {
    assert(graph);

    for (int i = 0; i < graph->width; i++) {
        for (int j = 0; j < graph->height; j++) {
            vertex_free(graph_get_vertex(graph, i, j));
        }
    }

    free(graph->list_vertices);

    free(graph);
}

void dijkstra_update_monsters(struct map *map, struct player *player) {
    assert(map);
    assert(player);

    for (struct monster_node *current = map_get_monster_head(map); current != NULL; current = monster_node_get_next(current)) {
        timer_update(monster_node_get_timer(current));

        if (timer_get_state(monster_node_get_timer(current)) == IS_OVER) {

            struct graph *graph = graph_new(map, current, player);

            graph_get_vertex(graph, graph->x_start, graph->y_start)->distance = 0;

            for (int count = 0; count < graph->width * graph->height; count++) {

                int min_distance = INT_MAX;
                int x_min = -1;
                int y_min = -1;

                for (int i = 0; i < graph->width; i++) {
                    for (int j = 0; j < graph->height; j++) {
                        if (!graph_get_vertex(graph, i, j)->is_visited && graph_get_vertex(graph, i, j)->distance < min_distance) {
                            min_distance = graph_get_vertex(graph, i, j)->distance;
                            x_min = i;
                            y_min = j;
                        }
                    }
                }

                if (x_min == -1 || y_min == -1) {
                    break;
                }

                graph_get_vertex(graph, x_min, y_min)->is_visited = 1;

                if (x_min == graph->x_dest && y_min == graph->y_dest) {
                    break;
                }

                for (struct adj_vertex_node *current_adj = graph_get_vertex(graph, x_min, y_min)->adj_vertex_head; current_adj != NULL; current_adj = current_adj->next) {

                    if (!graph_get_vertex(graph, current_adj->x, current_adj->y)->is_visited && graph_get_vertex(graph, x_min, y_min)->distance != INT_MAX && graph_get_vertex(graph, x_min, y_min)->distance + 1 < graph_get_vertex(graph, current_adj->x, current_adj->y)->distance) {

                        graph_get_vertex(graph, current_adj->x, current_adj->y)->distance = graph_get_vertex(graph, x_min, y_min)->distance + 1;
                        graph_get_vertex(graph, current_adj->x, current_adj->y)->x_prev = x_min;
                        graph_get_vertex(graph, current_adj->x, current_adj->y)->y_prev = y_min;
                    }
                }
            }

            struct vertex *v = graph_get_vertex(graph, graph->x_dest, graph->y_dest);

            if (v->x_prev == -1 || v->y_prev == -1) {
                graph_free(graph);

                random_move_monster(map, current, player);

                continue;
            }

            while (v->x_prev != monster_node_get_x(current) || v->y_prev != monster_node_get_y(current)) {
                v = graph_get_vertex(graph, v->x_prev, v->y_prev);
            }

            enum direction next_dir = direction_get_from_coordinates(graph->x_start, graph->y_start, v->x, v->y);

            if (map_can_monster_move(map, player, current, next_dir)) {
                if (map_will_monster_meet_player(current, player, next_dir)) {
                    map_monster_meeting_player(current, player, next_dir);
                } else {
                    monster_node_move(current, next_dir);
                }
            }

            graph_free(graph);
        }
    }
}
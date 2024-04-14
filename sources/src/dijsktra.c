#include "../include/dijsktra.h"
#include <stdbool.h>
#include <limits.h>

#define ROW 12
#define COL 12

// Structure pour représenter un nœud dans le graphe
struct Node {
    int x, y;   // Coordonnées du nœud dans la grille
    int distance;  // Distance minimale depuis le nœud source
    bool visited;  // Indique si le nœud a été visité
    struct Node *parent; // Nœud parent dans le chemin le plus court
};

// Fonction pour initialiser un nœud
struct Node createNode(int x, int y) {
    struct Node node;
    node.x = x;
    node.y = y;
    node.distance = INT_MAX;  // Initialisation à l'infini
    node.visited = false;
    node.parent = NULL;
    return node;
}

// Fonction pour vérifier si un nœud est dans les limites de la grille
bool isValid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

// Fonction pour trouver le nœud avec la distance minimale non visitée
struct Node *minDistance(struct Node grid[][COL]) {
    int minDist = INT_MAX;
    struct Node *minNode = NULL;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (!grid[i][j].visited && grid[i][j].distance <= minDist) {
                minDist = grid[i][j].distance;
                minNode = &grid[i][j];
            }
        }
    }

    return minNode;
}

// Fonction pour trouver le chemin le plus court du nœud source au nœud destination

/*
struct Node *dijkstra(struct Node grid[][COL], int srcX, int srcY, int destX, int destY) {
    // Initialisation des distances
    grid[srcX][srcY].distance = 0;

    // Exploration de tous les nœuds
    for (int count = 0; count < ROW * COL - 1; count++) {
        struct Node *minNode = minDistance(grid);
        if (minNode == NULL || (minNode->x == destX && minNode->y == destY))
            break;

        minNode->visited = true;

        // Mise à jour des distances des nœuds adjacents non visités
        // et mise à jour des nœuds parent
        int rowMoves[] = {-1, 0, 0, 1};
        int colMoves[] = {0, -1, 1, 0};
        for (int i = 0; i < 4; i++) {
            int newRow = minNode->x + rowMoves[i];
            int newCol = minNode->y + colMoves[i];

            // Vérification de validité et non-visitée
            if (isValid(newRow, newCol) && !grid[newRow][newCol].visited && grid[newRow][newCol].distance > minNode->distance + 1) {
                grid[newRow][newCol].distance = minNode->distance + 1;
                grid[newRow][newCol].parent = minNode;
            }
        }
    }

    // Reconstruction du chemin à partir du nœud destination
    struct Node *current = &grid[destX][destY];

    printf("Chemin le plus court :\n");
    while (current != NULL) {
        printf("(%d,%d)\n", current->x, current->y);
        current = current->parent;
    }

    return current;
}

void dijsktra_update_monsters(struct map *map, struct player *player) {

    struct monster_node **list_monsters = map_get_monster_head(map);


    for (int i = 0; i < NUM_MONSTERS_MAX; i++) {
        if (list_monsters[i] != NULL) {
            timer_update(monster_node_get_timer(list_monsters[i]));
            if (timer_get_state(monster_node_get_timer(list_monsters[i])) == IS_OVER){
                struct Node grid[map_get_width(map)][map_get_height(map)];

                for (int j = 0; j < map_get_height(map); j++) {
                    for (int k = 0; k < map_get_width(map); k++) {
                        grid[j][k] = createNode(j, k);

                        switch (map_get_cell_value(map, j, k) & 0xf0) {

                            case CELL_SCENERY:
                            case CELL_BOMB:
                            case CELL_DOOR:
                            case CELL_BOX:
                                grid[j][k].visited = true;
                                break;

                            default:
                                break;
                        }
                    }
                }

                struct monster_node *monster_head = list_monsters[i];

                struct Node *path = dijkstra(grid, monster_node_get_x(monster_head), monster_node_get_y(monster_head), player_get_x(player), player_get_y(player));

                while (path->parent->parent != NULL) {
                    path = path->parent;
                }

                if (path->x - monster_node_get_x(monster_head) == 1 && path->y - monster_node_get_y(monster_head) == 0) {
                    if (map_will_monster_meet_player(monster_head, player, EAST)) {
                        map_monster_meeting_player(monster_head, player, EAST);
                    } else {
                        monster_node_move(monster_head, EAST);
                    }
                } else if (path->x - monster_node_get_x(monster_head) == -1 && path->y - monster_node_get_y(monster_head) == 0) {
                    if (map_will_monster_meet_player(monster_head, player, WEST)) {
                        map_monster_meeting_player(monster_head, player, WEST);
                    } else {
                        monster_node_move(monster_head, WEST);
                    }
                } else if (path->x - monster_node_get_x(monster_head) == 0 && path->y - monster_node_get_y(monster_head) == 1) {
                    if (map_will_monster_meet_player(monster_head, player, SOUTH)) {
                        map_monster_meeting_player(monster_head, player, SOUTH);
                    } else {
                        monster_node_move(monster_head, SOUTH);
                    }
                } else if (path->x - monster_node_get_x(monster_head) == 0 && path->y - monster_node_get_y(monster_head) == -1) {
                    if (map_will_monster_meet_player(monster_head, player, NORTH)) {
                        map_monster_meeting_player(monster_head, player, NORTH);
                    } else {
                        monster_node_move(monster_head, NORTH);
                    }
                }

                int test = 0;
            }
        }
    }
}

 */
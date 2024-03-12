#include "../include/player.h"
#include "../include/misc.h"
#include "../include/window.h"
#include "../include/constant.h"
#include "../include/timer.h"
#include <assert.h>
#include <stdlib.h>

/**
 * @brief Structure representing the player.
 */
struct player {
    int x; /**< X position of the player */
    int y; /**< Y position of the player */
    enum direction direction; /**< Direction of the player */
    int num_bombs; /**< Number of bombs of the player */
    int range_bombs; /**< Range of the player's bombs */
    int num_lives; /**< Number of lives of the player */
    int num_keys; /**< Number of keys of the player */
    struct timer *timer_invincibility; /**< Invincibility timer of the player */
};

struct player *player_new(int num_bombs) {
    assert(num_bombs >= 0 && num_bombs <= NUM_BOMBS_MAX);

    struct player *player = malloc(sizeof(*player));

    if (!player) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(player, 0, sizeof(*player));

    player->direction = NORTH;
    player->num_bombs = num_bombs;
    player->range_bombs = 1;
    player->num_lives = 3;
    player->num_keys = 0;
    player->timer_invincibility = timer_new();
    player->x = 1;
    player->y = 0;

    return player;
}

void player_free(struct player *player) {
    assert(player);
    assert(player->timer_invincibility);

    timer_free(player->timer_invincibility);
    free(player);
}

void player_write(struct player *player, FILE *file) {
    assert(player);
    assert(file);

    fwrite(player, sizeof(*player), 1, file);
    timer_write(player->timer_invincibility, file);
}

void player_read(struct player *player, FILE *file) {
    assert(player);
    assert(file);

    struct timer *timer_invicibility = player->timer_invincibility;

    fread(player, sizeof(*player), 1, file);
    player->timer_invincibility = timer_invicibility;
    timer_read(player->timer_invincibility, file);
}

enum direction player_get_direction(struct player *player) {
    assert(player);
    return player->direction;
}

struct timer *player_get_timer_invincibility(struct player *player) {
    assert(player);
    return player->timer_invincibility;
}

void player_set_timer_invincibility(struct player *player, struct timer *timer_invincibility) {
    assert(player);
    assert(timer_invincibility);
    player->timer_invincibility = timer_invincibility;
}

int player_get_x(struct player *player) {
    assert(player);
    return player->x;
}

int player_get_y(struct player *player) {
    assert(player);
    return player->y;
}

void player_set_x(struct player *player, int x) {
    assert(player);
    player->x = x;
}

void player_set_y(struct player *player, int y) {
    assert(player);
    player->y = y;
}

void player_set_direction(struct player *player, enum direction way) {
    assert(player);
    player->direction = way;
}

int player_get_num_bomb(struct player *player) {
    assert(player);
    return player->num_bombs;
}

void player_inc_num_bomb(struct player *player) {
    assert(player);

    if (player->num_bombs < NUM_BOMBS_MAX) {
        player->num_bombs++;
    }
}

void player_dec_num_bomb(struct player *player) {
    assert(player);

    if (player->num_bombs > 0) {
        player->num_bombs--;
    }
}

void player_set_num_bombs(struct player *player, int num_bombs) {
    assert(player);

    if (num_bombs <= NUM_BOMBS_MAX && num_bombs >= 0) {
        player->num_bombs = num_bombs;
    }
}

int player_get_num_lives(struct player *player) {
    assert(player);
    return player->num_lives;
}

void player_dec_num_lives(struct player *player) {
    assert(player);

    timer_update(player->timer_invincibility);

    if (player->num_lives > 0 && timer_get_state(player->timer_invincibility) == IS_OVER) {
        player->num_lives--;
        timer_start(player->timer_invincibility, DURATION_PLAYER_INVINCIBILITY);
    }
}

void player_inc_num_lives(struct player *player) {
    assert(player);

    if (player->num_lives < NUM_LIVES_MAX) {
        player->num_lives++;
    }
}

int player_get_range_bombs(struct player *player) {
    assert(player);
    return player->range_bombs;
}

void player_inc_range_bombs(struct player *player) {
    assert(player);

    if (player->range_bombs < NUM_BOMBS_MAX) {
        player->range_bombs++;
    }
}

void player_dec_range_bombs(struct player *player) {
    assert(player);

    if (player->range_bombs > 1) {
        player->range_bombs--;
    }
}

int player_get_num_keys(struct player *player) {
    assert(player);
    return player->num_keys;
}

void player_inc_num_keys(struct player *player) {
    assert(player);

    if (player->num_keys < NUM_KEYS_MAX) {
        player->num_keys++;
    }
}

void player_dec_num_keys(struct player *player) {
    assert(player);

    if (player->num_keys > 0) {
        player->num_keys--;
    }
}

void player_display(struct player *player, SDL_Surface *window, struct sprites *sprites) {
    assert(player);
    assert(window);

    window_display_image(window, sprites_get_player(sprites, player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

void player_get_bonus(struct player *player, enum bonus_type bonus_type) {
    assert(player);

    if (bonus_type == BONUS_BOMB_RANGE_INC) {
        player_inc_range_bombs(player);

    } else if (bonus_type == BONUS_BOMB_RANGE_DEC) {
        player_dec_range_bombs(player);

    } else if (bonus_type == BONUS_BOMB_NB_DEC) {
        player_dec_num_bomb(player);

    } else if (bonus_type == BONUS_BOMB_NB_INC) {
        player_inc_num_bomb(player);

    } else if (bonus_type == BONUS_LIFE) {
        player_inc_num_lives(player);
    }
}

void player_move(struct player *player, enum direction direction) {
    assert(player);

    player_set_direction(player, direction);

    player_set_x(player, direction_get_x(direction, player_get_x(player), 1));
    player_set_y(player, direction_get_y(direction, player_get_y(player), 1));
}


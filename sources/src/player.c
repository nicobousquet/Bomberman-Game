#include "../include/player.h"
#include "../include/sprite.h"
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
    int bombs_range; /**< Range of the player's bombs */
    int num_lives; /**< Number of lives of the player */
    int num_keys; /**< Number of keys of the player */
    struct timer *timer_invincibility; /**< Invincibility timer of the player */
};

struct player *player_init(int bombs) {
    assert(bombs >= 0 && bombs <= NUM_BOMBS_MAX);
    struct player *player = malloc(sizeof(struct player));
    if (!player) {
        error("Memory error");
    }

    memset(player, 0, sizeof(struct player));
    player->direction = NORTH;
    player->num_bombs = bombs;
    player->bombs_range = 1;
    player->num_lives = 3;
    player->num_keys = 0;
    player->timer_invincibility = timer_init(DURATION_PLAYER_INVINCIBILITY);
    player->x = 1;
    player->y = 0;
    return player;
}

void player_free(struct player *player) {
    assert(player);
    assert(player->timer_invincibility);
    free(player->timer_invincibility);
    free(player);
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

void player_set_current_way(struct player *player, enum direction way) {
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
    if (player->num_lives > 0) {
        player->num_lives--;
    }
}

void player_inc_num_lives(struct player *player) {
    assert(player);
    if (player->num_lives < NUM_LIVES_MAX) {
        player->num_lives++;
    }
}

int player_get_size() {
    return sizeof(struct player);
}

int player_get_bombs_range(struct player *player) {
    assert(player);
    return player->bombs_range;
}

void player_inc_bombs_range(struct player *player) {
    assert(player);
    if (player->bombs_range < NUM_BOMBS_MAX) {
        player->bombs_range++;
    }
}

void player_dec_bombs_range(struct player *player) {
    assert(player);
    if (player->bombs_range > 1) {
        player->bombs_range--;
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

/* displaying player */
void player_display(struct player *player) {
    assert(player);
    window_display_image(sprite_get_player(player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

void player_get_bonus(struct player *player, enum bonus_type bonus_type) {
    assert(player);

    if (bonus_type == BONUS_BOMB_RANGE_INC) {
        player_inc_bombs_range(player);
    } else if (bonus_type == BONUS_BOMB_RANGE_DEC) {
        player_dec_bombs_range(player);
    } else if (bonus_type == BONUS_BOMB_NB_DEC) {
        player_dec_num_bomb(player);
    } else if (bonus_type == BONUS_BOMB_NB_INC) {
        player_inc_num_bomb(player);
    } else if (bonus_type == BONUS_LIFE) {
        player_inc_num_lives(player);
    }
}


#include "../include/sprites.h"
#include "../include/misc.h"
#include <assert.h>

#define MAP_CASE           "sprite/wood_box.png"
#define MAP_KEY            "sprite/key.png"
#define MAP_DOOR_OPEN    "sprite/door_open.png"
#define MAP_DOOR_CLOSE    "sprite/door_close.png"

#define MAP_STONE          "sprite/stone.png"
#define MAP_TREE           "sprite/tree.png"

#define BANNER_LINE        "sprite/banner_line.png"
#define BANNER_LINE_VERT   "sprite/banner_line_vert.png"
#define BANNER_LIFE        "sprite/banner_life.png"
#define BANNER_BOMB        "sprite/bomb3.png"
#define BANNER_RANGE       "sprite/banner_range.png"
#define DIGIT_0            "sprite/banner_0.jpg"
#define DIGIT_1            "sprite/banner_1.jpg"
#define DIGIT_2            "sprite/banner_2.jpg"
#define DIGIT_3            "sprite/banner_3.jpg"
#define DIGIT_4            "sprite/banner_4.jpg"
#define DIGIT_5            "sprite/banner_5.jpg"
#define DIGIT_6            "sprite/banner_6.jpg"
#define DIGIT_7            "sprite/banner_7.jpg"
#define DIGIT_8            "sprite/banner_8.jpg"
#define DIGIT_9            "sprite/banner_9.jpg"

#define BOMB_TTL1           "sprite/bomb1.png"
#define BOMB_TTL2           "sprite/bomb2.png"
#define BOMB_TTL3           "sprite/bomb3.png"
#define BOMB_TTL4           "sprite/bomb4.png"
#define BOMB_EXPLOSION      "sprite/explosion.png"

#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_num_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_num_dec.png"
#define IMG_BONUS_LIFE            "sprite/banner_life.png"

#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

#define PRINCESS        "sprite/bomberwoman.png"

#define MONSTER_LEFT    "sprite/monster_left.png"
#define MONSTER_UP      "sprite/monster_up.png"
#define MONSTER_RIGHT   "sprite/monster_right.png"
#define MONSTER_DOWN    "sprite/monster_down.png"

struct sprites {
    SDL_Surface *bomb_img[5];
    SDL_Surface *numbers[10];
    SDL_Surface *banner_life;
    SDL_Surface *banner_bomb;
    SDL_Surface *banner_range;
    SDL_Surface *banner_line;
    SDL_Surface *banner_line_vert;
    SDL_Surface *box;
    SDL_Surface *key;
    SDL_Surface *door_open;
    SDL_Surface *door_close;
    SDL_Surface *stone;
    SDL_Surface *tree;
    SDL_Surface *bonus[6];
    SDL_Surface *player_img[4];
    SDL_Surface *princess;
    SDL_Surface *monster_img[4];
};

int sprites_get_size() {
    return sizeof(struct sprites);
}

static void bomb_load(struct sprites *sprites) {
    sprites->bomb_img[EXPLOSION - 1] = image_load(BOMB_EXPLOSION);
    sprites->bomb_img[TTL1 - 1] = image_load(BOMB_TTL1);
    sprites->bomb_img[TTL2 - 1] = image_load(BOMB_TTL2);
    sprites->bomb_img[TTL3 - 1] = image_load(BOMB_TTL3);
    sprites->bomb_img[TTL4 - 1] = image_load(BOMB_TTL4);
}

static void bomb_unload(struct sprites *sprites) {
    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(sprites->bomb_img[i]);
    }
}

static void banner_load(struct sprites *sprites) {
    sprites->numbers[0] = image_load(DIGIT_0);
    sprites->numbers[1] = image_load(DIGIT_1);
    sprites->numbers[2] = image_load(DIGIT_2);
    sprites->numbers[3] = image_load(DIGIT_3);
    sprites->numbers[4] = image_load(DIGIT_4);
    sprites->numbers[5] = image_load(DIGIT_5);
    sprites->numbers[6] = image_load(DIGIT_6);
    sprites->numbers[7] = image_load(DIGIT_7);
    sprites->numbers[8] = image_load(DIGIT_8);
    sprites->numbers[9] = image_load(DIGIT_9);

    sprites->banner_life = image_load(BANNER_LIFE);
    sprites->banner_bomb = image_load(BANNER_BOMB);
    sprites->banner_range = image_load(BANNER_RANGE);
    sprites->banner_line = image_load(BANNER_LINE);
    sprites->banner_line_vert = image_load(BANNER_LINE_VERT);
}

static void banner_unload(struct sprites *sprites) {
    for (int i = 0; i < 10; i++) {
        SDL_FreeSurface(sprites->numbers[i]);
    }

    SDL_FreeSurface(sprites->banner_line);
    SDL_FreeSurface(sprites->banner_line_vert);
    SDL_FreeSurface(sprites->banner_bomb);
    SDL_FreeSurface(sprites->banner_range);
    SDL_FreeSurface(sprites->banner_life);
}

static void map_load(struct sprites *sprites) {
    sprites->tree = image_load(MAP_TREE);
    sprites->box = image_load(MAP_CASE);
    sprites->key = image_load(MAP_KEY);
    sprites->stone = image_load(MAP_STONE);
    sprites->door_open = image_load(MAP_DOOR_OPEN);
    sprites->door_close = image_load(MAP_DOOR_CLOSE);
    sprites->princess = image_load(PRINCESS);
}

static void map_unload(struct sprites *sprites) {
    SDL_FreeSurface(sprites->tree);
    SDL_FreeSurface(sprites->box);
    SDL_FreeSurface(sprites->key);
    SDL_FreeSurface(sprites->stone);
    SDL_FreeSurface(sprites->door_open);
    SDL_FreeSurface(sprites->door_close);
    SDL_FreeSurface(sprites->princess);
}

static void bonus_load(struct sprites *sprites) {
    sprites->bonus[0] = NULL;
    sprites->bonus[BONUS_BOMB_RANGE_INC] = image_load(IMG_BONUS_BOMB_RANGE_INC);
    sprites->bonus[BONUS_BOMB_RANGE_DEC] = image_load(IMG_BONUS_BOMB_RANGE_DEC);
    sprites->bonus[BONUS_BOMB_NB_INC] = image_load(IMG_BONUS_BOMB_NB_INC);
    sprites->bonus[BONUS_BOMB_NB_DEC] = image_load(IMG_BONUS_BOMB_NB_DEC);
    sprites->bonus[BONUS_LIFE] = image_load(IMG_BONUS_LIFE);
}

static void bonus_unload(struct sprites *sprites) {
    for (int i = 0; i < 6; i++) {
        if (sprites->bonus[i]) {
            SDL_FreeSurface(sprites->bonus[i]);
        }
    }
}

static void player_load(struct sprites *sprites) {
    sprites->player_img[WEST] = image_load(PLAYER_LEFT);
    sprites->player_img[EAST] = image_load(PLAYER_RIGHT);
    sprites->player_img[NORTH] = image_load(PLAYER_UP);
    sprites->player_img[SOUTH] = image_load(PLAYER_DOWN);
}


static void player_unload(struct sprites *sprites) {
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(sprites->player_img[i]);
    }
}

static void monster_load(struct sprites *sprites) {
    sprites->monster_img[WEST] = image_load(MONSTER_LEFT);
    sprites->monster_img[EAST] = image_load(MONSTER_RIGHT);
    sprites->monster_img[NORTH] = image_load(MONSTER_UP);
    sprites->monster_img[SOUTH] = image_load(MONSTER_DOWN);
}

static void monster_unload(struct sprites *sprites) {
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(sprites->monster_img[i]);
    }
}

void sprites_load(struct sprites *sprites) {
    map_load(sprites);
    bonus_load(sprites);
    banner_load(sprites);
    player_load(sprites);
    bomb_load(sprites);
    monster_load(sprites);
}

void sprites_free(struct sprites *sprites) {
    map_unload(sprites);
    bonus_unload(sprites);
    banner_unload(sprites);
    player_unload(sprites);
    bomb_unload(sprites);
    monster_unload(sprites);
}

SDL_Surface *sprites_get_number(struct sprites *sprites, short number) {
    assert(number >= 0 && number <= 9);
    return sprites->numbers[number];
}

SDL_Surface *sprites_get_player(struct sprites *sprites, enum direction direction) {
    assert(sprites->player_img[direction]);
    return sprites->player_img[direction];
}

SDL_Surface *sprites_get_monster(struct sprites *sprites, enum direction direction) {
    assert(sprites->monster_img[direction]);
    return sprites->monster_img[direction];
}

SDL_Surface *sprites_get_banner_life(struct sprites *sprites) {
    assert(sprites->banner_life);
    return sprites->banner_life;
}

SDL_Surface *sprites_get_banner_bomb(struct sprites *sprites) {
    assert(sprites->banner_bomb);
    return sprites->banner_bomb;
}

SDL_Surface *sprites_get_banner_line(struct sprites *sprites) {
    assert(sprites->banner_line);
    return sprites->banner_line;
}

SDL_Surface *sprites_get_banner_vertical_line(struct sprites *sprites) {
    assert(sprites->banner_line_vert);
    return sprites->banner_line_vert;
}

SDL_Surface *sprites_get_banner_range(struct sprites *sprites) {
    assert(sprites->banner_range);
    return sprites->banner_range;
}

SDL_Surface *sprites_get_bonus(struct sprites *sprites, enum bonus_type bonus_type) {
    assert(sprites->bonus[bonus_type]);
    return sprites->bonus[bonus_type];
}

SDL_Surface *sprites_get_box(struct sprites *sprites) {
    assert(sprites->box);
    return sprites->box;
}

SDL_Surface *sprites_get_key(struct sprites *sprites) {
    assert(sprites->key);
    return sprites->key;
}

SDL_Surface *sprites_get_scenery(struct sprites *sprites, enum scenery_type type) {
    if (type == SCENERY_STONE) {
        assert(sprites->stone);
        return sprites->stone;
    } else if (type == SCENERY_TREE) {
        assert(sprites->tree);
        return sprites->tree;
    } else if (type == SCENERY_PRINCESS) {
        assert(sprites->princess);
        return sprites->princess;
    }
    return NULL;
}

SDL_Surface *sprites_get_door(struct sprites *sprites, enum door_status status) {
    if (status == OPEN) {
        assert(sprites->door_open);
        return sprites->door_open;
    } else if (status == CLOSE) {
        assert(sprites->door_close);
        return sprites->door_close;
    }
    return NULL;
}

SDL_Surface *sprites_get_bomb(struct sprites *sprites, int ttl) {
    assert(sprites->bomb_img[ttl - 1]);
    return sprites->bomb_img[ttl - 1];
}

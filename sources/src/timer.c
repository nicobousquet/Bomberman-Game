#include "../include/timer.h"
#include <SDL/SDL.h>
#include <string.h>
#include <assert.h>

/**
 * @brief Structure representing a timer.
 */
struct timer {
    int duration;          /**< The duration of the timer. */
    enum state_timer state;/**< The state of the timer. */
    long start_time;       /**< The start time of the timer. */
    int remaining;         /**< The remaining time of the timer. */
};

int timer_get_size() {
    return sizeof(struct timer);
}

struct timer *timer_new() {
    struct timer *timer = malloc(sizeof(struct timer));
    if (!timer) {
        perror("malloc");
    }
    memset(timer, 0, sizeof(struct timer));
    timer->state = IS_OVER;
    return timer;
}

void timer_free(struct timer *timer) {
    assert(timer);
    free(timer);
}

void timer_write(struct timer *timer, FILE *file) {
    assert(timer);
    assert(file);
    fwrite(timer, sizeof(struct timer), 1, file);
}

void timer_read(struct timer *timer, FILE *file) {
    assert(timer);
    assert(file);
    fread(timer, sizeof(struct timer), 1, file);
    timer->start_time = SDL_GetTicks() - (timer->duration - timer->remaining);
}

void timer_start(struct timer *timer, int duration) {
    assert(timer);
    timer->duration = duration;
    timer->start_time = SDL_GetTicks();
    timer->state = RUNNING;
    timer->remaining = timer->duration;
}

void timer_update(struct timer *timer) {
    assert(timer);
    if ((timer->remaining = timer->duration - (SDL_GetTicks() - timer->start_time)) < 0) {
        timer->state = IS_OVER;
    }
}

int timer_get_remaining(struct timer *timer) {
    assert(timer);
    return timer->remaining;
}

int timer_get_duration(struct timer *timer) {
    assert(timer);
    return timer->duration;
}

void timer_set_start_time(struct timer *timer, long start_time) {
    assert(timer);
    timer->start_time = start_time;
}

enum state_timer timer_get_state(struct timer *timer) {
    assert(timer);
    return timer->state;
}

void timer_set_state(struct timer *timer, enum state_timer state) {
    assert(timer);
    timer->state = state;
}





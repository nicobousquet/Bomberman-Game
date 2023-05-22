#include "../include/timer.h"
#include <SDL/SDL.h>
#include <string.h>

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

struct timer *timer_init() {
    struct timer *timer = malloc(sizeof(struct timer));
    if (!timer) {
        perror("malloc");
    }
    memset(timer, 0, sizeof(struct timer));
    timer->state = IS_OVER;
    return timer;
}

void timer_start(struct timer *timer, int duration) {
    timer->duration = duration;
    timer->start_time = SDL_GetTicks();
    timer->state = RUNNING;
    timer->remaining = timer->duration;
}

void timer_update(struct timer *timer) {
    if ((timer->remaining = timer->duration - (SDL_GetTicks() - timer->start_time)) < 0) {
        timer->state = IS_OVER;
    }
}

int timer_get_remaining(struct timer *timer) {
    return timer->remaining;
}

int timer_get_duration(struct timer *timer) {
    return timer->duration;
}

void timer_set_start_time(struct timer *timer, long start_time) {
    timer->start_time = start_time;
}

enum state_timer timer_get_state(struct timer *timer) {
    return timer->state;
}

void timer_set_state(struct timer *timer, enum state_timer state) {
    timer->state = state;
}





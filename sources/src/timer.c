#include "../include/timer.h"
#include <SDL/SDL.h>
#include <string.h>
#include <assert.h>

/**
 * @brief Structure representing a timer.
 */
struct timer {
    int duration;          /**< The duration of the timer. */
    int is_over;           /**< The state of the timer. */
    long start_time;       /**< The start time of the timer. */
    int remaining;         /**< The remaining time of the timer. */
};

struct timer *timer_new() {
    struct timer *timer = malloc(sizeof(struct timer));

    if (!timer) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    memset(timer, 0, sizeof(struct timer));

    timer->is_over = 1;

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

struct timer *timer_read(FILE *file) {
    assert(file);

    struct timer *timer = malloc(sizeof(struct timer));

    if (!timer) {
        fprintf(stderr, "Malloc failed line %d, file %s", __LINE__, __FILE__);
        exit(EXIT_FAILURE);
    }

    fread(timer, sizeof(struct timer), 1, file);

    timer->start_time = SDL_GetTicks() - (timer->duration - timer->remaining);

    return timer;
}

void timer_start(struct timer *timer, int duration) {
    assert(timer);

    timer->duration = duration;
    timer->start_time = SDL_GetTicks();
    timer->is_over = 0;
    timer->remaining = timer->duration;
}

void timer_update(struct timer *timer) {
    assert(timer);

    if ((timer->remaining = timer->duration - (int) (SDL_GetTicks() - timer->start_time)) < 0) {
        timer->is_over = 1;
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

int timer_is_over(struct timer *timer) {
    assert(timer);
    return timer->is_over;
}

void timer_set_is_over(struct timer *timer, int is_over) {
    assert(timer);
    timer->is_over = is_over;
}

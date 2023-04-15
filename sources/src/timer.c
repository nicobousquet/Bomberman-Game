#include "../include/timer.h"
#include <SDL/SDL.h>
#include <string.h>

struct timer {
    int duration;
    int is_over;
    long start_time;
    int remaining;
};

int timer_get_size() {
    return sizeof(struct timer);
}

struct timer *timer_init(int duration) {
    struct timer *timer = malloc(sizeof(struct timer));
    if (!timer) {
        perror("malloc");
    }
    memset(timer, 0, sizeof(struct timer));
    timer->duration = duration;
    timer->start_time = 0;
    timer->is_over = 1;
    timer->remaining = duration;
    return timer;
}

void timer_start(struct timer *timer) {
    timer->start_time = SDL_GetTicks();
    timer->is_over = 0;
}

void timer_reset(struct timer *timer, int duration) {
    timer->duration = duration;
    timer->start_time = 0;
    timer->is_over = 1;
    timer->remaining = timer->duration;
}

void timer_update(struct timer *timer) {
    if ((timer->remaining = timer->duration - (SDL_GetTicks() - timer->start_time)) < 0) {
        timer->is_over = 1;
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

int timer_is_over(struct timer *timer) {
    return timer->is_over;
}

void timer_set_is_over(struct timer *timer, int is_over) {
    timer->is_over = is_over;
}





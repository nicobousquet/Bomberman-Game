#ifndef TIMER_H_
#define TIMER_H_

enum timer_state {
    RUNNING = 0,
    IS_OVER
};

struct timer;

int timer_get_size();

struct timer *timer_init();

int timer_get_duration(struct timer *timer);

int timer_get_remaining(struct timer *timer);

void timer_set_start_time(struct timer *timer, long start_time);

void timer_update(struct timer *timer);

enum timer_state timer_get_state(struct timer *timer);

void timer_start(struct timer *timer, int duration);

#endif //TIMER_H_

#ifndef SOURCES_TIMER_H
#define SOURCES_TIMER_H

struct timer;

int timer_get_size();

struct timer *timer_init(int duration);

void timer_restart(struct timer *timer);

int timer_get_duration(struct timer *timer);

int timer_get_remaining(struct timer *timer);

void timer_set_start_time(struct timer *timer, long start_time);

void timer_update(struct timer *timer);

int timer_is_over(struct timer *timer);

void timer_start(struct timer *timer);

void timer_reset(struct timer *timer, int duration);

#endif //SOURCES_TIMER_H

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>

/**
 * @brief Enumeration of timer states.
 */
enum state_timer {
    RUNNING = 0, /**< Timer is running. */
    IS_OVER /**< Timer has finished. */
};

/**
 * @brief Structure representing a timer.
 */
struct timer;

/**
 * @brief Get the size of the timer structure.
 * @return The size of the timer structure.
 */
int timer_get_size();

/**
 * @brief Initialize a timer.
 * @return A pointer to the initialized timer.
 */
struct timer *timer_new();

void timer_write(struct timer *timer, FILE *file);

void timer_read(struct timer *timer, FILE *file);

/**
 * @brief Get the duration of the timer.
 * @param timer The timer to get the duration from.
 * @return The duration of the timer in milliseconds.
 */
int timer_get_duration(struct timer *timer);

/**
 * @brief Get the remaining time of the timer.
 * @param timer The timer to get the remaining time from.
 * @return The remaining time of the timer in milliseconds.
 */
int timer_get_remaining(struct timer *timer);

/**
 * @brief Set the start time of the timer.
 * @param timer The timer to set the start time for.
 * @param start_time The start time in milliseconds.
 */
void timer_set_start_time(struct timer *timer, long start_time);

/**
 * @brief Update the timer.
 * @param timer The timer to update.
 */
void timer_update(struct timer *timer);

/**
 * @brief Get the state of the timer.
 * @param timer The timer to get the state from.
 * @return The state of the timer (RUNNING or IS_OVER).
 */
enum state_timer timer_get_state(struct timer *timer);

/**
 * @brief Start the timer with a specified duration.
 * @param timer The timer to start.
 * @param duration The duration of the timer in milliseconds.
 */
void timer_start(struct timer *timer, int duration);

#endif //TIMER_H_
#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>

/**
 * @brief Initialize a timer.
 * @return A pointer to the initialized timer.
 */
struct timer *timer_new();

/**
 * @brief Free the memory occupied by a timer.
 * @param timer A pointer to the timer to be freed.
 */
void timer_free(struct timer *timer);

/**
 * @brief Write a timer to a file.
 * @param timer The timer to write.
 * @param file The file to write the timer to.
 */
void timer_write(struct timer *timer, FILE *file);

/**
 * @brief Read a timer from a file.
 * @param file The file to read the timer from.
 * @return A pointer to the timer read.
 */
struct timer *timer_read(FILE *file);

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
 * @return The state of the timer (is_over or not).
 */
int timer_is_over(struct timer *timer);

/**
 * @brief Start the timer with a specified duration.
 * @param timer The timer to start.
 * @param duration The duration of the timer in milliseconds.
 */
void timer_start(struct timer *timer, int duration);

#endif //TIMER_H

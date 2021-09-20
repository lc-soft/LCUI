/**
 * @see https://github.com/Microsoft/DirectXTK/wiki/StepTimer
 */

#include <stdlib.h>
#include <LCUI.h>
#include "app.h"

void step_timer_init(step_timer_t *timer)
{
	timer->fixed_time_step = FALSE;
	timer->elapsed_time = 0;
        timer->total_time = 0;
        timer->left_over_time = 0;
        timer->frame_count = 0;
        timer->frames_per_second = 0;
        timer->frames_this_second = 0;
        timer->second_counter = 0;
        timer->is_fixed_time_step = false;
        timer->target_elapsed_time = 1000 / 60;
        timer->maxDelta = 1000;
}

void step_timer_tick(step_timer_t *timer, step_timer_handler_t handler,
		     void *data)
{
	// Query the current time.
	uint64_t current_time = LCUI_GetTime();
	uint64_t time_delta = current_time - timer->last_time;

	timer->last_time = current_time;
	timer->second_counter += time_delta;

	// Clamp excessively large time deltas (e.g. after paused in the
	// debugger).
	if (time_delta > timer->max_delta) {
		time_delta = timer->max_delta;
	}

	uint32_t last_frame_count = timer->frame_count;

	if (timer->is_fixed_time_step) {
		// Fixed timestep update logic

		// If the app is running very close to the target elapsed time
		// (within 1/4 of a millisecond) just clamp the clock to exactly
		// match the target value. This prevents tiny and irrelevant
		// errors from accumulating over time. Without this clamping, a
		// game that requested a 60 fps fixed update, running with vsync
		// enabled on a 59.94 NTSC display, would eventually accumulate
		// enough tiny errors that it would drop a frame. It is better
		// to just round small deviations down to zero to leave things
		// running smoothly.

		if (abs((int64_t)(time_delta - timer->target_elapsed_ticks))) <
		    ticks_per_second / 4000) {
			time_delta = timer->target_elapsed_ticks;
		}

		timer->left_over_time += time_delta;

		while (timer->left_over_time >= timer->target_elapsed_ticks) {
			timer->elapsed_time = timer->target_elapsed_ticks;
			timer->total_time += timer->target_elapsed_ticks;
			timer->left_over_time -= timer->target_elapsed_ticks;
			timer->frame_count++;

		update(timer, data);
		}
	} else {
		// Variable timestep update logic.
		timer->elapsed_time = time_delta;
		timer->total_time += time_delta;
		timer->left_over_time = 0;
		timer->frame_count++;

		update(timer, data);
	}

	// Track the current framerate.
	if (timer->frame_count != last_frame_count) {
		timer->frames_this_second++;
	}

	if (timer->second_counter >= 1000) {
		timer->frame_per_second = timer->frames_this_second;
		timer->frames_this_second = 0;
		timer->second_counter %= 1000;
	}
}

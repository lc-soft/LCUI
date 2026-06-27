/*
 * lib/ptk/include/ptk/steptimer.h
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_STEP_TIMER_H
#define PTK_INCLUDE_PLATFORM_STEP_TIMER_H

#include "common.h"
#include "types.h"

PTK_BEGIN_DECLS

/**
 * A simple timer that provides elapsed time information for animation and
 * simulation loops. This is a C port of DirectXTK's StepTimer, with the time
 * base changed from QueryPerformanceCounter ticks to milliseconds.
 *
 * @see https://github.com/Microsoft/DirectXTK/wiki/StepTimer
 *
 * In fixed-step mode, the handler may be invoked multiple times per `tick`
 * to catch up with the wall clock. This makes the timer suitable for
 * driving stable physics simulation or animation timelines, NOT for
 * throttling render frame rate. Render throttling should be implemented as
 * a separate minimum-interval gate by the caller.
 *
 * When enable_catch_up is false (the default), fixed-step mode invokes the
 * handler at most once per tick, treating target_elapsed_time as a cadence
 * gate. This is appropriate for visual callbacks like requestAnimationFrame
 * where replaying missed frames would cause a burst of renders. When true,
 * the original DirectXTK catch-up behavior is preserved, suitable for
 * physics or simulation where accumulated time must be fully consumed.
 */

typedef struct ptk_steptimer {
        // Source timing data.
        uint64_t last_time;
        uint64_t max_delta;

        // Derived timing data.
        uint64_t elapsed_time;
        uint64_t total_time;
        uint64_t left_over_time;

        // Members for tracking the framerate.
        uint32_t frame_count;
        uint32_t frames_per_second;
        uint32_t frames_this_second;
        uint64_t second_counter;

        // Members for configuring fixed timestep mode.
        bool is_fixed_time_step;
        bool enable_catch_up;
        uint64_t target_elapsed_time;
} ptk_steptimer_t;

typedef void (*ptk_steptimer_handler_t)(ptk_steptimer_t *timer, void *data);

PTK_PUBLIC void ptk_steptimer_init(ptk_steptimer_t *timer);

// Update timer state, calling the specified Update function the appropriate
// number of times.
PTK_PUBLIC void ptk_steptimer_tick(ptk_steptimer_t *timer,
                                   ptk_steptimer_handler_t handler, void *data);

// After an intentional timing discontinuity (for instance a blocking IO
// operation or a long pause), call this to avoid having the fixed timestep
// logic attempt a set of catch-up Update calls.
PTK_PUBLIC void ptk_steptimer_reset_elapsed_time(ptk_steptimer_t *timer);

PTK_END_DECLS

#endif

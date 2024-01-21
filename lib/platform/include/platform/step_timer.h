/*
 * lib/platform/include/platform/step_timer.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBPLAT_INCLUDE_PLATFORM_STEP_TIMER_H
#define LIBPLAT_INCLUDE_PLATFORM_STEP_TIMER_H

#include "common.h"
#include "types.h"

LIBPLAT_BEGIN_DECLS

typedef struct step_timer_t {
        // Source timing data.
        uint64_t frequency;
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
        uint64_t target_elapsed_time;
} step_timer_t;

typedef void (*step_timer_handler_t)(step_timer_t *timer, void *data);

LIBPLAT_PUBLIC void step_timer_init(step_timer_t *timer);

// Update timer state, calling the specified Update function the appropriate
// number of times.
LIBPLAT_PUBLIC void step_timer_tick(step_timer_t *timer,
                                    step_timer_handler_t handler, void *data);

LIBPLAT_END_DECLS

#endif

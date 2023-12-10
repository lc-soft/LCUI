/*
 * src/lcui_settings.c: -- global settings.
 *
 * Copyright (c) 2020, James Duong <duong.james@gmail.com>
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io>
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui.h>
#include <ui_server.h>
#include <LCUI/app.h>

static lcui_settings_t lcui_settings;

/* Initialize settings with the current global settings. */
void lcui_get_settings(lcui_settings_t *settings)
{
	*settings = lcui_settings;
}

/* Update global settings with the given input. */
void lcui_apply_settings(lcui_settings_t *settings)
{
	lcui_settings = *settings;
	lcui_settings.frame_rate_cap = y_max(lcui_settings.frame_rate_cap, 1);
	lcui_settings.parallel_rendering_threads =
	    y_max(lcui_settings.parallel_rendering_threads, 1);
	ui_server_set_threads(lcui_settings.parallel_rendering_threads);
	ui_server_set_paint_flashing_enabled(lcui_settings.paint_flashing);
	lcui_set_frame_rate_cap(lcui_settings.frame_rate_cap);
}

/* Reset global settings to their defaults. */
void lcui_reset_settings(void)
{
	lcui_settings_t settings = {
		.frame_rate_cap = LCUI_MAX_FRAMES_PER_SEC,
		.parallel_rendering_threads = 4,
		.paint_flashing = FALSE
	};
	lcui_apply_settings(&settings);
}

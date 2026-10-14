/*
 * include/LCUI/widgets/spinner.h: -- LCUI's Spinner widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SPINNER_H
#define LCUI_INCLUDE_WIDGETS_SPINNER_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

/*
 * A self-drawn rotating loading indicator.
 *
 * The spinner captures its `spinner-content` subtree into a reusable offscreen
 * buffer and composites it back to the window rotated around the content box
 * center. The `spinner-content` child is a real child of the spinner but is
 * styled offscreen (position: absolute; left/top: -100000px) so the normal
 * window render path culls it while the offscreen capture still records it.
 *
 * A spinner created without an explicit `spinner-content` child gets a default
 * one so it is immediately visible; callers and TSX may declare their own.
 */

/* Create a spinner widget with a default `spinner-content` subtree. */
LCUI_API ui_widget_t *ui_create_spinner(void);

/* Start or stop the rotation animation. Running is on by default. */
LCUI_API void ui_spinner_set_running(ui_widget_t *w, bool running);

/* Set the rotation speed in revolutions per second (default 1.0). */
LCUI_API void ui_spinner_set_speed(ui_widget_t *w, float revolutions_per_sec);

/* Replace the direct `spinner-content` child with the given subtree. */
LCUI_API void ui_spinner_set_content(ui_widget_t *w, ui_widget_t *content);

/* Set the rotation angle in degrees; while running the animation overrides it.
 */
LCUI_API void ui_spinner_set_angle(ui_widget_t *w, double degrees);

LCUI_API void ui_register_spinner(void);

LCUI_END_HEADER

#endif /* LCUI_INCLUDE_WIDGETS_SPINNER_H */

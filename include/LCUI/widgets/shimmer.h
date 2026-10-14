/*
 * include/LCUI/widgets/shimmer.h: -- LCUI's Shimmer widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_INCLUDE_WIDGETS_SHIMMER_H
#define LCUI_INCLUDE_WIDGETS_SHIMMER_H

#include <LCUI/common.h>
#include <ui.h>

LCUI_BEGIN_HEADER

/*
 * A self-drawn effect that sweeps a soft highlight across real content.
 *
 * The shimmer captures its `shimmer-content` subtree into a reusable offscreen
 * buffer (retaining its alpha as a mask), composites that content at full
 * strength so it keeps its original colours, then adds a tilted, gradient
 * sheen band on top. The band is masked to the content's alpha, so the
 * highlight only lands on the content's opaque pixels and fades out at its
 * left and right edges and across its tilt. `color` supplies the highlight
 * hue and its alpha the peak strength; `duration` the time of one sweep;
 * `spread` the band width; `angle` its tilt. With `running` set to false the
 * content is shown as-is, with no sheen.
 *
 * The `shimmer-content` child is a real child of the shimmer but is styled
 * offscreen (position: absolute; left/top: -100%) so the normal window render
 * path culls it while the offscreen capture still records it. A shimmer created
 * without an explicit `shimmer-content` child gets a default one so it is
 * immediately visible; callers and TSX may declare their own.
 */

/* Create a shimmer widget with a default `shimmer-content` subtree. */
LCUI_API ui_widget_t *ui_create_shimmer(void);

/* Start or stop the sweep animation. Running is on by default; when off the
 * content is composited with no sheen added. */
LCUI_API void ui_shimmer_set_running(ui_widget_t *w, bool running);

/* Set the highlight colour that is overlaid on the content's opaque pixels;
 * its alpha is the peak strength of the sheen. Defaults to a semi-transparent
 * white. */
LCUI_API void ui_shimmer_set_color(ui_widget_t *w, pd_color_t color);

/* Set the time for one full sweep in milliseconds. Defaults to 2000. */
LCUI_API void ui_shimmer_set_duration(ui_widget_t *w, unsigned ms);

/* Set the highlight band width in logical pixels; when unset it defaults to
 * about 45% of the box width, recomputed on resize. */
LCUI_API void ui_shimmer_set_spread(ui_widget_t *w, float px);

/* Set the tilt of the highlight band in degrees. Defaults to 20. */
LCUI_API void ui_shimmer_set_angle(ui_widget_t *w, float degrees);

/* Set the sweep phase in [0, 1); while running the animation overrides it.
 * Added for deterministic tests and previews. */
LCUI_API void ui_shimmer_set_phase(ui_widget_t *w, float phase);

/* Replace the direct `shimmer-content` child with the given subtree. */
LCUI_API void ui_shimmer_set_content(ui_widget_t *w, ui_widget_t *content);

LCUI_API void ui_register_shimmer(void);

LCUI_END_HEADER

#endif /* LCUI_INCLUDE_WIDGETS_SHIMMER_H */

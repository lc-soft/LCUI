/*
 * lib/ui/src/ui_widget_box_shadow.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>
#include <ui/base.h>
#include <ui/metrics.h>
#include "ui_widget_box_shadow.h"

void ui_widget_paint_box_shadow(ui_widget_t* w, pd_context_t* ctx,
				ui_widget_actual_style_t* style)
{
	pd_rect_t box;
	pd_boxshadow_t bs;
	css_computed_style_t* s = &w->computed_style;

	if (s->type_bits.box_shadow == CSS_BOX_SHADOW_NONE ||
	    s->box_shadow_color < 1) {
		return;
	}
	bs.x = ui_compute(s->box_shadow_x);
	bs.y = ui_compute(s->box_shadow_y);
	bs.blur = ui_compute(s->box_shadow_blur);
	bs.spread = ui_compute(s->box_shadow_spread);
	bs.color.value = s->box_shadow_color;
	bs.top_left_radius = ui_compute(s->border_top_left_radius);
	bs.top_right_radius = ui_compute(s->border_top_right_radius);
	bs.bottom_left_radius = ui_compute(s->border_bottom_left_radius);
	bs.bottom_right_radius = ui_compute(s->border_bottom_right_radius);
	box.x = box.y = 0;
	box.width = style->canvas_box.width;
	box.height = style->canvas_box.height;
	pd_paint_boxshadow(ctx, &bs, &box, style->border_box.width,
			   style->border_box.height);
}

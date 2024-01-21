/*
 * lib/ui/src/ui_widget_border.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <pandagl.h>
#include <ui/base.h>
#include <ui/metrics.h>
#include "ui_widget_border.h"

static void ui_widget_compute_border(ui_widget_t* w, pd_border_t* b)
{
	css_computed_style_t* s = &w->computed_style;
	float r = y_min(w->border_box.width, w->border_box.width) / 2.0f;

	b->top.color.value = s->border_top_color;
	b->left.color.value = s->border_left_color;
	b->right.color.value = s->border_right_color;
	b->bottom.color.value = s->border_bottom_color;
	b->top.style = s->type_bits.border_top_style;
	b->left.style = s->type_bits.border_left_style;
	b->right.style = s->type_bits.border_right_style;
	b->bottom.style = s->type_bits.border_bottom_style;
	b->top.width = ui_compute(s->border_top_width);
	b->left.width = ui_compute(s->border_left_width);
	b->right.width = ui_compute(s->border_right_width);
	b->bottom.width = ui_compute(s->border_bottom_width);
	b->top_left_radius = ui_compute(y_min(s->border_top_left_radius, r));
	b->top_right_radius = ui_compute(y_min(s->border_top_right_radius, r));
	b->bottom_left_radius =
	    ui_compute(y_min(s->border_bottom_left_radius, r));
	b->bottom_right_radius =
	    ui_compute(y_min(s->border_bottom_right_radius, r));
}

void ui_widget_paint_border(ui_widget_t* w, pd_context_t* ctx,
			    ui_widget_actual_style_t* style)
{
	pd_rect_t box;
	pd_border_t border;

	box.x = style->border_box.x - style->canvas_box.x;
	box.y = style->border_box.y - style->canvas_box.y;
	box.width = style->border_box.width;
	box.height = style->border_box.height;
	ui_widget_compute_border(w, &border);
	pd_paint_border(ctx, &border, &box);
}

void ui_widget_crop_content(ui_widget_t* w, pd_context_t* ctx,
			    ui_widget_actual_style_t* style)
{
	pd_rect_t box;
	pd_border_t border;

	box.x = style->border_box.x - style->canvas_box.x;
	box.y = style->border_box.y - style->canvas_box.y;
	box.width = style->border_box.width;
	box.height = style->border_box.height;
	ui_widget_compute_border(w, &border);
	pd_crop_border_content(ctx, &border, &box);
}

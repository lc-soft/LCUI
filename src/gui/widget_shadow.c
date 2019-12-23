/*
 * widget_shadow.c -- widget shadow style processing module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/draw/boxshadow.h>

static float ComputeXMetric(LCUI_Widget w, LCUI_Style s)
{
	if (s->type == LCUI_STYPE_SCALE) {
		return w->width * s->scale;
	}
	return LCUIMetrics_Compute(s->value, s->type);
}

static float ComputeYMetric(LCUI_Widget w, LCUI_Style s)
{
	if (s->type == LCUI_STYPE_SCALE) {
		return w->height * s->scale;
	}
	return LCUIMetrics_Compute(s->value, s->type);
}

void Widget_ComputeBoxShadowStyle(LCUI_Widget w)
{
	int key;
	LCUI_Style s;
	LCUI_BoxShadowStyle *sd;

	sd = &w->computed_style.shadow;
	memset(sd, 0, sizeof(LCUI_BoxShadowStyle));
	for (key = key_box_shadow_start; key <= key_box_shadow_end; ++key) {
		s = &w->style->sheet[key];
		if (!s->is_valid) {
			continue;
		}
		switch (key) {
		case key_box_shadow_x:
			sd->x = ComputeXMetric(w, s);
			break;
		case key_box_shadow_y:
			sd->y = ComputeYMetric(w, s);
			break;
		case key_box_shadow_spread:
			sd->spread = LCUIMetrics_Compute(s->value, s->type);
			break;
		case key_box_shadow_blur:
			sd->blur = LCUIMetrics_Compute(s->value, s->type);
			break;
		case key_box_shadow_color:
			sd->color = s->color;
			break;
		default:
			break;
		}
	}
}

float Widget_GetBoxShadowOffsetX(LCUI_Widget w)
{
	const LCUI_BoxShadowStyle *shadow;

	shadow = &w->computed_style.shadow;
	if (shadow->x >= SHADOW_WIDTH(shadow)) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

float Widget_GetBoxShadowOffsetY(LCUI_Widget w)
{
	const LCUI_BoxShadowStyle *shadow;

	shadow = &w->computed_style.shadow;
	if (shadow->y >= SHADOW_WIDTH(shadow)) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

float Widget_GetCanvasWidth(LCUI_Widget widget)
{
	float width;
	const LCUI_BoxShadowStyle *shadow;

	width = widget->box.border.width;
	shadow = &widget->computed_style.shadow;
	if (shadow->x >= SHADOW_WIDTH(shadow)) {
		return width + SHADOW_WIDTH(shadow) + shadow->x;
	} else if (shadow->x <= -SHADOW_WIDTH(shadow)) {
		return width + SHADOW_WIDTH(shadow) - shadow->x;
	}
	return width + SHADOW_WIDTH(shadow) * 2;
}

float Widget_GetCanvasHeight(LCUI_Widget widget)
{
	float height;
	const LCUI_BoxShadowStyle *shadow;

	height = widget->box.border.height;
	shadow = &widget->computed_style.shadow;
	if (shadow->y >= SHADOW_WIDTH(shadow)) {
		return height + SHADOW_WIDTH(shadow) + shadow->y;
	} else if (shadow->y <= -SHADOW_WIDTH(shadow)) {
		return height + SHADOW_WIDTH(shadow) - shadow->y;
	}
	return height + SHADOW_WIDTH(shadow) * 2;
}

#define ComputeActual(X) LCUIMetrics_ComputeActual(X, LCUI_STYPE_PX)

void Widget_ComputeBoxShadow(LCUI_Widget w, LCUI_BoxShadow *out)
{
	LCUI_BoxShadowStyle *s;
	LCUI_BorderStyle *b;

	b = &w->computed_style.border;
	s = &w->computed_style.shadow;
	out->x = ComputeActual(s->x);
	out->y = ComputeActual(s->y);
	out->blur = ComputeActual(s->blur);
	out->spread = ComputeActual(s->spread);
	out->color = s->color;
	out->top_left_radius = ComputeActual(b->top_left_radius);
	out->top_right_radius = ComputeActual(b->top_right_radius);
	out->bottom_left_radius = ComputeActual(b->bottom_left_radius);
	out->bottom_right_radius = ComputeActual(b->bottom_right_radius);
}

void Widget_PaintBoxShadow(LCUI_Widget w, LCUI_PaintContext paint,
			   LCUI_WidgetActualStyle style)
{
	LCUI_Rect box;

	box.x = box.y = 0;
	box.width = style->canvas_box.width;
	box.height = style->canvas_box.height;
	BoxShadow_Paint(&style->shadow, &box, style->border_box.width,
			style->border_box.height, paint);
}

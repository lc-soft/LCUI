#include <string.h>
#include <LCUI/util.h>
#include <LCUI/graph.h>
#include <LCUI/gui/css_library.h>
#include "../include/ui.h"
#include "internal.h"

#define compute_actual(X) ui_compute_actual(X, LCUI_STYPE_PX)

static float compute_metric_x(ui_widget_t* w, LCUI_Style s)
{
	if (s->type == LCUI_STYPE_SCALE) {
		return w->width * s->scale;
	}
	return ui_compute(s->value, s->type);
}

static float compute_metric_y(ui_widget_t* w, LCUI_Style s)
{
	if (s->type == LCUI_STYPE_SCALE) {
		return w->height * s->scale;
	}
	return ui_compute(s->value, s->type);
}

void ui_widget_compute_box_shadow_style(ui_widget_t* w)
{
	int key;
	LCUI_Style s;
	ui_boxshadow_style_t* sd;

	sd = &w->computed_style.shadow;
	memset(sd, 0, sizeof(ui_boxshadow_style_t));
	for (key = key_box_shadow_start; key <= key_box_shadow_end; ++key) {
		s = &w->style->sheet[key];
		if (!s->is_valid) {
			continue;
		}
		switch (key) {
		case key_box_shadow_x:
			sd->x = compute_metric_x(w, s);
			break;
		case key_box_shadow_y:
			sd->y = compute_metric_y(w, s);
			break;
		case key_box_shadow_spread:
			sd->spread = ui_compute(s->value, s->type);
			break;
		case key_box_shadow_blur:
			sd->blur = ui_compute(s->value, s->type);
			break;
		case key_box_shadow_color:
			sd->color = s->color;
			break;
		default:
			break;
		}
	}
}

void ui_widget_compute_box_shadow(ui_widget_t* w, pd_boxshadow_t* out)
{
	ui_boxshadow_style_t* s;
	ui_border_style_t* b;

	b = &w->computed_style.border;
	s = &w->computed_style.shadow;
	out->x = compute_actual(s->x);
	out->y = compute_actual(s->y);
	out->blur = compute_actual(s->blur);
	out->spread = compute_actual(s->spread);
	out->color = s->color;
	out->top_left_radius = compute_actual(b->top_left_radius);
	out->top_right_radius = compute_actual(b->top_right_radius);
	out->bottom_left_radius = compute_actual(b->bottom_left_radius);
	out->bottom_right_radius = compute_actual(b->bottom_right_radius);
}

void ui_widget_paint_box_shadow(ui_widget_t* w, pd_paint_context_t *paint,
				ui_widget_actual_style_t* style)
{
	pd_rect_t box;

	box.x = box.y = 0;
	box.width = style->canvas_box.width;
	box.height = style->canvas_box.height;
	pd_boxshadow_paint(&style->shadow, &box, style->border_box.width,
			style->border_box.height, paint);
}

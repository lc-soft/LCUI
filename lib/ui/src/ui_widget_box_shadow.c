#include <LCUI.h>
#include <LCUI/gui/css_library.h>
#include "../include/ui.h"
#include "private.h"

#define ComputeActual(X) ui_compute_actual(X, LCUI_STYPE_PX)

static float ComputeXMetric(ui_widget_t* w, LCUI_Style s)
{
	if (s->type == LCUI_STYPE_SCALE) {
		return w->width * s->scale;
	}
	return ui_compute(s->value, s->type);
}

static float ComputeYMetric(ui_widget_t* w, LCUI_Style s)
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
	LCUI_BoxShadowStyle* sd;

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

void ui_widget_compute_box_shadow(ui_widget_t* w, LCUI_BoxShadow* out)
{
	LCUI_BoxShadowStyle* s;
	LCUI_BorderStyle* b;

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

void ui_widget_paint_box_shadow(ui_widget_t* w, LCUI_PaintContext paint,
				ui_widget_actual_style_t* style)
{
	LCUI_Rect box;

	box.x = box.y = 0;
	box.width = style->canvas_box.width;
	box.height = style->canvas_box.height;
	BoxShadow_Paint(&style->shadow, &box, style->border_box.width,
			style->border_box.height, paint);
}

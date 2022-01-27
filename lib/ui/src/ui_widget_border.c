#include <string.h>
#include <LCUI/util.h>
#include <LCUI/graph.h>
#include <LCUI/draw.h>
#include <LCUI/gui/css_library.h>
#include "../include/ui.h"
#include "internal.h"

static float compute_metric_x(ui_widget_t* w, css_unit_value_t *s)
{
	if (s->type == CSS_UNIT_SCALE) {
		return w->width * s->scale;
	}
	return ui_compute(s->value, s->type);
}

static float compute_metric_y(ui_widget_t* w, css_unit_value_t *s)
{
	if (s->type == CSS_UNIT_SCALE) {
		return w->height * s->scale;
	}
	return ui_compute(s->value, s->type);
}

static unsigned int compute_actual(float width)
{
	unsigned int w;

	w = ui_compute_actual(width, CSS_UNIT_PX);
	if (width > 0 && w < 1) {
		return 1;
	}
	return w;
}

void ui_widget_compute_border_style(ui_widget_t* w)
{
	int key;
	css_unit_value_t *s;
	ui_border_style_t* b;

	b = &w->computed_style.border;
	memset(b, 0, sizeof(ui_border_style_t));
	for (key = css_key_border_start; key <= css_key_border_end; ++key) {
		s = &w->style->sheet[key];
		if (!s->is_valid) {
			continue;
		}
		switch (key) {
		case css_key_border_top_color:
			b->top.color = s->color;
			break;
		case css_key_border_right_color:
			b->right.color = s->color;
			break;
		case css_key_border_bottom_color:
			b->bottom.color = s->color;
			break;
		case css_key_border_left_color:
			b->left.color = s->color;
			break;
		case css_key_border_top_width:
			b->top.width = compute_metric_x(w, s);
			break;
		case css_key_border_right_width:
			b->right.width = compute_metric_y(w, s);
			break;
		case css_key_border_bottom_width:
			b->bottom.width = compute_metric_x(w, s);
			break;
		case css_key_border_left_width:
			b->left.width = compute_metric_y(w, s);
			break;
		case css_key_border_top_style:
			b->top.style = s->val_style;
			break;
		case css_key_border_right_style:
			b->right.style = s->val_style;
			break;
		case css_key_border_bottom_style:
			b->bottom.style = s->val_style;
			break;
		case css_key_border_left_style:
			b->left.style = s->val_style;
			break;
		case css_key_border_top_left_radius:
			b->top_left_radius = compute_metric_x(w, s);
			break;
		case css_key_border_top_right_radius:
			b->top_right_radius = compute_metric_x(w, s);
			break;
		case css_key_border_bottom_left_radius:
			b->bottom_left_radius = compute_metric_x(w, s);
			break;
		case css_key_border_bottom_right_radius:
			b->bottom_right_radius = compute_metric_x(w, s);
			break;
		default:
			break;
		}
	}
}

/** 计算部件边框样式的实际值 */
void ui_widget_compute_border(ui_widget_t* w, pd_border_t* b)
{
	ui_border_style_t* s;
	float r = y_min(w->width, w->height) / 2.0f;

	s = &w->computed_style.border;
	b->top.color = s->top.color;
	b->left.color = s->left.color;
	b->right.color = s->right.color;
	b->bottom.color = s->bottom.color;
	b->top.style = s->top.style;
	b->left.style = s->left.style;
	b->right.style = s->right.style;
	b->bottom.style = s->bottom.style;
	b->top.width = compute_actual(s->top.width);
	b->left.width = compute_actual(s->left.width);
	b->right.width = compute_actual(s->right.width);
	b->bottom.width = compute_actual(s->bottom.width);
	b->top_left_radius = compute_actual(y_min(s->top_left_radius, r));
	b->top_right_radius = compute_actual(y_min(s->top_right_radius, r));
	b->bottom_left_radius = compute_actual(y_min(s->bottom_left_radius, r));
	b->bottom_right_radius = compute_actual(y_min(s->bottom_right_radius, r));
}

void ui_widget_paint_border(ui_widget_t* w, pd_paint_context_t *paint,
			    ui_widget_actual_style_t* style)
{
	pd_rect_t box;

	box.x = style->border_box.x - style->canvas_box.x;
	box.y = style->border_box.y - style->canvas_box.y;
	box.width = style->border_box.width;
	box.height = style->border_box.height;
	pd_border_paint(&style->border, &box, paint);
}

void ui_widget_crop_content(ui_widget_t* w, pd_paint_context_t *paint,
			    ui_widget_actual_style_t* style)
{
	pd_rect_t box;

	box.x = style->border_box.x - style->canvas_box.x;
	box.y = style->border_box.y - style->canvas_box.y;
	box.width = style->border_box.width;
	box.height = style->border_box.height;
	pd_border_crop_content(&style->border, &box, paint);
}

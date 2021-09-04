#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

void ui_widget_update_box_position(ui_widget_t* w)
{
	float x = w->layout_x;
	float y = w->layout_y;

	switch (w->computed_style.position) {
	case SV_ABSOLUTE:
		if (!ui_widget_has_auto_style(w, key_left)) {
			x = w->computed_style.left;
		} else if (!ui_widget_has_auto_style(w, key_right)) {
			if (w->parent) {
				x = w->parent->box.border.width - w->width;
			}
			x -= w->computed_style.right;
		}
		if (!ui_widget_has_auto_style(w, key_top)) {
			y = w->computed_style.top;
		} else if (!ui_widget_has_auto_style(w, key_bottom)) {
			if (w->parent) {
				y = w->parent->box.border.height - w->height;
			}
			y -= w->computed_style.bottom;
		}
		break;
	case SV_RELATIVE:
		if (!ui_widget_has_auto_style(w, key_left)) {
			x += w->computed_style.left;
		} else if (!ui_widget_has_auto_style(w, key_right)) {
			x -= w->computed_style.right;
		}
		if (!ui_widget_has_auto_style(w, key_top)) {
			y += w->computed_style.top;
		} else if (!ui_widget_has_auto_style(w, key_bottom)) {
			y -= w->computed_style.bottom;
		}
	case SV_STATIC:
	default:
		break;
	}
	w->box.outer.x = x;
	w->box.outer.y = y;
	w->x = x + w->margin.left;
	w->y = y + w->margin.top;
	if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_NONE &&
	    (w->x != w->box.border.x || w->y != w->box.border.y)) {
		ui_widget_t* parent = w->parent;

		w->dirty_rect = w->box.canvas;
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		while (parent) {
			parent->has_child_dirty_rect = TRUE;
			parent = parent->parent;
		}
	}
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.padding.x = w->x + w->computed_style.border.left.width;
	w->box.padding.y = w->y + w->computed_style.border.top.width;
	w->box.content.x = w->box.padding.x + w->padding.left;
	w->box.content.y = w->box.padding.y + w->padding.top;
	w->box.canvas.x = w->x - ui_widget_get_box_shadow_offset_x(w);
	w->box.canvas.y = w->y - ui_widget_get_box_shadow_offset_y(w);
}

float ui_widget_get_canvas_box_width(ui_widget_t* widget)
{
	float width;
	const LCUI_BoxShadowStyle* shadow;

	width = widget->box.border.width;
	shadow = &widget->computed_style.shadow;
	if (shadow->x >= SHADOW_WIDTH(shadow)) {
		return width + SHADOW_WIDTH(shadow) + shadow->x;
	} else if (shadow->x <= -SHADOW_WIDTH(shadow)) {
		return width + SHADOW_WIDTH(shadow) - shadow->x;
	}
	return width + SHADOW_WIDTH(shadow) * 2;
}

float ui_widget_get_canvas_box_height(ui_widget_t* widget)
{
	float height;
	const LCUI_BoxShadowStyle* shadow;

	height = widget->box.border.height;
	shadow = &widget->computed_style.shadow;
	if (shadow->y >= SHADOW_WIDTH(shadow)) {
		return height + SHADOW_WIDTH(shadow) + shadow->y;
	} else if (shadow->y <= -SHADOW_WIDTH(shadow)) {
		return height + SHADOW_WIDTH(shadow) - shadow->y;
	}
	return height + SHADOW_WIDTH(shadow) * 2;
}

void ui_widget_update_canvas_box(ui_widget_t* w)
{
	w->box.canvas.x =
	    w->box.border.x - ui_widget_get_box_shadow_offset_x(w);
	w->box.canvas.y =
	    w->box.border.y - ui_widget_get_box_shadow_offset_y(w);
	w->box.canvas.width = ui_widget_get_canvas_box_width(w);
	w->box.canvas.height = ui_widget_get_canvas_box_height(w);
}

void ui_widget_update_box_size(ui_widget_t* w)
{
	w->width = Widget_GetLimitedWidth(w, w->width);
	w->height = Widget_GetLimitedHeight(w, w->height);
	if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_NONE &&
	    (w->width != w->box.border.width ||
	     w->height != w->box.border.height)) {
		ui_widget_t* parent = w->parent;

		w->dirty_rect = w->box.canvas;
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		while (parent) {
			parent->has_child_dirty_rect = TRUE;
			parent = parent->parent;
		}
	}
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.padding.width = w->box.border.width - Widget_BorderX(w);
	w->box.padding.height = w->box.border.height - Widget_BorderY(w);
	w->box.content.width = w->box.padding.width - Widget_PaddingX(w);
	w->box.content.height = w->box.padding.height - Widget_PaddingY(w);
	w->box.outer.width = w->box.border.width + Widget_MarginX(w);
	w->box.outer.height = w->box.border.height + Widget_MarginY(w);
	w->box.canvas.width = ui_widget_get_canvas_box_width(w);
	w->box.canvas.height = ui_widget_get_canvas_box_height(w);
}

float ui_widget_get_box_shadow_offset_x(ui_widget_t* w)
{
	const LCUI_BoxShadowStyle* shadow;

	shadow = &w->computed_style.shadow;
	if (shadow->x >= SHADOW_WIDTH(shadow)) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->x;
}

float ui_widget_get_box_shadow_offset_y(ui_widget_t* w)
{
	const LCUI_BoxShadowStyle* shadow;

	shadow = &w->computed_style.shadow;
	if (shadow->y >= SHADOW_WIDTH(shadow)) {
		return 0;
	}
	return SHADOW_WIDTH(shadow) - shadow->y;
}

void ui_widget_compute_border_box_actual(ui_widget_t* w,
					 ui_widget_actual_style_t* s)
{
	LCUI_RectF rect;
	rect.x = s->x + w->box.border.x;
	rect.y = s->y + w->box.border.y;
	rect.width = w->box.border.width;
	rect.height = w->box.border.height;
	ui_widget_compute_border(w, &s->border);
	ui_compute_rect_actual(&s->border_box, &rect);
}

void ui_widget_compute_canvas_box_actual(ui_widget_t* w,
					 ui_widget_actual_style_t* s)
{
	ui_widget_compute_box_shadow(w, &s->shadow);
	BoxShadow_GetCanvasRect(&s->shadow, &s->border_box, &s->canvas_box);
}

void ui_widget_compute_padding_box_actual(ui_widget_t* w,
					  ui_widget_actual_style_t* s)
{
	ui_widget_compute_background(w, &s->background);
	s->padding_box.x = s->border_box.x + s->border.left.width;
	s->padding_box.y = s->border_box.y + s->border.top.width;
	s->padding_box.width = s->border_box.width - s->border.left.width;
	s->padding_box.width -= s->border.right.width;
	s->padding_box.height = s->border_box.height - s->border.top.width;
	s->padding_box.height -= s->border.bottom.width;
}

void ui_widget_compute_content_box_actual(ui_widget_t* w,
					  ui_widget_actual_style_t* s)
{
	LCUI_RectF rect;
	rect.x = s->x + w->box.content.x;
	rect.y = s->y + w->box.content.y;
	rect.width = w->box.content.width;
	rect.height = w->box.content.height;
	ui_compute_rect_actual(&s->content_box, &rect);
}

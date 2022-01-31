#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/css/library.h>
#include "../include/ui.h"
#include "internal.h"

void ui_widget_init_background(ui_widget_t *w)
{
	ui_background_style_t *bg;
	bg = &w->computed_style.background;
	bg->color = RGB(255, 255, 255);
	bg->image = NULL;
	bg->size.using_value = TRUE;
	bg->size.value = CSS_KEYWORD_AUTO;
	bg->position.using_value = TRUE;
	bg->position.value = CSS_KEYWORD_AUTO;
}

void ui_widget_destroy_background(ui_widget_t *w)
{
	ui_widget_unset_style(w, css_key_background_image);
	w->computed_style.background.image = NULL;
	if (ui_widget_check_style_type(w, css_key_background_image, string)) {
		ui_image_remove_ref(
		    (ui_image_t *)&w->computed_style.background.image);
	}
}

static void ui_widget_on_background_image_load(ui_image_t *image,
					       void *w)
{
	ui_widget_mark_dirty_rect(w, NULL, CSS_KEYWORD_BORDER_BOX);
}

void ui_widget_compute_background_style(ui_widget_t *widget)
{
	css_unit_value_t *s;
	css_style_decl_t *ss = widget->style;
	ui_background_style_t *bg = &widget->computed_style.background;
	int key = css_key_background_start;

	for (; key <= css_key_background_end; ++key) {
		s = &ss->sheet[key];
		switch (key) {
		case css_key_background_color:
			if (s->is_valid) {
				bg->color = s->color;
			} else {
				bg->color.value = 0;
			}
			break;
		case css_key_background_image:
			if (!s->is_valid) {
				bg->image = NULL;
				break;
			}
			switch (s->unit) {
			case CSS_UNIT_STRING:
				bg->image = (pd_canvas_t*)ui_load_image(s->string);
				ui_image_on_event(
				    (ui_image_t *)bg->image,
				    ui_widget_on_background_image_load, widget);
				ui_image_add_ref((ui_image_t *)bg->image);
				break;
			case CSS_UNIT_IMAGE:
				if (s->image) {
					bg->image = s->image;
				}
			default:
				break;
			}
			break;
		case css_key_background_position:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->position.using_value = TRUE;
				bg->position.value = s->val_keyword;
			} else {
				bg->position.using_value = FALSE;
				bg->position.value = 0;
			}
			break;
		case css_key_background_position_x:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->position.using_value = FALSE;
				bg->position.x = *s;
			}
			break;
		case css_key_background_position_y:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->position.using_value = FALSE;
				bg->position.y = *s;
			}
			break;
		case css_key_background_size:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->size.using_value = TRUE;
				bg->size.value = s->val_keyword;
			} else {
				bg->size.using_value = FALSE;
				bg->size.value = 0;
			}
			break;
		case css_key_background_size_width:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->size.using_value = FALSE;
				bg->size.width = *s;
			}
			break;
		case css_key_background_size_height:
			if (s->is_valid && s->unit != CSS_UNIT_NONE) {
				bg->size.using_value = FALSE;
				bg->size.height = *s;
			}
			break;
		default:
			break;
		}
	}
}

void ui_widget_compute_background(ui_widget_t *w, pd_background_t *out)
{
	css_unit_t type;
	pd_rectf_t *box = &w->box.border;
	ui_background_style_t *bg = &w->computed_style.background;
	float scale, x = 0, y = 0, width, height;

	out->color = bg->color;
	out->image = bg->image;
	out->repeat.x = bg->repeat.x;
	out->repeat.y = bg->repeat.y;
	if (!bg->image) {
		out->position.x = 0;
		out->position.y = 0;
		out->size.width = 0;
		out->size.height = 0;
		return;
	}
	/* 计算背景图应有的尺寸 */
	if (bg->size.using_value) {
		switch (bg->size.value) {
		case CSS_KEYWORD_CONTAIN:
			width = box->width;
			scale = 1.0f * bg->image->width / width;
			height = bg->image->height / scale;
			if (height > box->height) {
				height = box->height;
				scale = 1.0f * bg->image->height / box->height;
				width = bg->image->width / scale;
			}
			break;
		case CSS_KEYWORD_COVER:
			width = box->width;
			scale = 1.0f * bg->image->width / width;
			height = bg->image->height / scale;
			if (height < box->height) {
				height = box->height;
				scale = 1.0f * bg->image->height / height;
				width = bg->image->width / scale;
			}
			x = (box->width - width) / 2.0f;
			y = (box->height - height) / 2.0f;
			break;
		case CSS_KEYWORD_AUTO:
		default:
			width = (float)bg->image->width;
			height = (float)bg->image->height;
			break;
		}
		out->position.x = ui_compute_actual(x, CSS_UNIT_PX);
		out->position.y = ui_compute_actual(y, CSS_UNIT_PX);
		out->size.width = ui_compute_actual(width, CSS_UNIT_PX);
		out->size.height = ui_compute_actual(height, CSS_UNIT_PX);
	} else {
		type = CSS_UNIT_PX;
		switch (bg->size.width.unit) {
		case CSS_UNIT_SCALE:
			width = box->width * bg->size.width.scale;
			break;
		case CSS_UNIT_NONE:
		case CSS_UNIT_AUTO:
			width = (float)bg->image->width;
			break;
		default:
			width = bg->size.width.value;
			type = bg->size.width.unit;
			break;
		}
		out->size.width = ui_compute_actual(width, type);
		type = CSS_UNIT_PX;
		switch (bg->size.height.unit) {
		case CSS_UNIT_SCALE:
			height = box->height * bg->size.height.scale;
			break;
		case CSS_UNIT_NONE:
		case CSS_UNIT_AUTO:
			height = (float)bg->image->height;
			break;
		default:
			height = (float)bg->size.height.value;
			break;
		}
		out->size.height = ui_compute_actual(height, type);
	}
	/* 计算背景图的像素坐标 */
	if (bg->position.using_value) {
		switch (bg->position.value) {
		case CSS_KEYWORD_TOP:
		case CSS_KEYWORD_TOP_CENTER:
			x = (box->width - width) / 2;
			y = 0;
			break;
		case CSS_KEYWORD_TOP_RIGHT:
			x = box->width - width;
			y = 0;
			break;
		case CSS_KEYWORD_CENTER_LEFT:
			x = 0;
			y = (box->height - height) / 2;
			break;
		case CSS_KEYWORD_CENTER:
		case CSS_KEYWORD_CENTER_CENTER:
			x = (box->width - width) / 2;
			y = (box->height - height) / 2;
			break;
		case CSS_KEYWORD_CENTER_RIGHT:
			x = box->width - width;
			y = (box->height - height) / 2;
			break;
		case CSS_KEYWORD_BOTTOM_LEFT:
			x = 0;
			y = box->height - height;
			break;
		case CSS_KEYWORD_BOTTOM_CENTER:
			x = (box->width - width) / 2;
			y = box->height - height;
			break;
		case CSS_KEYWORD_BOTTOM_RIGHT:
			x = box->width - width;
			y = box->height - height;
			break;
		case CSS_KEYWORD_TOP_LEFT:
		default:
			break;
		}
		out->position.x = ui_compute_actual(x, CSS_UNIT_PX);
		out->position.y = ui_compute_actual(y, CSS_UNIT_PX);
	} else {
		type = CSS_UNIT_PX;
		switch (bg->position.x.unit) {
		case CSS_UNIT_SCALE:
			x = box->width - width;
			x = x * bg->position.x.scale;
			break;
		case CSS_UNIT_NONE:
		case CSS_UNIT_AUTO:
			break;
		default:
			x = bg->position.x.value;
			type = bg->position.x.unit;
			break;
		}
		out->position.x = ui_compute_actual(x, type);
		type = CSS_UNIT_PX;
		switch (bg->position.y.unit) {
		case CSS_UNIT_SCALE:
			y = box->height - height;
			y = y * bg->position.y.scale;
			break;
		case CSS_UNIT_NONE:
		case CSS_UNIT_AUTO:
			break;
		default:
			y = bg->position.y.value;
			type = bg->position.y.unit;
			break;
		}
		out->position.y = ui_compute_actual(y, type);
	}
}

void ui_widget_paint_background(ui_widget_t *w, pd_paint_context_t *paint,
				ui_widget_actual_style_t* style)
{
	pd_rect_t box;
	box.x = style->padding_box.x - style->canvas_box.x;
	box.y = style->padding_box.y - style->canvas_box.y;
	box.width = style->padding_box.width;
	box.height = style->padding_box.height;
	pd_background_paint(&style->background, &box, paint);
}

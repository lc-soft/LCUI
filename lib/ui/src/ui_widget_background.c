#include <LCUI.h>
#include <LCUI/gui/css_library.h>
#include "../include/ui.h"
#include "private.h"

void ui_widget_init_background(ui_widget_t *w)
{
	LCUI_BackgroundStyle *bg;
	bg = &w->computed_style.background;
	bg->color = RGB(255, 255, 255);
	Graph_Init(&bg->image);
	bg->size.using_value = TRUE;
	bg->size.value = SV_AUTO;
	bg->position.using_value = TRUE;
	bg->position.value = SV_AUTO;
}

void ui_widget_destroy_background(ui_widget_t *w)
{
	ui_widget_unset_style(w, key_background_image);
	Graph_Init(&w->computed_style.background.image);
	if (ui_widget_check_style_type(w, key_background_image, string)) {
		ui_image_remove_ref(
		    (ui_image_t *)&w->computed_style.background.image);
	}
}

static void ui_widget_on_background_image_load(ui_image_t *image,
					       ui_widget_t *w)
{
	ui_widget_mark_dirty_rect(w, NULL, SV_BORDER_BOX);
}

void ui_widget_compute_background_style(ui_widget_t *widget)
{
	LCUI_Style s;
	LCUI_StyleSheet ss = widget->style;
	LCUI_BackgroundStyle *bg = &widget->computed_style.background;
	int key = key_background_start;

	for (; key <= key_background_end; ++key) {
		s = &ss->sheet[key];
		switch (key) {
		case key_background_color:
			if (s->is_valid) {
				bg->color = s->color;
			} else {
				bg->color.value = 0;
			}
			break;
		case key_background_image:
			if (!s->is_valid) {
				Graph_Init(&bg->image);
				break;
			}
			switch (s->type) {
			case LCUI_STYPE_STRING:
				bg->image = ui_load_image(s->string);
				ui_image_on_event(
				    (ui_image_t *)bg->image,
				    ui_widget_on_background_image_load, widget);
				ui_image_add_ref((ui_image_t *)bg->image);
				break;
			case LCUI_STYPE_IMAGE:
				if (s->image) {
					bg->image = s->image;
				}
			default:
				break;
			}
			break;
		case key_background_position:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = TRUE;
				bg->position.value = s->val_style;
			} else {
				bg->position.using_value = FALSE;
				bg->position.value = 0;
			}
			break;
		case key_background_position_x:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = FALSE;
				bg->position.x = *s;
			}
			break;
		case key_background_position_y:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->position.using_value = FALSE;
				bg->position.y = *s;
			}
			break;
		case key_background_size:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = TRUE;
				bg->size.value = s->val_style;
			} else {
				bg->size.using_value = FALSE;
				bg->size.value = 0;
			}
			break;
		case key_background_size_width:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = FALSE;
				bg->size.width = *s;
			}
			break;
		case key_background_size_height:
			if (s->is_valid && s->type != LCUI_STYPE_NONE) {
				bg->size.using_value = FALSE;
				bg->size.height = *s;
			}
			break;
		default:
			break;
		}
	}
}

void ui_widget_compute_background(ui_widget_t *w, LCUI_Background *out)
{
	LCUI_StyleType type;
	LCUI_RectF *box = &w->box.border;
	LCUI_BackgroundStyle *bg = &w->computed_style.background;
	float scale, x = 0, y = 0, width, height;

	/* 计算背景图应有的尺寸 */
	if (bg->size.using_value) {
		switch (bg->size.value) {
		case SV_CONTAIN:
			width = box->width;
			scale = 1.0f * bg->image->width / width;
			height = bg->image->height / scale;
			if (height > box->height) {
				height = box->height;
				scale = 1.0f * bg->image->height / box->height;
				width = bg->image->width / scale;
			}
			break;
		case SV_COVER:
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
		case SV_AUTO:
		default:
			width = (float)bg->image->width;
			height = (float)bg->image->height;
			break;
		}
		out->position.x = ComputeActual(x, LCUI_STYPE_PX);
		out->position.y = ComputeActual(y, LCUI_STYPE_PX);
		out->size.width = ComputeActual(width, LCUI_STYPE_PX);
		out->size.height = ComputeActual(height, LCUI_STYPE_PX);
	} else {
		type = LCUI_STYPE_PX;
		switch (bg->size.width.type) {
		case LCUI_STYPE_SCALE:
			width = box->width * bg->size.width.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			width = (float)bg->image->width;
			break;
		default:
			width = bg->size.width.value;
			type = bg->size.width.type;
			break;
		}
		out->size.width = ComputeActual(width, type);
		type = LCUI_STYPE_PX;
		switch (bg->size.height.type) {
		case LCUI_STYPE_SCALE:
			height = box->height * bg->size.height.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			height = (float)bg->image->height;
			break;
		default:
			height = (float)bg->size.height.value;
			break;
		}
		out->size.height = ComputeActual(height, type);
	}
	/* 计算背景图的像素坐标 */
	if (bg->position.using_value) {
		switch (bg->position.value) {
		case SV_TOP:
		case SV_TOP_CENTER:
			x = (box->width - width) / 2;
			y = 0;
			break;
		case SV_TOP_RIGHT:
			x = box->width - width;
			y = 0;
			break;
		case SV_CENTER_LEFT:
			x = 0;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER:
		case SV_CENTER_CENTER:
			x = (box->width - width) / 2;
			y = (box->height - height) / 2;
			break;
		case SV_CENTER_RIGHT:
			x = box->width - width;
			y = (box->height - height) / 2;
			break;
		case SV_BOTTOM_LEFT:
			x = 0;
			y = box->height - height;
			break;
		case SV_BOTTOM_CENTER:
			x = (box->width - width) / 2;
			y = box->height - height;
			break;
		case SV_BOTTOM_RIGHT:
			x = box->width - width;
			y = box->height - height;
			break;
		case SV_TOP_LEFT:
		default:
			break;
		}
		out->position.x = ComputeActual(x, LCUI_STYPE_PX);
		out->position.y = ComputeActual(y, LCUI_STYPE_PX);
	} else {
		type = LCUI_STYPE_PX;
		switch (bg->position.x.type) {
		case LCUI_STYPE_SCALE:
			x = box->width - width;
			x = x * bg->position.x.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			break;
		default:
			x = bg->position.x.value;
			type = bg->position.x.type;
			break;
		}
		out->position.x = ComputeActual(x, type);
		type = LCUI_STYPE_PX;
		switch (bg->position.y.type) {
		case LCUI_STYPE_SCALE:
			y = box->height - height;
			y = y * bg->position.y.scale;
			break;
		case LCUI_STYPE_NONE:
		case LCUI_STYPE_AUTO:
			break;
		default:
			y = bg->position.y.value;
			type = bg->position.y.type;
			break;
		}
		out->position.y = ComputeActual(y, type);
	}
	out->color = bg->color;
	out->image = &bg->image;
	out->repeat.x = bg->repeat.x;
	out->repeat.y = bg->repeat.y;
}

void ui_widget_paint_background(ui_widget_t *w, LCUI_PaintContext paint,
				ui_widget_actual_style_t* style)
{
	LCUI_Rect box;
	box.x = style->padding_box.x - style->canvas_box.x;
	box.y = style->padding_box.y - style->canvas_box.y;
	box.width = style->padding_box.width;
	box.height = style->padding_box.height;
	Background_Paint(&style->background, &box, paint);
}

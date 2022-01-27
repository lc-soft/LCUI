#include <assert.h>
#include <LCUI.h>
#include "../include/ui.h"
#include "internal.h"

void ui_widget_set_padding(ui_widget_t* w, float top, float right, float bottom,
		       float left)
{
	ui_widget_set_style(w, css_key_padding_top, top, px);
	ui_widget_set_style(w, css_key_padding_right, right, px);
	ui_widget_set_style(w, css_key_padding_bottom, bottom, px);
	ui_widget_set_style(w, css_key_padding_left, left, px);
	ui_widget_update_style(w);
}

void ui_widget_set_margin(ui_widget_t* w, float top, float right, float bottom,
		      float left)
{
	ui_widget_set_style(w, css_key_margin_top, top, px);
	ui_widget_set_style(w, css_key_margin_right, right, px);
	ui_widget_set_style(w, css_key_margin_bottom, bottom, px);
	ui_widget_set_style(w, css_key_margin_left, left, px);
	ui_widget_update_style(w);
}

void ui_widget_set_border_color(ui_widget_t* w, pd_color_t color)
{
	ui_widget_set_style(w, css_key_border_top_color, color, color);
	ui_widget_set_style(w, css_key_border_right_color, color, color);
	ui_widget_set_style(w, css_key_border_bottom_color, color, color);
	ui_widget_set_style(w, css_key_border_left_color, color, color);
	ui_widget_update_style(w);
}

void ui_widget_set_border_width(ui_widget_t* w, float width)
{
	ui_widget_set_style(w, css_key_border_top_width, width, px);
	ui_widget_set_style(w, css_key_border_right_width, width, px);
	ui_widget_set_style(w, css_key_border_bottom_width, width, px);
	ui_widget_set_style(w, css_key_border_left_width, width, px);
	ui_widget_update_style(w);
}

void ui_widget_set_border_style(ui_widget_t* w, int style)
{
	ui_widget_set_style(w, css_key_border_top_style, style, style);
	ui_widget_set_style(w, css_key_border_right_style, style, style);
	ui_widget_set_style(w, css_key_border_bottom_style, style, style);
	ui_widget_set_style(w, css_key_border_left_style, style, style);
}

void ui_widget_set_border(ui_widget_t* w, float width, int style, pd_color_t color)
{
	ui_widget_set_border_color(w, color);
	ui_widget_set_border_width(w, width);
	ui_widget_set_border_style(w, style);
}

void ui_widget_set_box_shadow(ui_widget_t* w, float x, float y, float blur,
			 pd_color_t color)
{
	ui_widget_set_style(w, css_key_box_shadow_x, x, px);
	ui_widget_set_style(w, css_key_box_shadow_y, y, px);
	ui_widget_set_style(w, css_key_box_shadow_blur, blur, px);
	ui_widget_set_style(w, css_key_box_shadow_color, color, color);
	ui_widget_update_style(w);
}

void ui_widget_move(ui_widget_t* w, float left, float top)
{
	ui_widget_set_style(w, css_key_top, top, px);
	ui_widget_set_style(w, css_key_left, left, px);
	ui_widget_update_style(w);
}

void ui_widget_resize(ui_widget_t* w, float width, float height)
{
	ui_widget_set_style(w, css_key_width, width, px);
	ui_widget_set_style(w, css_key_height, height, px);
	ui_widget_update_style(w);
}

css_unit_value_t *ui_widget_get_style(ui_widget_t* w, int key)
{
	css_style_property_t *node;

	if (w->custom_style) {
		node = css_style_properties_find(w->custom_style, key);
		if (node) {
			return &node->style;
		}
	} else {
		w->custom_style = css_style_properties_create();
	}
	node = css_style_properties_add(w->custom_style, key);
	return &node->style;
}

int ui_widget_unset_style(ui_widget_t* w, int key)
{
	if (!w->custom_style) {
		return -1;
	}
	ui_widget_add_task_by_style(w, key);
	return css_style_properties_remove(w->custom_style, key);
}

css_unit_value_t *ui_widget_get_matched_style(ui_widget_t* w, int key)
{
	css_selector_t *selector;

	if (!w->matched_style) {
		selector = ui_widget_create_selector(w);
		w->matched_style = css_get_computed_style_with_cache(selector);
		css_selector_destroy(selector);
	}
	assert(key >= 0 && key < w->matched_style->length);
	return &w->matched_style->sheet[key];
}

void ui_widget_set_visibility(ui_widget_t* w, const char *value)
{
	css_unit_value_t *s = ui_widget_get_style(w, css_key_visibility);
	if (s->is_valid && s->type == CSS_UNIT_STRING) {
		free(s->val_string);
		s->val_string = NULL;
	}
	ui_widget_set_style(w, css_key_visibility, strdup2(value), string);
	ui_widget_update_style(w);
}

void ui_widget_show(ui_widget_t* w)
{
	css_unit_value_t *s = ui_widget_get_style(w, css_key_display);

	if (s->is_valid && s->type == CSS_UNIT_STYLE &&
	    s->val_style == CSS_KEYWORD_NONE) {
		ui_widget_unset_style(w, css_key_display);
	} else if (!w->computed_style.visible) {
		s = ui_widget_get_matched_style(w, css_key_display);
		if (s->is_valid && s->type == CSS_UNIT_STYLE &&
		    s->val_style != CSS_KEYWORD_NONE) {
			ui_widget_set_style(w, css_key_display, s->val_style, style);
		} else {
			ui_widget_set_style(w, css_key_display, CSS_KEYWORD_BLOCK, style);
		}
	}
	ui_widget_update_style(w);
}

void ui_widget_hide(ui_widget_t* w)
{
	ui_widget_set_style(w, css_key_display, CSS_KEYWORD_NONE, style);
	ui_widget_update_style(w);
}

void ui_widget_set_position(ui_widget_t* w, css_keyword_value_t position)
{
	ui_widget_set_style(w, css_key_position, position, style);
	ui_widget_update_style(w);
}

void ui_widget_set_opacity(ui_widget_t* w, float opacity)
{
	ui_widget_set_style(w, css_key_opacity, opacity, scale);
	ui_widget_update_style(w);
}

void ui_widget_set_box_sizing(ui_widget_t* w, css_keyword_value_t sizing)
{
	ui_widget_set_style(w, css_key_box_sizing, sizing, style);
	ui_widget_update_style(w);
}

ui_widget_t* ui_widget_get_closest(ui_widget_t* w, const char *type)
{
	ui_widget_t* target;

	for (target = w; target; target = target->parent) {
		if (ui_check_widget_type(target, type)) {
			return target;
		}
	}
	return NULL;
}

static void ui_widget_collect_reference(ui_widget_t* w, void *arg)
{
	const char *ref = ui_widget_get_attribute_value(w, "ref");

	if (ref) {
		dict_add(arg, (void*)ref, w);
	}
}

dict_t *ui_widget_collect_references(ui_widget_t* w)
{
	dict_t *dict;
	static dict_type_t t;

	dict_init_string_key_type(&t);
	dict = dict_create(&t, NULL);
	ui_widget_each(w, ui_widget_collect_reference, dict);
	return dict;
}

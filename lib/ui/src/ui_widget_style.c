#include <string.h>
#include <LCUI/util.h>
#include "../include/ui.h"
#include "internal.h"

#define ARRAY_LEN(ARR) sizeof(ARR) / sizeof(ARR[0])

INLINE int ui_widget_compute_style_option(ui_widget_t* w, int key,
					  int default_value)
{
	if (!w->style->sheet[key].is_valid ||
	    w->style->sheet[key].type != CSS_UNIT_STYLE) {
		return default_value;
	}
	return w->style->sheet[key].style;
}

static float ui_widget_compute_metric_x(ui_widget_t* w, int key)
{
	css_unit_value_t *s = &w->style->sheet[key];

	if (s->type == CSS_UNIT_SCALE) {
		if (!w->parent) {
			return 0;
		}
		if (ui_widget_has_absolute_position(w)) {
			return w->parent->box.padding.width * s->scale;
		}
		return w->parent->box.content.width * s->scale;
	}
	return ui_compute(s->value, s->type);
}

static float ui_widget_compute_metric_y(ui_widget_t* w, int key)
{
	css_unit_value_t *s = &w->style->sheet[key];

	if (s->type == CSS_UNIT_SCALE) {
		if (!w->parent) {
			return 0;
		}
		if (ui_widget_has_absolute_position(w)) {
			return w->parent->box.padding.height * s->scale;
		}
		return w->parent->box.content.height * s->scale;
	}
	return ui_compute(s->value, s->type);
}

void ui_widget_compute_padding_style(ui_widget_t* w)
{
	if (!ui_widget_has_auto_style(w, css_key_padding_top)) {
		w->padding.top = ui_widget_compute_metric_y(w, css_key_padding_top);
	}
	if (!ui_widget_has_auto_style(w, css_key_padding_right)) {
		w->padding.right =
		    ui_widget_compute_metric_x(w, css_key_padding_right);
	}
	if (!ui_widget_has_auto_style(w, css_key_padding_bottom)) {
		w->padding.bottom =
		    ui_widget_compute_metric_y(w, css_key_padding_bottom);
	}
	if (!ui_widget_has_auto_style(w, css_key_padding_left)) {
		w->padding.left =
		    ui_widget_compute_metric_x(w, css_key_padding_left);
	}
}

void ui_widget_compute_margin_style(ui_widget_t* w)
{
	if (!ui_widget_has_auto_style(w, css_key_margin_top)) {
		w->margin.top = ui_widget_compute_metric_y(w, css_key_margin_top);
	}
	if (!ui_widget_has_auto_style(w, css_key_margin_right)) {
		w->margin.right =
		    ui_widget_compute_metric_x(w, css_key_margin_right);
	}
	if (!ui_widget_has_auto_style(w, css_key_margin_bottom)) {
		w->margin.bottom =
		    ui_widget_compute_metric_y(w, css_key_margin_bottom);
	}
	if (!ui_widget_has_auto_style(w, css_key_margin_left)) {
		w->margin.left = ui_widget_compute_metric_x(w, css_key_margin_left);
	}
}

void ui_widget_compute_properties(ui_widget_t* w)
{
	css_unit_value_t *s;
	ui_widget_style_t* style = &w->computed_style;

	s = &w->style->sheet[css_key_focusable];
	style->pointer_events =
	    ui_widget_compute_style_option(w, css_key_pointer_events, CSS_KEYWORD_INHERIT);
	if (s->is_valid && s->type == CSS_UNIT_BOOL && s->val_bool == 0) {
		style->focusable = FALSE;
	} else {
		style->focusable = TRUE;
	}
}

INLINE LCUI_BOOL ui_widget_has_fixed_width(ui_widget_t* w,
					   ui_layout_rule_t rule)
{
	return rule == UI_LAYOUT_RULE_FIXED ||
	       rule == UI_LAYOUT_RULE_FIXED_WIDTH ||
	       (w->parent &&
		w->parent->computed_style.width_sizing == UI_SIZING_RULE_FIXED);
}

INLINE LCUI_BOOL ui_widget_has_fixed_height(ui_widget_t* w,
					    ui_layout_rule_t rule)
{
	return rule == UI_LAYOUT_RULE_FIXED ||
	       rule == UI_LAYOUT_RULE_FIXED_HEIGHT ||
	       (w->parent && w->parent->computed_style.height_sizing ==
				 UI_SIZING_RULE_FIXED);
}

void ui_widget_compute_widget_limit_style(ui_widget_t* w, ui_layout_rule_t rule)
{
	ui_widget_style_t* style = &w->computed_style;

	style->max_width = -1;
	style->min_width = -1;
	while (ui_widget_check_style_valid(w, css_key_max_width)) {
		if (ui_widget_check_style_type(w, css_key_max_width, SCALE) &&
		    !ui_widget_has_fixed_width(w, rule)) {
			break;
		}
		style->max_width = ui_widget_compute_metric_x(w, css_key_max_width);
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			style->max_width +=
			    border_x(w) + padding_x(w);
		}
		break;
	}
	while (ui_widget_check_style_valid(w, css_key_min_width)) {
		if (ui_widget_check_style_type(w, css_key_min_width, SCALE) &&
		    !ui_widget_has_fixed_width(w, rule)) {
			break;
		}
		style->min_width = ui_widget_compute_metric_x(w, css_key_min_width);
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			style->min_width +=
			    border_x(w) + padding_x(w);
		}
		break;
	}
}

void ui_widget_compute_height_limit_style(ui_widget_t* w, ui_layout_rule_t rule)
{
	ui_widget_style_t* style = &w->computed_style;

	style->max_height = -1;
	style->min_height = -1;
	while (ui_widget_check_style_valid(w, css_key_max_height)) {
		if (ui_widget_check_style_type(w, css_key_max_height, SCALE) &&
		    !ui_widget_has_fixed_height(w, rule)) {
			break;
		}
		style->max_height =
		    ui_widget_compute_metric_y(w, css_key_max_height);
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			style->max_height +=
			    border_y(w) + padding_y(w);
		}
		break;
	}
	while (ui_widget_check_style_valid(w, css_key_min_height)) {
		if (ui_widget_check_style_type(w, css_key_min_height, SCALE) &&
		    !ui_widget_has_fixed_height(w, rule)) {
			break;
		}
		style->min_height =
		    ui_widget_compute_metric_y(w, css_key_min_height);
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			style->min_height +=
			    border_y(w) + padding_y(w);
		}
		break;
	}
}

void ui_widget_compute_width_style(ui_widget_t* w)
{
	ui_widget_style_t* style;

	style = &w->computed_style;
	do {
		if (ui_widget_has_auto_style(w, css_key_width)) {
			if (!w->parent ||
			    w->computed_style.display == CSS_KEYWORD_INLINE_BLOCK ||
			    ui_widget_has_absolute_position(w)) {
				style->width_sizing =
				    UI_SIZING_RULE_FIT_CONTENT;
				break;
			}
			// TODO: Improved sizing rule calculation
			// We should consider whether to use other rules when
			// the display value is inline-block, inline-flex or
			// flex.
			style->width_sizing = UI_SIZING_RULE_FILL;
			if (w->parent->computed_style.width_sizing ==
			    UI_SIZING_RULE_FIXED) {
				style->width_sizing = UI_SIZING_RULE_FIXED;
			}
			w->width =
			    w->parent->box.content.width - margin_x(w);
			break;
		}
		if (ui_widget_check_style_type(w, css_key_width, scale)) {
			if (!w->parent) {
				style->width_sizing =
				    UI_SIZING_RULE_FIT_CONTENT;
				break;
			}
			style->width_sizing = UI_SIZING_RULE_PERCENT;
			if (w->parent->computed_style.width_sizing ==
			    UI_SIZING_RULE_FIXED) {
				style->width_sizing = UI_SIZING_RULE_FIXED;
			}
			w->width = ui_widget_compute_metric_x(w, css_key_width);
		} else {
			w->width = ui_widget_compute_metric_x(w, css_key_width);
			style->width_sizing = UI_SIZING_RULE_FIXED;
		}
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			w->width += border_x(w) + padding_x(w);
		}
	} while (0);
	w->width = ui_widget_get_limited_width(w, w->width);
}

void ui_widget_compute_height_style(ui_widget_t* w)
{
	ui_widget_style_t* style;

	style = &w->computed_style;
	do {
		if (ui_widget_has_auto_style(w, css_key_height)) {
			style->height_sizing = UI_SIZING_RULE_FIT_CONTENT;
			break;
		}
		if (ui_widget_check_style_type(w, css_key_height, scale)) {
			if (!w->parent) {
				style->height_sizing =
				    UI_SIZING_RULE_FIT_CONTENT;
				break;
			}
			style->height_sizing = UI_SIZING_RULE_PERCENT;
			if (w->parent->computed_style.height_sizing ==
			    UI_SIZING_RULE_FIXED) {
				style->height_sizing = UI_SIZING_RULE_FIXED;
			}
			w->height = ui_widget_compute_metric_y(w, css_key_height);
		} else {
			w->height = ui_widget_compute_metric_y(w, css_key_height);
			style->height_sizing = UI_SIZING_RULE_FIXED;
		}
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			w->height += border_y(w) + padding_y(w);
		}
	} while (0);
	w->height = ui_widget_get_limited_height(w, w->height);
}

void ui_widget_compute_size_style(ui_widget_t* w)
{
	w->computed_style.box_sizing =
	    ui_widget_compute_style_option(w, css_key_box_sizing, CSS_KEYWORD_CONTENT_BOX);
	ui_widget_compute_widget_limit_style(w, UI_LAYOUT_RULE_MAX_CONTENT);
	ui_widget_compute_height_limit_style(w, UI_LAYOUT_RULE_MAX_CONTENT);
	ui_widget_compute_width_style(w);
	ui_widget_compute_height_style(w);
}

void ui_widget_compute_flex_basis_style(ui_widget_t* w)
{
	ui_flexbox_layout_style_t* flex = &w->computed_style.flex;

	if (w->parent &&
	    w->parent->computed_style.flex.direction == CSS_KEYWORD_COLUMN) {
		if (!ui_widget_has_auto_style(w, css_key_flex_basis)) {
			flex->basis =
			    ui_widget_compute_metric_y(w, css_key_flex_basis);
			flex->basis = ui_widget_get_limited_height(w, flex->basis);
			return;
		}
		if (w->computed_style.height_sizing == UI_SIZING_RULE_FIXED) {
			flex->basis = ui_widget_compute_metric_y(w, css_key_height);
			if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
				flex->basis = to_border_box_height(w, flex->basis);
			}
		} else {
			flex->basis = w->max_content_height;
			flex->basis = to_border_box_height(w, flex->basis);
		}
		return;
	}
	if (ui_widget_has_auto_style(w, css_key_flex_basis)) {
		if (w->computed_style.width_sizing == UI_SIZING_RULE_FIXED) {
			flex->basis = ui_widget_compute_metric_x(w, css_key_width);
			if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
				flex->basis = to_border_box_width(w, flex->basis);
			}
		} else {
			flex->basis = w->max_content_width;
			flex->basis = to_border_box_width(w, flex->basis);
		}
	} else {
		flex->basis = ui_widget_compute_metric_x(w, css_key_flex_basis);
		if (w->computed_style.box_sizing == CSS_KEYWORD_CONTENT_BOX) {
			flex->basis = to_border_box_width(w, flex->basis);
		}
	}
	flex->basis = ui_widget_get_limited_width(w, flex->basis);
}

void ui_widget_compute_visibility_style(ui_widget_t* w)
{
	css_unit_value_t *s = &w->style->sheet[css_key_visibility];

	if (w->computed_style.display == CSS_KEYWORD_NONE) {
		w->computed_style.visible = FALSE;
	} else if (s->is_valid && s->type == CSS_UNIT_STRING &&
		   strcmp(s->val_string, "hidden") == 0) {
		w->computed_style.visible = FALSE;
	} else {
		w->computed_style.visible = TRUE;
	}
}

void ui_widget_compute_display_style(ui_widget_t* w)
{
	css_unit_value_t *s = &w->style->sheet[css_key_display];
	ui_widget_style_t* style = &w->computed_style;

	if (s->is_valid && s->type == CSS_UNIT_STYLE) {
		style->display = s->style;
		if (style->display == CSS_KEYWORD_NONE) {
			w->computed_style.visible = FALSE;
		}
	} else {
		style->display = CSS_KEYWORD_BLOCK;
	}
	ui_widget_compute_visibility_style(w);
}

void ui_widget_compute_opacity_style(ui_widget_t* w)
{
	float opacity = 1.0;
	css_unit_value_t *s = &w->style->sheet[css_key_opacity];

	if (s->is_valid) {
		switch (s->type) {
		case CSS_UNIT_INT:
			opacity = 1.0f * s->val_int;
			break;
		case CSS_UNIT_SCALE:
			opacity = s->val_scale;
			break;
		default:
			opacity = 1.0f;
			break;
		}
		if (opacity > 1.0) {
			opacity = 1.0;
		} else if (opacity < 0.0) {
			opacity = 0.0;
		}
	}
	w->computed_style.opacity = opacity;
}

void ui_widget_compute_zindex_style(ui_widget_t* w)
{
	css_unit_value_t *s = &w->style->sheet[css_key_z_index];

	if (s->is_valid && s->type == CSS_UNIT_INT) {
		w->computed_style.z_index = s->val_int;
	} else {
		w->computed_style.z_index = 0;
	}
}

void ui_widget_compute_position_style(ui_widget_t* w)
{
	int position =
	    ui_widget_compute_style_option(w, css_key_position, CSS_KEYWORD_STATIC);
	int valign =
	    ui_widget_compute_style_option(w, css_key_vertical_align, CSS_KEYWORD_TOP);

	w->computed_style.vertical_align = valign;
	w->computed_style.left = ui_widget_compute_metric_x(w, css_key_left);
	w->computed_style.right = ui_widget_compute_metric_x(w, css_key_right);
	w->computed_style.top = ui_widget_compute_metric_y(w, css_key_top);
	w->computed_style.bottom = ui_widget_compute_metric_y(w, css_key_bottom);
	w->computed_style.position = position;
	ui_widget_compute_zindex_style(w);
}

void ui_widget_compute_flex_style(ui_widget_t* w)
{
	css_unit_value_t *s = w->style->sheet;
	ui_flexbox_layout_style_t* flex = &w->computed_style.flex;

	if (!ui_widget_has_valid_flexbox_style(w)) {
		return;
	}

	/* Reset to default value */

	flex->grow = 0;
	flex->shrink = 1;
	flex->wrap = CSS_KEYWORD_NOWRAP;
	flex->direction = CSS_KEYWORD_ROW;
	flex->justify_content = CSS_KEYWORD_NORMAL;
	flex->align_content = CSS_KEYWORD_NORMAL;
	flex->align_items = CSS_KEYWORD_NORMAL;

	/* Compute style */

	if (s[css_key_flex_grow].is_valid &&
	    s[css_key_flex_grow].type == CSS_UNIT_INT) {
		flex->grow = 1.f * s[css_key_flex_grow].val_int;
	}
	if (s[css_key_flex_shrink].is_valid &&
	    s[css_key_flex_shrink].type == CSS_UNIT_INT) {
		flex->shrink = 1.f * s[css_key_flex_shrink].val_int;
	}
	if (s[css_key_flex_wrap].is_valid &&
	    s[css_key_flex_wrap].type == CSS_UNIT_STYLE) {
		flex->wrap = s[css_key_flex_wrap].val_style;
	}
	if (s[css_key_flex_direction].is_valid &&
	    s[css_key_flex_direction].type == CSS_UNIT_STYLE) {
		flex->direction = s[css_key_flex_direction].val_style;
	}
	if (s[css_key_justify_content].is_valid &&
	    s[css_key_justify_content].type == CSS_UNIT_STYLE) {
		flex->justify_content = s[css_key_justify_content].val_style;
	}
	if (s[css_key_align_content].is_valid &&
	    s[css_key_align_content].type == CSS_UNIT_STYLE) {
		flex->align_content = s[css_key_align_content].val_style;
	}
	if (s[css_key_align_items].is_valid &&
	    s[css_key_align_items].type == CSS_UNIT_STYLE) {
		flex->align_items = s[css_key_align_items].val_style;
	}
	ui_widget_compute_flex_basis_style(w);
}

css_selector_node_t *ui_widget_create_selector_node(ui_widget_t* w)
{
	int i;
	css_selector_node_t *sn;

	sn = malloc(sizeof(css_selector_node_t));
	memset(sn, 0, sizeof(css_selector_node_t));
	if (w->id) {
		sn->id = strdup2(w->id);
	}
	if (w->type) {
		sn->type = strdup2(w->type);
	}
	for (i = 0; w->classes && w->classes[i]; ++i) {
		strlist_sorted_add(&sn->classes, w->classes[i]);
	}
	for (i = 0; w->status && w->status[i]; ++i) {
		strlist_sorted_add(&sn->status, w->status[i]);
	}
	css_selector_node_update(sn);
	return sn;
}

css_selector_t *ui_widget_create_selector(ui_widget_t* w)
{
	int i = 0;
	list_t list;
	css_selector_t *s;
	ui_widget_t* parent;
	list_node_t* node;

	s = css_selector_create(NULL);
	list_create(&list);
	for (parent = w; parent; parent = parent->parent) {
		if (parent->id || parent->type || parent->classes ||
		    parent->status) {
			list_append(&list, parent);
		}
	}
	if (list.length >= CSS_SELECTOR_MAX_DEPTH) {
		list_destroy(&list, NULL);
		css_selector_destroy(s);
		return NULL;
	}
	for (list_each_reverse(node, &list)) {
		parent = node->data;
		s->nodes[i] = ui_widget_create_selector_node(parent);
		s->rank += s->nodes[i]->rank;
		i += 1;
	}
	list_destroy(&list, NULL);
	s->nodes[i] = NULL;
	s->length = i;
	css_selector_update(s);
	return s;
}

size_t ui_widget_get_children_style_changes(ui_widget_t* w, int type,
					    const char* name)
{
	css_selector_t *s;
	list_t snames;
	list_node_t* node;

	size_t i, n, len;
	size_t count = 0;
	char ch, *str, **names = NULL;

	switch (type) {
	case 0:
		ch = '.';
		break;
	case 1:
		ch = ':';
		break;
	default:
		return 0;
	}
	list_create(&snames);
	s = ui_widget_create_selector(w);
	n = strsplit(name, " ", &names);
	/* 为分割出来的字符串加上前缀 */
	for (i = 0; i < n; ++i) {
		len = strlen(names[i]) + 2;
		str = malloc(len * sizeof(char));
		strcpy(str + 1, names[i]);
		str[0] = ch;
		free(names[i]);
		names[i] = str;
	}
	css_selector_node_get_name_list(s->nodes[s->length - 1], &snames);
	for (list_each(node, &snames)) {
		char* sname = node->data;
		/* 过滤掉不包含 name 中存在的名称 */
		for (i = 0; i < n; ++i) {
			char* p = strstr(sname, names[i]);
			if (p) {
				p += strlen(names[i]);
				switch (*p) {
				case 0:
				case ':':
				case '.':
					break;
				default:
					continue;
				}
				break;
			}
		}
		if (i < n) {
			count +=
			    css_query_selector_from_group(1, sname, s, NULL);
		}
	}
	css_selector_destroy(s);
	list_destroy(&snames, free);
	for (i = 0; names[i]; ++i) {
		free(names[i]);
	}
	free(names);
	return count;
}

void ui_widget_print_stylesheet(ui_widget_t* w)
{
	css_selector_t *s = ui_widget_create_selector(w);
	css_print_style_rules_by_selector(s);
	css_selector_destroy(s);
}

void ui_widget_update_children_style(ui_widget_t* w)
{
	list_node_t* node;
	w->update.for_children = TRUE;
	for (list_each(node, &w->children)) {
		ui_widget_update_style(node->data);
		ui_widget_update_children_style(node->data);
	}
}

void ui_widget_refresh_children_style(ui_widget_t* w)
{
	list_node_t* node;
	w->update.for_children = TRUE;
	for (list_each(node, &w->children)) {
		ui_widget_refresh_style(node->data);
		ui_widget_refresh_children_style(node->data);
	}
}

static void ui_widget_on_set_style(int key, css_unit_value_t *style, void* arg)
{
	ui_widget_t* w = arg;
	css_unit_value_t *s = ui_widget_get_style(w, key);

	if (style->is_valid) {
		css_unit_value_destroy(s);
		*s = *style;
		ui_widget_add_task_by_style(w, key);
	} else {
		ui_widget_unset_style(w, key);
	}
}

void ui_widget_set_style_string(ui_widget_t* w, const char* name,
				const char* value)
{
	css_style_parser_t parser;
	css_property_parser_t *prop_parser;

	css_style_parser_init(&parser, NULL);
	parser.style_handler = ui_widget_on_set_style;
	parser.style_handler_arg = w;
	prop_parser = css_get_property_parser(name);
	prop_parser->parse(&parser, value);
	css_style_parser_destroy(&parser);
	ui_widget_update_style(w);
}

void ui_widget_add_task_by_style(ui_widget_t* w, int key)
{
	size_t i;
	struct {
		int start, end;
		ui_task_type_t task;
		LCUI_BOOL is_valid;
	} task_status[] = {
		{ css_key_visibility, css_key_visibility, UI_TASK_VISIBLE,
		  TRUE },
		{ css_key_display, css_key_display, UI_TASK_DISPLAY, TRUE },
		{ css_key_flex_style_start, css_key_flex_style_end, UI_TASK_FLEX,
		  TRUE },
		{ css_key_opacity, css_key_opacity, UI_TASK_OPACITY, TRUE },
		{ css_key_z_index, css_key_z_index, UI_TASK_ZINDEX, TRUE },
		{ css_key_width, css_key_height, UI_TASK_RESIZE, TRUE },
		{ css_key_min_width, css_key_max_height, UI_TASK_RESIZE, TRUE },
		{ css_key_padding_start, css_key_padding_end, UI_TASK_RESIZE,
		  TRUE },
		{ css_key_box_sizing, css_key_box_sizing, UI_TASK_RESIZE, TRUE },
		{ css_key_margin_start, css_key_margin_end, UI_TASK_MARGIN,
		  TRUE },
		{ css_key_position_start, css_key_position_end, UI_TASK_POSITION,
		  TRUE },
		{ css_key_vertical_align, css_key_vertical_align,
		  UI_TASK_POSITION, TRUE },
		{ css_key_border_start, css_key_border_end, UI_TASK_BORDER,
		  TRUE },
		{ css_key_background_start, css_key_background_end,
		  UI_TASK_BACKGROUND, TRUE },
		{ css_key_box_shadow_start, css_key_box_shadow_end,
		  UI_TASK_SHADOW, TRUE },
		{ css_key_pointer_events, css_key_focusable, UI_TASK_PROPS,
		  TRUE }
	};

	for (i = 0; i < ARRAY_LEN(task_status); ++i) {
		if (key >= task_status[i].start && key <= task_status[i].end) {
			if (!task_status[i].is_valid) {
				break;
			}
			task_status[i].is_valid = FALSE;
			ui_widget_add_task(w, task_status[i].task);
		}
	}
}

void ui_widget_force_update_style(ui_widget_t* w)
{
	css_style_declaration_clear(w->style);
	if (w->custom_style) {
		css_style_declaration_merge_properties(w->style, w->custom_style);
	}
	css_style_declaration_merge(w->style, w->matched_style);
	if (w->proto && w->proto->update &&
	    w->style->length > STYLE_KEY_TOTAL) {
		/* 扩展部分的样式交给该部件自己处理 */
		w->proto->update(w);
	}
}

void ui_widget_force_refresh_style(ui_widget_t* w)
{
	ui_widget_force_update_style(w);
	/* 刷新该部件的相关数据 */
	if (w->proto && w->proto->refresh) {
		w->proto->refresh(w);
	}
}

void ui_widget_destroy_style(ui_widget_t* w)
{
	w->matched_style = NULL;
	if (w->custom_style) {
		css_style_properties_destroy(w->custom_style);
	}
	css_style_declaration_destroy(w->style);
}

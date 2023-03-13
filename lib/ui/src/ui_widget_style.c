#include <string.h>
#include <css.h>
#include "internal.h"


css_selector_node_t *ui_widget_create_selector_node(ui_widget_t *w)
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

css_selector_t *ui_widget_create_selector(ui_widget_t *w)
{
	int i = 0;
	list_t list;
	css_selector_t *s;
	ui_widget_t *parent;
	list_node_t *node;

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

size_t ui_widget_get_children_style_changes(ui_widget_t *w, int type,
					    const char *name)
{
	css_selector_t *s;
	list_t snames;
	list_node_t *node;

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
		char *sname = node->data;
		/* 过滤掉不包含 name 中存在的名称 */
		for (i = 0; i < n; ++i) {
			char *p = strstr(sname, names[i]);
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

void ui_widget_update_children_style(ui_widget_t *w)
{
	list_node_t *node;
	w->update.for_children = TRUE;
	for (list_each(node, &w->children)) {
		ui_widget_update_style(node->data);
		ui_widget_update_children_style(node->data);
	}
}

void ui_widget_refresh_children_style(ui_widget_t *w)
{
	list_node_t *node;
	w->update.for_children = TRUE;
	for (list_each(node, &w->children)) {
		ui_widget_refresh_style(node->data);
		ui_widget_refresh_children_style(node->data);
	}
}

void ui_widget_get_style(ui_widget_t *w, int key, css_style_value_t *value)
{
	const css_prop_t *prop;

	if (w->custom_style) {
		prop = css_style_decl_find(w->custom_style, key);
		if (prop) {
			*value = prop->style;
			return;
		}
	}
	value->type = CSS_NO_VALUE;
}

void ui_widget_set_style(ui_widget_t *w, int key,
			 const css_style_value_t *value)
{
	if (w->custom_style) {
		css_style_decl_set(w->custom_style, key, value);
	} else {
		w->custom_style = css_style_decl_create();
		css_style_decl_add(w->custom_style, key, value);
	}
	ui_widget_update_style(w);
}

int ui_widget_unset_style(ui_widget_t *w, int key)
{
	if (!w->custom_style) {
		return -1;
	}
	ui_widget_update_style(w);
	return css_style_decl_remove(w->custom_style, key);
}

int ui_widget_set_style_string(ui_widget_t *w, const char *property,
			       const char *css_text)
{
	const css_propdef_t *propdef;
	css_style_value_t value;

	propdef = css_get_propdef_by_name(property);
	if (!propdef) {
		return -1;
	}
	if (css_parse_value(propdef->valdef, css_text, &value) <= 0) {
		return -2;
	}
	ui_widget_set_style(w, propdef->key, &value);
	return 0;
}

void ui_widget_set_style_unit_value(ui_widget_t *w, int key,
				    css_numeric_value_t value, css_unit_t unit)
{
	css_style_value_t v;

	v.unit_value.value = value;
	v.unit_value.unit = unit;
	v.type = CSS_UNIT_VALUE;
	ui_widget_set_style(w, key, &v);
}

void ui_widget_set_style_color_value(ui_widget_t *w, int key,
				     css_color_value_t value)
{
	css_style_value_t v;

	v.type = CSS_COLOR_VALUE;
	v.color_value = value;
	ui_widget_set_style(w, key, &v);
}

void ui_widget_set_style_numeric_value(ui_widget_t *w, int key,
				       css_numeric_value_t value)
{
	css_style_value_t v;

	v.type = CSS_NUMERIC_VALUE;
	v.numeric_value = value;
	ui_widget_set_style(w, key, &v);
}

void ui_widget_set_style_keyword_value(ui_widget_t *w, int key,
				       css_keyword_value_t value)
{
	css_style_value_t v;

	v.keyword_value = value;
	v.type = CSS_KEYWORD_VALUE;
	ui_widget_set_style(w, key, &v);
}

static void ui_widget_on_background_image_load(ui_image_t *image, void *data)
{
	float scale;
	ui_widget_t *w = data;
	css_computed_style_t *s = &w->computed_style;
	pd_canvas_t *img = (pd_canvas_t *)image;

	ui_widget_mark_dirty_rect(w, NULL, UI_BOX_TYPE_BORDER_BOX);
	if (!image) {
		return;
	}
	if (IS_CSS_PERCENTAGE(s, background_position_x)) {
		CSS_SET_FIXED_LENGTH(
		    s, background_position_x,
		    w->padding_box.width -
			img->width / 2.f * s->background_position_x);
	}
	if (IS_CSS_PERCENTAGE(s, background_position_y)) {
		CSS_SET_FIXED_LENGTH(
		    s, background_position_y,
		    w->padding_box.height -
			img->height / 2.f * s->background_position_y);
	}
	switch (s->type_bits.background_width) {
	case CSS_BACKGROUND_SIZE_CONVER:
		scale = 1.f * w->padding_box.width / img->width;
		scale = y_max(scale, 1.f * w->padding_box.height / img->height);
		CSS_SET_FIXED_LENGTH(s, background_width, scale * img->width);
		CSS_SET_FIXED_LENGTH(s, background_height, scale * img->height);
		break;
	case CSS_BACKGROUND_SIZE_CONTAIN:
		scale = 1.f * w->padding_box.width / img->width;
		scale = y_min(scale, 1.f * w->padding_box.height / img->height);
		CSS_SET_FIXED_LENGTH(s, background_width, scale * img->width);
		CSS_SET_FIXED_LENGTH(s, background_height, scale * img->height);
		break;
	case CSS_BACKGROUND_SIZE_SET:
		if (IS_CSS_PERCENTAGE(s, background_width)) {
			CSS_SET_FIXED_LENGTH(s, background_width,
					     img->width * w->padding_box.width);
		}
		break;
	default:
		break;
	}
	switch (s->type_bits.background_height) {
	case CSS_BACKGROUND_SIZE_SET:
		if (IS_CSS_PERCENTAGE(s, background_height)) {
			CSS_SET_FIXED_LENGTH(
			    s, background_height,
			    img->height * w->padding_box.height);
		}
		break;
	default:
		break;
	}
	if (s->type_bits.background_width == CSS_BACKGROUND_SIZE_AUTO) {
		if (s->type_bits.background_height ==
		    CSS_BACKGROUND_SIZE_AUTO) {
			CSS_SET_FIXED_LENGTH(s, background_height,
					     1.f * img->height);
		}
		CSS_SET_FIXED_LENGTH(
		    s, background_width,
		    s->background_height * img->width / img->height);
	} else if (s->type_bits.background_height == CSS_BACKGROUND_SIZE_SET) {
		CSS_SET_FIXED_LENGTH(
		    s, background_height,
		    s->background_width * img->height / img->width);
	}
}

void ui_widget_compute_style(ui_widget_t *w)
{
	if (w->parent) {
		css_compute_absolute_values(&w->parent->computed_style,
					    &w->computed_style, &ui_metrics);
	}
}

void ui_widget_force_update_style(ui_widget_t *w)
{
	css_computed_style_t *s = &w->specified_style;
	css_style_decl_t *style;

	DEBUG_MSG("begin, %s\n", w->classes ? w->classes[0] : "null");
	if (s->background_image &&
	    s->type_bits.background_image == CSS_BACKGROUND_IMAGE_DATA) {
		ui_image_remove_ref((ui_image_t *)s->background_image);
	}
	css_computed_style_destroy(s);
	if (w->custom_style) {
		style = css_style_decl_create();
		css_style_decl_merge(style, w->custom_style);
		css_style_decl_merge(style, w->matched_style);
		css_cascade_style(style, s);
		css_style_decl_destroy(style);
	} else {
		css_cascade_style(w->matched_style, s);
	}
	w->computed_style = *s;
	ui_widget_compute_style(w);
	if (s->background_image) {
		s->type_bits.background_image = CSS_BACKGROUND_IMAGE_DATA;
		s->background_image =
		    (css_image_value_t)ui_load_image(s->background_image);
		ui_image_on_event((ui_image_t *)s->background_image,
				  ui_widget_on_background_image_load, w);
		ui_image_add_ref((ui_image_t *)s->background_image);
	}
	if (w->proto && w->proto->update) {
		/* 扩展部分的样式交给该部件自己处理 */
		w->proto->update(w);
	}
	DEBUG_MSG("end, %s\n", w->classes ? w->classes[0] : "null");
}

void ui_widget_force_refresh_style(ui_widget_t *w)
{
	ui_widget_force_update_style(w);
	/* 刷新该部件的相关数据 */
	if (w->proto && w->proto->refresh) {
		w->proto->refresh(w);
	}
}

void ui_widget_destroy_style(ui_widget_t *w)
{
	w->matched_style = NULL;
	if (w->computed_style.type_bits.background_image ==
	    CSS_BACKGROUND_IMAGE_DATA) {
		ui_image_remove_ref(
		    (ui_image_t *)w->computed_style.background_image);
	}
	if (w->custom_style) {
		css_style_decl_destroy(w->custom_style);
	}
	css_computed_style_destroy(&w->specified_style);
}

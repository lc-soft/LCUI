/*
 * lib/ui/src/ui_debug.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <css.h>
#include <ui.h>

int ui_debug_msg_indent = 0;

LIBUI_INLINE const char *get_css_unit_str(css_unit_t unit)
{
	switch (unit) {
	case CSS_UNIT_PERCENT:
		return "%";
	case CSS_UNIT_DIP:
		return "dip";
	case CSS_UNIT_PT:
		return "pt";
	case CSS_UNIT_SP:
		return "sp";
	case CSS_UNIT_PX:
		return "px";
	default:
		break;
	}
	return "";
}

void ui_widget_to_string(ui_widget_t *w, char *str)
{
	css_selector_node_t *node = ui_widget_create_selector_node(w);
	if (node->status) {
		strlist_free(node->status);
		node->status = NULL;
		css_selector_node_update(node);
	}
	strcpy(str, node->fullname ? node->fullname : "<widget>");
	css_selector_node_destroy(node);
}

void ui_widget_size_to_string(ui_widget_t *w, char str[40])
{
	char width_str[16] = "auto";
	char height_str[16] = "auto";

	switch (w->computed_style.type_bits.width) {
	case CSS_WIDTH_FIT_CONTENT:
		strcpy(width_str, "fit-content");
		break;
	case CSS_WIDTH_SET:
		snprintf(width_str, 16, "%g%s", w->computed_style.width,
			 get_css_unit_str(w->computed_style.unit_bits.width));
		break;
	default:
		break;
	}
	switch (w->computed_style.type_bits.height) {
	case CSS_HEIGHT_SET:
		snprintf(height_str, 16, "%g%s", w->computed_style.height,
			 get_css_unit_str(w->computed_style.unit_bits.height));
		break;
	}
	snprintf(str, 40, "(%s, %s)", width_str, height_str);
}

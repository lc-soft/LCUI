/*
 * lib/ui/src/ui_widget_classes.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <ui/base.h>
#include "ui_widget_classes.h"
#include "ui_widget_style.h"

static void ui_widget_refresh_children_by_classes(ui_widget_t* w)
{
	list_node_t *node;

	if (w->extra && w->extra->rules.ignore_classes_change) {
		return;
	}
	ui_widget_request_refresh_style(w);
	for (list_each(node, &w->children)) {
		ui_widget_refresh_children_by_classes(node->data);
	}
}

static int ui_widget_handle_classes_change(ui_widget_t* w, const char *name)
{
	ui_widget_request_refresh_style(w);
	if (w->extra && w->extra->rules.ignore_classes_change) {
		return 0;
	}
	/* If widget is not ready, indicate that the style of the children has
	 * been marked needs to be refreshed */
	if (w->state < UI_WIDGET_STATE_READY || w->state == UI_WIDGET_STATE_DELETED) {
		return 1;
	}
	if (ui_widget_get_children_style_changes(w, 0, name) > 0) {
		ui_widget_refresh_children_by_classes(w);
		return 1;
	}
	return 0;
}

int ui_widget_add_class(ui_widget_t* w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		return 1;
	}
	if (strlist_add(&w->classes, class_name) <= 0) {
		return 0;
	}
	return ui_widget_handle_classes_change(w, class_name);
}

bool ui_widget_has_class(ui_widget_t* w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		return true;
	}
	return false;
}

int ui_widget_remove_class(ui_widget_t* w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		ui_widget_handle_classes_change(w, class_name);
		strlist_remove(&w->classes, class_name);
		return 1;
	}
	return 0;
}

void ui_widget_destroy_classes(ui_widget_t* w)
{
	if (w->classes) {
		strlist_free(w->classes);
	}
	w->classes = NULL;
}

#include <ui/base.h>
#include "ui_widget_status.h"
#include "ui_widget_style.h"

static void ui_widget_refresh_children_by_status(ui_widget_t* w)
{
	list_node_t *node;

	if (w->extra && w->extra->rules.ignore_status_change) {
		return;
	}
	ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
	for (list_each(node, &w->children)) {
		ui_widget_refresh_children_by_status(node->data);
	}
}

static int ui_wdiget_handle_status_change(ui_widget_t* w, const char *name)
{
	ui_widget_refresh_style(w);
	if (w->state < UI_WIDGET_STATE_READY || w->state == UI_WIDGET_STATE_DELETED) {
		return 1;
	}
	if (w->extra && w->extra->rules.ignore_status_change) {
		return 0;
	}
	if (ui_widget_get_children_style_changes(w, 1, name) > 0) {
		ui_widget_refresh_children_by_status(w);
		return 1;
	}
	return 0;
}

int ui_widget_add_status(ui_widget_t* w, const char *status_name)
{
	if (strlist_has(w->status, status_name)) {
		return 0;
	}
	if (strlist_add(&w->status, status_name) <= 0) {
		return 0;
	}
	return ui_wdiget_handle_status_change(w, status_name);
}

bool ui_widget_has_status(ui_widget_t* w, const char *status_name)
{
	if (strlist_has(w->status, status_name)) {
		return true;
	}
	return false;
}

int ui_widget_remove_status(ui_widget_t* w, const char *status_name)
{
	if (strlist_has(w->status, status_name)) {
		ui_wdiget_handle_status_change(w, status_name);
		strlist_remove(&w->status, status_name);
		return 1;
	}
	return 0;
}

void ui_widget_update_status(ui_widget_t* widget)
{
	ui_widget_t* child;

	if (!widget->parent) {
		return;
	}
	if (widget->index == widget->parent->children.length - 1) {
		ui_widget_add_status(widget, "last-child");
		child = ui_widget_prev(widget);
		if (child) {
			ui_widget_remove_status(child, "last-child");
		}
	}
	if (widget->index == 0) {
		ui_widget_add_status(widget, "first-child");
		child = ui_widget_next(widget);
		if (child) {
			ui_widget_remove_status(child, "first-child");
		}
	}
}

void ui_widget_set_disabled(ui_widget_t* w, bool disabled)
{
	w->disabled = disabled;
	if (w->disabled) {
		ui_widget_add_status(w, "disabled");
	} else {
		ui_widget_remove_status(w, "disabled");
	}
}

void ui_widget_destroy_status(ui_widget_t* w)
{
	if (w->status) {
		strlist_free(w->status);
	}
	w->status = NULL;
}

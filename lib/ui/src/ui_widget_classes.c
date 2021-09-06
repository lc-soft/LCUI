#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

static void ui_widget_refresh_children_by_classes(ui_widget_t* w)
{
	LinkedListNode *node;

	if (w->rules && w->rules->ignore_classes_change) {
		return;
	}
	ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
	for (LinkedList_Each(node, &w->children)) {
		ui_widget_refresh_children_by_classes(node->data);
	}
}

static int ui_widget_handle_classes_change(ui_widget_t* w, const char *name)
{
	ui_widget_refresh_style(w);
	if (w->rules && w->rules->ignore_classes_change) {
		return 0;
	}
	/* If widget is not ready, indicate that the style of the children has
	 * been marked needs to be refreshed */
	if (w->state < LCUI_WSTATE_READY || w->state == LCUI_WSTATE_DELETED) {
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

LCUI_BOOL ui_widget_has_class(ui_widget_t* w, const char *class_name)
{
	if (strlist_has(w->classes, class_name)) {
		return TRUE;
	}
	return FALSE;
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

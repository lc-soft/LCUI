#include <LCUI.h>
#include "../include/ui.h"
#include "internal.h"

static list_t ui_trash = { 0 };

size_t ui_trash_clear(void)
{
	size_t count;
	list_node_t* node;

	node = ui_trash.head.next;
	count = ui_trash.length;
	while (node) {
		list_node_t* next = node->next;
		list_unlink(&ui_trash, node);
		ui_widget_destroy(node->data);
		node = next;
	}
	return count;
}

void ui_trash_add(ui_widget_t* w)
{
	w->state = UI_WIDGET_STATE_DELETED;
	w->parent = NULL;
	list_append_node(&ui_trash, &w->node);
}

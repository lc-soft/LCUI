
#include "../include/ui.h"
#include "private.h"

static LinkedList ui_trash;

size_t ui_trash_clear(void)
{
	size_t count;
	LinkedListNode *node;

	node = trash.head.next;
	count = trash.length;
	while (node) {
		LinkedListNode *next = node->next;
		LinkedList_Unlink(&LCUIWidget.trash, node);
		ui_widget_destroy(node->data);
		node = next;
	}
	return count;
}

void ui_trash_add(ui_widget_t *w)
{
	w->state = LCUI_WSTATE_DELETED;
	if (ui_widget_unlink(w) != 0) {
		return;
	}
	LinkedList_AppendNode(&LCUIWidget.trash, &w->node);
	ui_widget_post_surface_event(w, UI_EVENT_UNLINK, TRUE);
}

void ui_widget_init(ui_widget_t* w)
{
	ZEROSET(widget, ui_widget_t*);
	w->state = LCUI_WSTATE_CREATED;
	w->style = StyleSheet();
	w->computed_style.opacity = 1.0;
	w->computed_style.visible = TRUE;
	w->computed_style.focusable = FALSE;
	w->computed_style.display = SV_BLOCK;
	w->computed_style.position = SV_STATIC;
	w->computed_style.pointer_events = SV_INHERIT;
	w->computed_style.box_sizing = SV_CONTENT_BOX;
	LinkedList_Init(&w->children);
	LinkedList_Init(&w->children_show);
	w->node.data = w;
	w->node_show.data = w;
	w->node.next = w->node.prev = NULL;
	w->node_show.next = w->node_show.prev = NULL;
	ui_widget_background_init(w);
}

void ui_widget_destroy(ui_widget_t* w)
{
	if (w->parent) {
		Widget_AddTask(w->parent, UI_WIDGET_TASK_REFLOW);
		ui_widget_unlink(w);
	}
	ui_widget_destroy_background(w);
	ui_widget_destroy_listeners(w);
	ui_widget_destroy_children(w);
	ui_widget_destroy_prototype(w);
	if (w->title) {
		free(w->title);
		w->title = NULL;
	}
	ui_widget_destroy_id(w);
	ui_widget_deleteStyleSheets(w);
	ui_widget_destroy_attributes(w);
	ui_widget_destroy_classes(w);
	ui_widget_deleteStatus(w);
	Widget_SetRules(w, NULL);
	free(w);
}

ui_widget_t* ui_create_widget(const char* type)
{
	ui_widget_t* widget = malloc(sizeof(ui_widget_t));

	ui_widget_init(widget);
	widget->proto = ui_get_widget_prototype(type);
	if (widget->proto->name) {
		widget->type = widget->proto->name;
	} else if (type) {
		widget->type = strdup2(type);
	}
	widget->proto->init(widget);
	Widget_AddTask(widget, UI_WIDGET_TASK_REFRESH_STYLE);
	return widget;
}

void ui_widget_remove(ui_widget_t* w)
{
	ui_widget_t* root = w;

	assert(w->state != LCUI_WSTATE_DELETED);
	while (root->parent) {
		root = root->parent;
	}
	/* If this widget is not mounted in the root widget tree */
	if (root != ui_root()) {
		w->state = LCUI_WSTATE_DELETED;
		ui_widget_destroy(w);
		return;
	}
	if (w->parent) {
		ui_widget_t* child;
		LinkedListNode* node;

		/* Update the index of the siblings behind it */
		node = w->node.next;
		while (node) {
			child = node->data;
			child->index -= 1;
			node = node->next;
		}
		if (w->computed_style.position != SV_ABSOLUTE) {
			Widget_AddTask(w->parent, UI_WIDGET_TASK_REFLOW);
		}
		Widget_InvalidateArea(w->parent, &w->box.canvas,
				      SV_CONTENT_BOX);
		ui_trash_add(w);
	}
}

void ui_widget_add_state(ui_widget_t* w, ui_widget_state_t state)
{
	/* 如果部件还处于未准备完毕的状态 */
	if (w->state < LCUI_WSTATE_READY) {
		w->state |= state;
		/* 如果部件已经准备完毕则触发 ready 事件 */
		if (w->state == LCUI_WSTATE_READY) {
			ui_event_t e = { 0 };
			e.type = UI_EVENT_READY;
			e.cancel_bubble = TRUE;
			ui_widget_emit_event(w, &e, NULL);
			w->state = LCUI_WSTATE_NORMAL;
		}
	}
}

void ui_widget_set_title(ui_widget_t* w, const wchar_t* title)
{
	size_t len;
	wchar_t *new_title, *old_title;

	len = wcslen(title) + 1;
	new_title = (wchar_t*)malloc(sizeof(wchar_t) * len);
	if (!new_title) {
		return;
	}
	wcsncpy(new_title, title, len);
	old_title = w->title;
	w->title = new_title;
	if (old_title) {
		free(old_title);
	}
	Widget_AddTask(w, UI_WIDGET_TASK_TITLE);
}

void ui_widget_set_text(ui_widget_t* w, const char *text)
{
	if (w->proto && w->proto->settext) {
		w->proto->settext(w, text);
	}
}

void ui_widget_bind_property(ui_widget_t* w, const char *name, LCUI_Object value)
{
	if (w->proto && w->proto->bindprop) {
		w->proto->bindprop(w, name, value);
	}
}

void ui_widget_empty(ui_widget_t* w)
{
	ui_widget_t* root = w;
	ui_widget_t* child;
	LinkedListNode *node;
	ui_event_t ev;

	while (root->parent) {
		root = root->parent;
	}
	if (root != LCUIWidget.root) {
		ui_widget_destroy_children(w);
		return;
	}
	ui_event_init(&ev, "unlink");
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;
		ui_widget_emit_event(child, &ev, NULL);
		if (child->parent == root) {
			ui_widget_post_surface_event(child, UI_EVENT_UNLINK,
						TRUE);
		}
		child->state = LCUI_WSTATE_DELETED;
		child->parent = NULL;
	}
	LinkedList_ClearData(&w->children_show, NULL);
	LinkedList_Concat(&LCUIWidget.trash, &w->children);
	Widget_InvalidateArea(w, NULL, SV_GRAPH_BOX);
	Widget_UpdateStyle(w, TRUE);
}

void ui_widget_get_offset(ui_widget_t* w, ui_widget_t* parent, float* offset_x,
			  float* offset_y)
{
	float x = 0, y = 0;
	while (w != parent) {
		x += w->box.border.x;
		y += w->box.border.y;
		w = w->parent;
		if (w) {
			x += w->box.padding.x - w->box.border.x;
			y += w->box.padding.y - w->box.border.y;
		} else {
			break;
		}
	}
	*offset_x = x;
	*offset_y = y;
}

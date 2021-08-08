#include <errno.h>
#include "../include/ui.h"

typedef struct ui_mutation_connection_t {
	ui_widget_t *widget;
	ui_mutation_observer_init_t options;
	list_node_t node;
} ui_mutation_connection_t;

static list_t ui_observers = { 0 };

struct ui_mutation_observer_t {
	list_node_t node;

	/** list_t<ui_mutation_connection_t> */
	list_t connections;

	/** list_t<ui_mutation_record_t> */
	list_t records;

	ui_mutation_observer_callback_t callback;
	void *data;
};

ui_mutation_record_t *ui_mutation_record_create(ui_widget_t *widget,
						ui_mutation_record_type_t type)
{
	ui_mutation_record_t *mutation;

	mutation = malloc(sizeof(ui_mutation_record_t));
	if (!mutation) {
		return NULL;
	}
	mutation->type = type;
	mutation->target = widget;
	mutation->attribute_name = NULL;
	mutation->property_name = NULL;
	list_create(&mutation->added_widgets);
	list_create(&mutation->removed_widgets);
	return mutation;
}

ui_mutation_record_t *ui_mutation_record_duplicate(ui_mutation_record_t *source)
{
	list_node_t *node;
	ui_mutation_record_t *mutation;

	mutation = ui_mutation_record_create(source->target, source->type);
	if (source->attribute_name) {
		mutation->attribute_name = strdup2(source->attribute_name);
	}
	if (source->property_name) {
		mutation->property_name = strdup2(source->property_name);
	}
	for (list_each(node, &source->added_widgets)) {
		list_append(&mutation->added_widgets, node->data);
	}
	for (list_each(node, &source->removed_widgets)) {
		list_append(&mutation->removed_widgets, node->data);
	}
	return mutation;
}

void ui_mutation_record_destroy(ui_mutation_record_t *mutation)
{
	mutation->target = NULL;
	if (mutation->attribute_name) {
		free(mutation->attribute_name);
		mutation->attribute_name = NULL;
	}
	if (mutation->property_name) {
		free(mutation->property_name);
		mutation->property_name = NULL;
	}
	list_destroy(&mutation->added_widgets, NULL);
	list_destroy(&mutation->removed_widgets, NULL);
	free(mutation);
}

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver
 */
ui_mutation_observer_t *ui_mutation_observer_create(
    ui_mutation_observer_callback_t callback, void *data)
{
	ui_mutation_observer_t *observer;

	observer = malloc(sizeof(ui_mutation_observer_t));
	if (!observer) {
		return NULL;
	}
	observer->callback = callback;
	observer->data = data;
	observer->node.data = observer;
	list_create(&observer->records);
	list_create(&observer->connections);
	list_append_node(&ui_observers, &observer->node);
	return observer;
}

static void ui_mutation_observer_on_widget_destroy(ui_widget_t *w,
						   ui_event_t *e, void *arg)
{
	list_node_t *node, *prev;
	ui_mutation_connection_t *conn;
	ui_mutation_observer_t *observer = e->data;

	for (list_each(node, &observer->connections)) {
		prev = node->prev;
		conn = node->data;
		if (conn->widget == w) {
			conn->widget->extra->observer = NULL;
			conn->widget = NULL;
			list_unlink(&observer->connections, node);
			free(conn);
			node = prev;
		}
	}
}

void ui_mutation_observer_add_record(ui_mutation_observer_t *observer, ui_mutation_record_t *record)
{
	list_append(&observer->records, ui_mutation_record_duplicate(record));
}

int ui_mutation_observer_observe(ui_mutation_observer_t *observer,
				 ui_widget_t *w,
				 ui_mutation_observer_init_t options)
{
	ui_mutation_connection_t *conn;

	conn = malloc(sizeof(ui_mutation_connection_t));
	if (!conn) {
		return -ENOMEM;
	}
	conn->widget = w;
	conn->options = options;
	conn->node.data = conn;
	list_append_node(&observer->connections, &conn->node);
	ui_widget_use_extra_data(w)->observer = observer;
	ui_widget_use_extra_data(w)->observer_options = options;
	ui_widget_on(w, "destroy", ui_mutation_observer_on_widget_destroy,
		     observer, NULL);
	return 0;
}

void ui_mutation_observer_disconnect(ui_mutation_observer_t *observer)
{
	list_node_t *node;
	ui_mutation_connection_t *conn;

	for (list_each(node, &observer->connections)) {
		conn = node->data;
		if (conn->widget->extra) {
			conn->widget->extra->observer = NULL;
		}
	}
	list_destroy_without_node(&observer->connections, free);
}

static void ui_on_destroy_mutation_record(void *arg)
{
	ui_mutation_record_destroy(arg);
}

void ui_mutation_observer_destroy(ui_mutation_observer_t *observer)
{
	list_unlink(&ui_observers, &observer->node);
	list_destroy_without_node(&observer->connections, free);
	list_destroy(&observer->records, ui_on_destroy_mutation_record);
	free(observer);
}

void ui_process_mutation_observers(void)
{
	list_t records;
	list_node_t *node;
	ui_mutation_observer_t *observer;

	list_create(&records);
	for (list_each(node, &ui_observers)) {
		observer = node->data;
		if (observer->records.length > 0) {
			list_concat(&records, &observer->records);
			observer->callback(&records, observer, observer->data);
			list_destroy(&records, ui_on_destroy_mutation_record);
		}
	}
}

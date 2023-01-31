#include "./internal.h"

LCUI_BOOL ui_widget_has_observer(ui_widget_t *widget,
				 ui_mutation_record_type_t type)
{
	ui_widget_t *parent;
	list_node_t *node;
	ui_mutation_connection_t *conn;

	for (parent = widget; parent; parent = parent->parent) {
		if (!parent->extra ||
		    parent->extra->observer_connections.length < 1) {
			continue;
		}
		for (list_each(node, &parent->extra->observer_connections)) {
			conn = node->data;
			if (!conn->options.subtree && widget != parent) {
				continue;
			}
			switch (type) {
			case UI_MUTATION_RECORD_TYPE_CHILD_LIST:
				if (conn->options.child_list) {
					return TRUE;
				}
				break;
			case UI_MUTATION_RECORD_TYPE_PROPERTIES:
				if (conn->options.properties) {
					return TRUE;
				}
				break;
			case UI_MUTATION_RECORD_TYPE_ATTRIBUTES:
				if (conn->options.attributes) {
					return TRUE;
				}
				break;
			default:
				break;
			}
		}
	}
	return FALSE;
}

int ui_widget_add_mutation_recrod(ui_widget_t *widget,
				  ui_mutation_record_t *record)
{
	int count = 0;
	ui_widget_t *parent;
	list_node_t *node;
	ui_mutation_connection_t *conn;

	for (parent = widget; parent; parent = parent->parent) {
		if (!parent->extra ||
		    parent->extra->observer_connections.length < 1) {
			continue;
		}
		for (list_each(node, &parent->extra->observer_connections)) {
			conn = node->data;
			if (!conn->options.subtree && widget != parent) {
				continue;
			}
			switch (record->type) {
			case UI_MUTATION_RECORD_TYPE_CHILD_LIST:
				if (!conn->options.child_list) {
					continue;
				}
				break;
			case UI_MUTATION_RECORD_TYPE_PROPERTIES:
				if (!conn->options.properties) {
					continue;
				}
				break;
			case UI_MUTATION_RECORD_TYPE_ATTRIBUTES:
				if (!conn->options.attributes) {
					continue;
				}
				break;
			default:
				break;
			}
			ui_mutation_observer_add_record(conn->observer, record);
		}
	}
	return count;
}

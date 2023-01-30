#include "../include/ui.h"

LCUI_BOOL ui_widget_has_observer(ui_widget_t *widget,
				 ui_mutation_record_type_t type)
{
	ui_widget_t *parent;

	for (parent = widget; parent; parent = parent->parent) {
		if (!parent->extra || !parent->extra->observer) {
			continue;
		}
		if (!parent->extra->observer_options.subtree &&
		    widget != parent) {
			continue;
		}
		if (type == UI_MUTATION_RECORD_TYPE_CHILD_LIST &&
		    parent->extra->observer_options.child_list) {
			return TRUE;
		}
		if (type == UI_MUTATION_RECORD_TYPE_PROPERTIES &&
		    parent->extra->observer_options.properties) {
			return TRUE;
		}
		if (type == UI_MUTATION_RECORD_TYPE_ATTRIBUTES &&
		    parent->extra->observer_options.attributes) {
			return TRUE;
		}
	}
	return FALSE;
}

int ui_widget_add_mutation_recrod(ui_widget_t *widget,
				  ui_mutation_record_t *record)
{
	int count = 0;
	ui_widget_t *parent;

	for (parent = widget; parent; parent = parent->parent) {
		if (!parent->extra || !parent->extra->observer) {
			continue;
		}
		if (!parent->extra->observer_options.subtree &&
		    widget != parent) {
			continue;
		}
		switch (record->type) {
		case UI_MUTATION_RECORD_TYPE_CHILD_LIST:
			if (!parent->extra->observer_options.child_list) {
				continue;
			}
			break;
		case UI_MUTATION_RECORD_TYPE_PROPERTIES:
			if (!parent->extra->observer_options.properties) {
				continue;
			}
			break;
		case UI_MUTATION_RECORD_TYPE_ATTRIBUTES:
			if (!parent->extra->observer_options.attributes) {
				continue;
			}
			break;
		default:
			break;
		}
		ui_mutation_observer_add_record(parent->extra->observer,
						record);
	}
	return count;
}

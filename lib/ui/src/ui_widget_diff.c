﻿#include <string.h>
#include <LCUI/util.h>
#include "../include/ui.h"
#include "internal.h"

#define MEMCMP(A, B) memcmp(A, B, sizeof(*(A)))

void ui_widget_init_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff)
{
	diff->box = w->box;
	diff->should_add_dirty_rect = FALSE;
	if (w->parent) {
		if (!w->parent->computed_style.visible) {
			return;
		}
		if (w->parent->dirty_rect_type >=
		    UI_DIRTY_RECT_TYPE_PADDING_BOX) {
			w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
			return;
		}
	}
	if (w->state < UI_WIDGET_STATE_LAYOUTED) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	diff->should_add_dirty_rect = TRUE;
}

void ui_widget_begin_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff)
{
	const ui_widget_style_t* style = &w->computed_style;

	diff->left = w->computed_style.left;
	diff->right = w->computed_style.right;
	diff->top = w->computed_style.top;
	diff->bottom = w->computed_style.bottom;
	diff->width = w->width;
	diff->height = w->height;
	diff->margin = w->margin;
	diff->padding = w->padding;
	diff->display = style->display;
	diff->z_index = style->z_index;
	diff->visible = style->visible;
	diff->opacity = style->opacity;
	diff->position = style->position;
	diff->shadow = style->shadow;
	diff->border = style->border;
	diff->background = style->background;
	diff->flex = style->flex;
}

INLINE void ui_widget_add_reflow_task(ui_widget_t* w)
{
	if (w) {
		if (w->parent && ui_widget_has_valid_flexbox_style(w)) {
			ui_widget_add_task(w->parent, UI_TASK_REFLOW);
		}
		ui_widget_add_task(w, UI_TASK_REFLOW);
	}
}

INLINE void ui_widget_add_reflow_task_to_parent(ui_widget_t* w)
{
	if (w->computed_style.position == CSS_KEYWORD_ABSOLUTE) {
		return;
	}
	ui_widget_add_reflow_task(w->parent);
}

int ui_widget_end_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff)
{
	list_node_t* node;
	ui_mutation_record_t* record;
	const ui_mutation_record_type_t record_type =
	    UI_MUTATION_RECORD_TYPE_PROPERTIES;
	const ui_widget_style_t* style = &w->computed_style;

	if (style->visible != diff->visible) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		if (ui_widget_has_observer(w, record_type)) {
			record = ui_mutation_record_create(w, record_type);
			record->property_name = strdup2("visible");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
	}

	/* check layout related property changes */

	ui_widget_update_box_size(w);
	ui_widget_update_box_position(w);
	if (MEMCMP(&diff->box.padding, &w->box.padding)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		for (list_each(node, &w->children)) {
			ui_widget_add_task(node->data, UI_TASK_POSITION);
			ui_widget_add_task(node->data, UI_TASK_RESIZE);
		}
		ui_widget_add_reflow_task(w);
	} else if (MEMCMP(&diff->box.canvas, &w->box.canvas)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	if (ui_widget_has_valid_flexbox_style(w)) {
		if (diff->flex.wrap != style->flex.wrap ||
		    diff->flex.direction != style->flex.direction ||
		    diff->flex.justify_content != style->flex.justify_content ||
		    diff->flex.align_content != style->flex.align_content ||
		    diff->flex.align_items != style->flex.align_items) {
			ui_widget_add_reflow_task(w);
		}
		if (diff->flex.grow != style->flex.grow ||
		    diff->flex.shrink != style->flex.shrink ||
		    diff->flex.basis != style->flex.basis) {
			ui_widget_add_reflow_task_to_parent(w);
		}
	}
	if (diff->display != style->display) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		ui_widget_add_reflow_task_to_parent(w);
		if (style->display != CSS_KEYWORD_NONE) {
			ui_widget_add_reflow_task(w);
		}
	} else if (diff->position != style->position) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		if (diff->position == CSS_KEYWORD_ABSOLUTE ||
		    style->position == CSS_KEYWORD_ABSOLUTE) {
			ui_widget_add_reflow_task(w);
		}
		ui_widget_add_reflow_task_to_parent(w);
	}

	/* check repaint related property changes */

	if (!diff->should_add_dirty_rect) {
		return 0;
	}
	if (diff->opacity != w->computed_style.opacity) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_CANVAS_BOX) {
	} else if (diff->z_index != style->z_index &&
		   style->position != CSS_KEYWORD_STATIC) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (MEMCMP(&diff->shadow, &style->shadow)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_BORDER_BOX) {
	} else if (MEMCMP(&diff->border, &style->border)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_BORDER_BOX;
	} else if (MEMCMP(&diff->background, &style->background)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_BORDER_BOX;
	} else {
		return 0;
	}
	return 1;
}

void ui_widget_begin_layout_diff(ui_widget_t* w, ui_widget_layout_diff_t* diff)
{
	diff->box = w->box;
	diff->should_add_dirty_rect = FALSE;
	if (w->parent) {
		if (!w->parent->computed_style.visible) {
			return;
		}
		if (w->parent->dirty_rect_type >=
		    UI_DIRTY_RECT_TYPE_PADDING_BOX) {
			return;
		}
	}
	diff->should_add_dirty_rect = TRUE;
}

int ui_widget_end_layout_diff(ui_widget_t* w, ui_widget_layout_diff_t* diff)
{
	ui_event_t e;
	ui_mutation_record_t* record;
	const ui_mutation_record_type_t record_type =
	    UI_MUTATION_RECORD_TYPE_PROPERTIES;

	if (w->dirty_rect_type >= UI_DIRTY_RECT_TYPE_BORDER_BOX) {
		ui_widget_update_canvas_box(w);
	}
	if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_CANVAS_BOX) {
	} else if (!LCUIRectF_IsEquals(&diff->box.canvas, &w->box.canvas)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	if (diff->box.outer.x != w->box.outer.x ||
	    diff->box.outer.y != w->box.outer.y) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		w->update.skip_surface_props_sync = TRUE;
		ui_widget_add_reflow_task_to_parent(w);
	}
	if (diff->box.outer.width != w->box.outer.width ||
	    diff->box.outer.height != w->box.outer.height) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		e.target = w;
		e.data = NULL;
		e.type = UI_EVENT_RESIZE;
		e.cancel_bubble = TRUE;
		ui_widget_post_event(w, &e, NULL, NULL);
		w->update.skip_surface_props_sync = TRUE;
		ui_widget_add_reflow_task_to_parent(w);
	}
	if (ui_widget_has_observer(w, record_type)) {
		if (diff->box.border.x != w->box.border.x) {
			record = ui_mutation_record_create(w, record_type);
			record->property_name = strdup2("x");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->box.border.y != w->box.border.y) {
			record = ui_mutation_record_create(w, record_type);
			record->property_name = strdup2("y");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->box.border.width != w->box.border.width) {
			record = ui_mutation_record_create(w, record_type);
			record->property_name = strdup2("width");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->box.border.height != w->box.border.height) {
			record = ui_mutation_record_create(w, record_type);
			record->property_name = strdup2("height");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
	}
	if (!diff->should_add_dirty_rect) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
		return 0;
	}
	if (w->dirty_rect_type < UI_DIRTY_RECT_TYPE_PADDING_BOX) {
		return 0;
	}
	switch (w->dirty_rect_type) {
	case UI_DIRTY_RECT_TYPE_PADDING_BOX:
		w->dirty_rect = diff->box.padding;
		break;
	case UI_DIRTY_RECT_TYPE_BORDER_BOX:
		w->dirty_rect = diff->box.border;
		break;
	default:
		w->dirty_rect = diff->box.canvas;
		break;
	}
	while (w->parent) {
		w->parent->has_child_dirty_rect = TRUE;
		w = w->parent;
	}
	return 1;
}

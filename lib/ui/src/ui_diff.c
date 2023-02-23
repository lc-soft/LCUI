#include <string.h>
#include "internal.h"

#define IS_PROP_TYPE_CHANGED(PROP_KEY) \
	diff->style.type_bits.PROP_KEY != w->computed_style.type_bits.PROP_KEY

#define IS_PROP_VALUE_CHANGED(PROP_KEY) \
	diff->style.PROP_KEY != w->computed_style.PROP_KEY

#define IS_LENGTH_PROP_CHANGED(PROP_KEY) \
	(IS_PROP_TYPE_CHANGED(PROP_KEY) || IS_PROP_VALUE_CHANGED(PROP_KEY))

void ui_style_diff_init(ui_style_diff_t *diff, ui_widget_t *w)
{
	diff->content_box = w->content_box;
	diff->padding_box = w->padding_box;
	diff->border_box = w->border_box;
	diff->canvas_box = w->canvas_box;
	diff->outer_box = w->outer_box;
	diff->should_add_dirty_rect = FALSE;
	if (w->parent) {
		if (!ui_widget_is_visible(w->parent)) {
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

void ui_style_diff_begin(ui_style_diff_t *diff, ui_widget_t *w)
{
	diff->style = w->computed_style;
	diff->visible = ui_widget_is_visible(w);
}

INLINE void ui_widget_add_reflow_task(ui_widget_t *w)
{
	if (w) {
		if (w->parent && !ui_widget_has_absolute_position(w) &&
		    ui_widget_has_flex_display(w->parent)) {
			ui_widget_add_task(w->parent, UI_TASK_REFLOW);
		}
		ui_widget_add_task(w, UI_TASK_REFLOW);
	}
}

INLINE void ui_widget_add_reflow_task_to_parent(ui_widget_t *w)
{
	switch (css_computed_position(&w->computed_style)) {
	case CSS_POSITION_ABSOLUTE:
	case CSS_POSITION_FIXED:
		return;
	default:
		break;
	}
	ui_widget_add_reflow_task(w->parent);
}

void ui_style_diff_end(ui_style_diff_t *diff, ui_widget_t *w)
{
	ui_mutation_record_t *record;
	const LCUI_BOOL is_flex_item = ui_widget_is_flex_item(w);

	if (ui_widget_is_visible(w) != diff->visible) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
		if (ui_widget_has_observer(
			w, UI_MUTATION_RECORD_TYPE_PROPERTIES)) {
			record = ui_mutation_record_create(
			    w, UI_MUTATION_RECORD_TYPE_PROPERTIES);
			record->property_name = strdup2("visible");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
	}

	ui_widget_update_box_size(w);
	ui_widget_update_box_position(w);
	if (w->parent && ui_widget_in_layout_flow(w)) {
		// 检测是否需要父组件重新布局
		if (IS_LENGTH_PROP_CHANGED(margin_left) ||
		    IS_LENGTH_PROP_CHANGED(margin_right) ||
		    IS_LENGTH_PROP_CHANGED(margin_top) ||
		    IS_LENGTH_PROP_CHANGED(margin_bottom) ||
		    IS_PROP_TYPE_CHANGED(display) ||
		    IS_PROP_TYPE_CHANGED(position) ||
		    (is_flex_item && (IS_PROP_TYPE_CHANGED(flex_grow) ||
				      IS_PROP_TYPE_CHANGED(flex_shrink) ||
				      IS_PROP_TYPE_CHANGED(flex_basis)))) {
			ui_widget_add_reflow_task_to_parent(w);
		}
		// 检测自身内部是否需要重新布局
		if (diff->padding_box.width != w->padding_box.width ||
		    diff->padding_box.height != w->padding_box.height ||
		    (is_flex_item && (IS_PROP_TYPE_CHANGED(flex_wrap) ||
				      IS_PROP_TYPE_CHANGED(flex_direction) ||
				      IS_PROP_TYPE_CHANGED(justify_content) ||
				      IS_PROP_TYPE_CHANGED(align_content) ||
				      IS_PROP_TYPE_CHANGED(align_items)))) {
			ui_widget_add_reflow_task(w);
		}
	} else {
		if (IS_PROP_TYPE_CHANGED(display) ||
		    IS_PROP_TYPE_CHANGED(position) ||
		    diff->padding_box.width != w->padding_box.width ||
		    diff->padding_box.height != w->padding_box.height) {
			ui_widget_add_reflow_task(w);
		}
	}

	/* check repaint related property changes */

	if (!diff->should_add_dirty_rect) {
		return;
	}
	if (IS_PROP_VALUE_CHANGED(opacity)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_CANVAS_BOX) {
	} else if (IS_PROP_VALUE_CHANGED(z_index) &&
		   w->computed_style.type_bits.position !=
		       CSS_POSITION_STATIC) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (IS_PROP_VALUE_CHANGED(box_shadow_color) ||
		   IS_PROP_VALUE_CHANGED(box_shadow_x) ||
		   IS_PROP_VALUE_CHANGED(box_shadow_y) ||
		   IS_PROP_VALUE_CHANGED(box_shadow_blur) ||
		   IS_PROP_VALUE_CHANGED(box_shadow_spread)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_BORDER_BOX) {
	} else if (IS_PROP_VALUE_CHANGED(border_top_width) ||
		   IS_PROP_VALUE_CHANGED(border_right_width) ||
		   IS_PROP_VALUE_CHANGED(border_bottom_width) ||
		   IS_PROP_VALUE_CHANGED(border_left_width) ||
		   IS_PROP_VALUE_CHANGED(border_top_color) ||
		   IS_PROP_VALUE_CHANGED(border_right_color) ||
		   IS_PROP_VALUE_CHANGED(border_bottom_color) ||
		   IS_PROP_VALUE_CHANGED(border_left_color) ||
		   IS_PROP_TYPE_CHANGED(border_top_style) ||
		   IS_PROP_TYPE_CHANGED(border_right_style) ||
		   IS_PROP_TYPE_CHANGED(border_bottom_style) ||
		   IS_PROP_TYPE_CHANGED(border_left_style) ||
		   IS_PROP_VALUE_CHANGED(border_top_left_radius) ||
		   IS_PROP_VALUE_CHANGED(border_top_right_radius) ||
		   IS_PROP_VALUE_CHANGED(border_bottom_left_radius) ||
		   IS_PROP_VALUE_CHANGED(border_bottom_right_radius) ||
		   IS_PROP_VALUE_CHANGED(background_color) ||
		   IS_PROP_VALUE_CHANGED(background_image) ||
		   IS_PROP_VALUE_CHANGED(background_position_x) ||
		   IS_PROP_VALUE_CHANGED(background_position_y) ||
		   IS_PROP_VALUE_CHANGED(background_width) ||
		   IS_PROP_VALUE_CHANGED(background_height)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_BORDER_BOX;
	}
}

void ui_layout_diff_begin(ui_layout_diff_t *diff, ui_widget_t *w)
{
	diff->content_box = w->content_box;
	diff->padding_box = w->padding_box;
	diff->border_box = w->border_box;
	diff->canvas_box = w->canvas_box;
	diff->outer_box = w->outer_box;
	diff->should_add_dirty_rect =
	    !w->parent ||
	    (ui_widget_is_visible(w->parent) &&
	     w->parent->dirty_rect_type < UI_DIRTY_RECT_TYPE_PADDING_BOX);
}

void ui_layout_diff_auto_reflow(ui_layout_diff_t *diff, ui_widget_t *w)
{
	if (diff->padding_box.width != w->padding_box.width ||
	    diff->padding_box.height != w->padding_box.height) {
		ui_widget_reflow(w);
	}
}

void ui_layout_diff_end(ui_layout_diff_t *diff, ui_widget_t *w)
{
	ui_widget_t *parent;
	ui_mutation_record_t *record;
	ui_mutation_record_type_t type = UI_MUTATION_RECORD_TYPE_PROPERTIES;

	if (w->dirty_rect_type >= UI_DIRTY_RECT_TYPE_BORDER_BOX) {
		ui_widget_update_canvas_box(w);
	}
	if (w->dirty_rect_type != UI_DIRTY_RECT_TYPE_CANVAS_BOX &&
	    !ui_rect_is_equal(&diff->canvas_box, &w->canvas_box)) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	}
	if (diff->should_add_dirty_rect) {
		if (w->dirty_rect_type >= UI_DIRTY_RECT_TYPE_PADDING_BOX) {
			// 标记变更前的区域为脏矩形
			switch (w->dirty_rect_type) {
			case UI_DIRTY_RECT_TYPE_PADDING_BOX:
				w->dirty_rect = diff->padding_box;
				break;
			case UI_DIRTY_RECT_TYPE_BORDER_BOX:
				w->dirty_rect = diff->border_box;
				break;
			default:
				w->dirty_rect = diff->canvas_box;
				break;
			}
		}
		for (parent = w->parent; parent; parent = parent->parent) {
			parent->has_child_dirty_rect = TRUE;
		}
	} else {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
	}
	if (ui_rect_is_equal(&diff->border_box, &w->border_box) ||
	    !IS_CSS_FIXED_LENGTH(&w->computed_style, width) ||
	    !IS_CSS_FIXED_LENGTH(&w->computed_style, height)) {
		return;
	}
	ui_widget_add_reflow_task_to_parent(w);
	ui_widget_add_state(w, UI_WIDGET_STATE_LAYOUTED);
	if (ui_widget_has_observer(w, type)) {
		if (diff->border_box.x != w->border_box.x) {
			record = ui_mutation_record_create(w, type);
			record->property_name = strdup2("x");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->border_box.y != w->border_box.y) {
			record = ui_mutation_record_create(w, type);
			record->property_name = strdup2("y");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->border_box.width != w->border_box.width) {
			record = ui_mutation_record_create(w, type);
			record->property_name = strdup2("width");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
		if (diff->border_box.height != w->border_box.height) {
			record = ui_mutation_record_create(w, type);
			record->property_name = strdup2("height");
			ui_widget_add_mutation_recrod(w, record);
			ui_mutation_record_destroy(record);
		}
	}
}

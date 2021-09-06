
#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

typedef struct ui_flexbox_line_t {
	/** The size of it on the main axis */
	float main_size;

	/** The size of it on the cross axis */
	float cross_size;

	float sum_of_grow_value;
	float sum_of_shrink_value;
	size_t count_of_auto_margin_items;

	/** LinkedList<ui_widget_t*> elements */
	LinkedList elements;
} ui_flexbox_line_t;

typedef struct ui_flexbox_layout_context_t {
	ui_widget_t* widget;
	ui_layout_rule_t rule;
	LCUI_BOOL is_initiative;

	float main_axis;
	float cross_axis;
	float main_size;
	float cross_size;

	/** LinkedList<ui_flexbox_line_t*> lines */
	LinkedList lines;
	ui_flexbox_line_t* line;

	LinkedList free_elements;
} ui_flexbox_layout_context_t;

static ui_flexbox_line_t* ui_flexbox_line_create(void)
{
	ui_flexbox_line_t* line;

	line = malloc(sizeof(ui_flexbox_line_t));
	line->main_size = 0;
	line->cross_size = 0;
	line->sum_of_grow_value = 0;
	line->sum_of_shrink_value = 0;
	line->count_of_auto_margin_items = 0;
	LinkedList_Init(&line->elements);
	return line;
}

static void ui_flexbox_line_destroy(void *arg)
{
	ui_flexbox_line_t* line = arg;

	LinkedList_Clear(&line->elements, NULL);
	free(line);
}

static void ui_flexbox_line_load_element(ui_flexbox_line_t* line, ui_widget_t* w)
{
	if (w->computed_style.flex.grow > 0) {
		line->sum_of_grow_value += w->computed_style.flex.grow;
	}
	if (w->computed_style.flex.shrink > 0) {
		line->sum_of_shrink_value += w->computed_style.flex.shrink;
	}
	LinkedList_Append(&line->elements, w);
}

static void ui_flexbox_layout_next_line(ui_flexbox_layout_context_t* ctx)
{
	if (ctx->line) {
		ctx->main_size = max(ctx->main_size, ctx->line->main_size);
		ctx->cross_axis += ctx->line->cross_size;
		ctx->cross_size = ctx->cross_axis;
		if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
			ctx->cross_size -= ctx->widget->padding.left;
		} else {
			ctx->cross_size -= ctx->widget->padding.top;
		}
	}
	ctx->main_axis = ctx->widget->padding.left;
	ctx->line = ui_flexbox_line_create();
	LinkedList_Append(&ctx->lines, ctx->line);
}

static ui_flexbox_layout_context_t* ui_flexbox_layout_begin(ui_widget_t* w,
						     ui_layout_rule_t rule)
{
	ui_widget_style_t *style = &w->computed_style;
	ui_flexbox_layout_context_t *ctx;

	ctx = malloc(sizeof(ui_flexbox_layout_context_t));
	if (!ctx) {
		return NULL;
	}
	if (rule == UI_LAYOUT_RULE_AUTO) {
		ctx->is_initiative = TRUE;
		if (style->flex.direction == SV_COLUMN) {
			if (style->height_sizing == UI_SIZING_RULE_FIXED) {
				rule = UI_LAYOUT_RULE_FIXED_HEIGHT;
			} else {
				rule = UI_LAYOUT_RULE_MAX_CONTENT;
			}
		} else {
			if (style->width_sizing == UI_SIZING_RULE_FIXED) {
				rule = UI_LAYOUT_RULE_FIXED_WIDTH;
			} else {
				rule = UI_LAYOUT_RULE_MAX_CONTENT;
			}
		}
	} else {
		ctx->is_initiative = FALSE;
	}
	if (style->position == SV_ABSOLUTE) {
		if (rule == UI_LAYOUT_RULE_FIXED_HEIGHT &&
		    style->width_sizing == UI_SIZING_RULE_PERCENT) {
			rule = UI_LAYOUT_RULE_FIXED;
		} else if (rule == UI_LAYOUT_RULE_FIXED_WIDTH &&
			   style->height_sizing == UI_SIZING_RULE_PERCENT) {
			rule = UI_LAYOUT_RULE_FIXED;
		}
	}
	if (rule == UI_LAYOUT_RULE_FIXED_WIDTH) {
		if (style->height_sizing == UI_SIZING_RULE_FIXED) {
			rule = UI_LAYOUT_RULE_FIXED;
		}
	} else if (rule == UI_LAYOUT_RULE_FIXED_HEIGHT) {
		if (style->width_sizing == UI_SIZING_RULE_FIXED) {
			rule = UI_LAYOUT_RULE_FIXED;
		}
	}
	DEBUG_MSG("%s, rule: %d, is_initiative: %d\n", w->id, rule,
		  ctx->is_initiative);
	ctx->rule = rule;
	ctx->line = NULL;
	ctx->widget = w;
	if (style->flex.direction == SV_COLUMN) {
		ctx->main_axis = w->padding.left;
		ctx->cross_axis = w->padding.top;
	} else {
		ctx->main_axis = w->padding.top;
		ctx->cross_axis = w->padding.left;
	}
	ctx->main_size = 0;
	ctx->cross_size = 0;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->lines);
	ui_flexbox_layout_next_line(ctx);
	return ctx;
}

static void ui_flexbox_layout_end(ui_flexbox_layout_context_t* ctx)
{
	LinkedList_Clear(&ctx->lines, ui_flexbox_line_destroy);
	LinkedList_Clear(&ctx->free_elements, NULL);
	free(ctx);
}

static void ui_flexbox_layout_load_rows(ui_flexbox_layout_context_t* ctx)
{
	ui_widget_t* child;
	ui_flexbox_layout_style_t *flex = &ctx->widget->computed_style.flex;
	LinkedListNode *node;

	float basis;
	float max_main_size = -1;

	if (ctx->rule == UI_LAYOUT_RULE_FIXED ||
	    ctx->rule == UI_LAYOUT_RULE_FIXED_WIDTH) {
		max_main_size = ctx->widget->box.content.width;
	}
	DEBUG_MSG("%s, max_main_size: %g\n", ctx->widget->id, max_main_size);
	for (LinkedList_Each(node, &ctx->widget->children)) {
		child = node->data;
		if (child->computed_style.display == SV_NONE) {
			continue;
		}
		if (ui_widget_has_absolute_position(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		/* Clears the auto margin calculated on the last layout */
		if (ui_widget_has_auto_style(child, key_margin_left)) {
			child->margin.left = 0;
			if (ui_widget_has_auto_style(child, key_margin_right)) {
				child->margin.right = 0;
			}
			ui_widget_update_box_size(child);
		} else if (ui_widget_has_auto_style(child, key_margin_right)) {
			child->margin.right = 0;
			ui_widget_update_box_size(child);
		}
		ui_widget_compute_flex_basis_style(child);
		basis = margin_x(child) + child->computed_style.flex.basis;
		DEBUG_MSG("[line %lu][%lu] main_size: %g, basis: %g\n",
			  ctx->lines.length, child->index, ctx->line->main_size,
			  basis);
		/* Check line wrap */
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0 &&
		    max_main_size != -1) {
			if (ctx->line->main_size + basis - max_main_size >
			    0.4f) {
				ui_flexbox_layout_next_line(ctx);
			}
		}
		if (child->box.outer.height > ctx->line->cross_size) {
			ctx->line->cross_size = child->box.outer.height;
		}
		if (ui_widget_has_auto_style(child, key_margin_left)) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (ui_widget_has_auto_style(child, key_margin_right)) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += basis;
		ui_flexbox_line_load_element(ctx->line, child);
	}
	ctx->main_size = max(ctx->main_size, ctx->line->main_size);
	ctx->cross_size += ctx->line->cross_size;
	DEBUG_MSG("%s,end load, size: (%g, %g)\n", ctx->widget->id,
		  ctx->main_size, ctx->cross_size);
}

static void ui_flexbox_layout_load_columns(ui_flexbox_layout_context_t* ctx)
{
	ui_widget_t* child;
	ui_flexbox_layout_style_t *flex = &ctx->widget->computed_style.flex;
	LinkedListNode *node;

	float basis;
	float max_main_size = -1;

	if (ctx->rule == UI_LAYOUT_RULE_FIXED ||
	    ctx->rule == UI_LAYOUT_RULE_FIXED_HEIGHT) {
		max_main_size = ctx->widget->box.content.height;
	}
	DEBUG_MSG("max_main_size: %g\n", max_main_size);
	for (LinkedList_Each(node, &ctx->widget->children)) {
		child = node->data;

		if (child->computed_style.display == SV_NONE) {
			continue;
		}
		if (ui_widget_has_absolute_position(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		ui_widget_compute_flex_basis_style(child);
		basis = margin_y(child) + child->computed_style.flex.basis;
		DEBUG_MSG("[column %lu][%lu] main_size: %g, basis: %g\n",
			  ctx->lines.length, child->index, ctx->line->main_size,
			  basis);
		if (flex->wrap == SV_WRAP && ctx->line->elements.length > 0 &&
		    max_main_size != -1) {
			if (ctx->line->main_size + basis - max_main_size >
			    0.4f) {
				ui_flexbox_layout_next_line(ctx);
			}
		}
		if (child->box.outer.width > ctx->line->cross_size) {
			ctx->line->cross_size = child->box.outer.width;
		}
		if (ui_widget_has_auto_style(child, key_margin_top)) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (ui_widget_has_auto_style(child, key_margin_top)) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += basis;
		ui_flexbox_line_load_element(ctx->line, child);
	}
	ctx->main_size = max(ctx->main_size, ctx->line->main_size);
	ctx->cross_size += ctx->line->cross_size;
	DEBUG_MSG("main_size: %g\n", ctx->main_size);
}

static void ui_flexbox_layout_load(ui_flexbox_layout_context_t* ctx)
{
	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		ui_flexbox_layout_load_columns(ctx);
	} else {
		ui_flexbox_layout_load_rows(ctx);
	}
}

static void ui_flexbox_layout_compute_justify_content(ui_flexbox_layout_context_t* ctx,
						float *start_axis, float *space)
{
	float free_space;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		free_space = ctx->widget->box.content.height;
	} else {
		free_space = ctx->widget->box.content.width;
	}
	free_space -= ctx->line->main_size;
	switch (ctx->widget->computed_style.flex.justify_content) {
	case SV_SPACE_BETWEEN:
		if (ctx->line->elements.length > 1) {
			*space = free_space / (ctx->line->elements.length - 1);
		}
		*start_axis -= *space;
		break;
	case SV_SPACE_AROUND:
		*space = free_space / ctx->line->elements.length;
		*start_axis -= *space * 0.5f;
		break;
	case SV_SPACE_EVENLY:
		*space = free_space / (ctx->line->elements.length + 1);
		*start_axis += *space;
		break;
	case SV_RIGHT:
	case SV_FLEX_END:
		*start_axis += free_space;
		break;
	case SV_CENTER:
		*start_axis += free_space * 0.5f;
		break;
	case SV_LEFT:
	default:
		break;
	}
}

static void update_flex_item_size(ui_widget_t* w, ui_layout_rule_t rule)
{
	float content_width = w->box.padding.width;
	float content_height = w->box.padding.height;
	ui_widget_layout_diff_t diff;

	ui_widget_begin_layout_diff(w, &diff);
	/*
	 * As a widget of a flex item, its size is calculated by the flexbox
	 * layout engine, so here we only need to calculate its width and
	 * height limits.
	 */
	ui_widget_compute_widget_limit_style(w, UI_LAYOUT_RULE_FIXED);
	ui_widget_compute_height_limit_style(w, UI_LAYOUT_RULE_FIXED);
	ui_widget_update_box_size(w);
	if (content_width == w->box.padding.width &&
	    content_height == w->box.padding.height) {
		return;
	}
	if (rule == UI_LAYOUT_RULE_FIXED_WIDTH ||
	    rule == UI_LAYOUT_RULE_FIXED_HEIGHT) {
		rule = UI_LAYOUT_RULE_FIXED;
	}
	ui_widget_update(w, rule);
	ui_widget_end_layout_diff(w, &diff);
	w->update.states[UI_TASK_REFLOW] = FALSE;
}

static void ui_flexbox_layout_update_row(ui_flexbox_layout_context_t* ctx)
{
	float k = 0;
	float main_axis = 0;
	float space = 0;
	float free_space;

	ui_widget_t* w;
	ui_flexbox_layout_style_t *flex;
	LinkedListNode *node;

	free_space = ctx->widget->box.content.width - ctx->line->main_size;
	if (free_space >= 0) {
		if (ctx->line->sum_of_grow_value > 0) {
			k = free_space / ctx->line->sum_of_grow_value;
		}
	} else if (ctx->line->sum_of_shrink_value > 0) {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */
	DEBUG_MSG("%s, free_space: %g\n", ctx->widget->id, free_space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (w->computed_style.height_sizing != UI_SIZING_RULE_FIXED) {
			ui_widget_compute_height_style(w);
		}
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->width = flex->basis + k * flex->grow;
				update_flex_item_size(
				    w, UI_LAYOUT_RULE_FIXED_WIDTH);
			} else {
				update_flex_item_size(
				    w, UI_LAYOUT_RULE_FIXED_WIDTH);
			}
		} else if (flex->shrink > 0) {
			w->width = flex->basis + k * flex->shrink;
			update_flex_item_size(w, UI_LAYOUT_RULE_FIXED_WIDTH);
		} else {
			update_flex_item_size(w, UI_LAYOUT_RULE_FIXED_WIDTH);
		}
		ui_widget_add_state(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.width;
	}
	ctx->line->main_size = main_axis;
	free_space = ctx->widget->box.content.width - main_axis;

	/* auto margin */
	DEBUG_MSG("free_space: %g, auto margin items: %lu\n", free_space,
		  ctx->line->count_of_auto_margin_items);
	if (free_space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = free_space / ctx->line->count_of_auto_margin_items;
		for (LinkedList_Each(node, &ctx->line->elements)) {
			w = node->data;
			if (ui_widget_has_auto_style(w, key_margin_left)) {
				w->margin.left = k;
				ui_widget_update_box_size(w);
			}
			if (ui_widget_has_auto_style(w, key_margin_right)) {
				w->margin.right = k;
				ui_widget_update_box_size(w);
			}
			DEBUG_MSG("basis: %g\n", w->box.outer.width);
			main_axis += w->box.outer.width;
		}
		ctx->line->main_size = main_axis;
	}

	/* update the position of each child widget */

	main_axis = ctx->widget->padding.left;
	ui_flexbox_layout_compute_justify_content(ctx, &main_axis, &space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		main_axis += space;
		w->layout_x = main_axis;
		ui_widget_update_box_position(w);
		main_axis += w->box.outer.width;
	}
	ctx->line->main_size = ctx->main_size;
}

static void ui_flexbox_layout_update_column(ui_flexbox_layout_context_t* ctx)
{
	float k;
	float main_axis = 0;
	float space = 0;
	float free_space;

	ui_widget_t* w;
	ui_flexbox_layout_style_t *flex;
	LinkedListNode *node;

	free_space = ctx->widget->box.content.height - ctx->line->main_size;
	if (free_space >= 0) {
		k = free_space / ctx->line->sum_of_grow_value;
	} else {
		k = free_space / ctx->line->sum_of_shrink_value;
	}

	/* flex-grow and flex-shrink */

	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		flex = &w->computed_style.flex;
		if (w->computed_style.width_sizing != UI_SIZING_RULE_FIXED) {
			ui_widget_compute_width_style(w);
		}
		if (free_space >= 0) {
			if (flex->grow > 0) {
				w->height = flex->basis + k * flex->grow;
				update_flex_item_size(
				    w, UI_LAYOUT_RULE_FIXED_HEIGHT);
			} else {
				update_flex_item_size(
				    w, UI_LAYOUT_RULE_FIXED_HEIGHT);
			}
		} else if (flex->shrink > 0) {
			w->height = flex->basis + k * flex->shrink;
			update_flex_item_size(w, UI_LAYOUT_RULE_FIXED_HEIGHT);
		} else {
			update_flex_item_size(w, UI_LAYOUT_RULE_FIXED_HEIGHT);
		}
		ui_widget_add_state(w, LCUI_WSTATE_LAYOUTED);
		main_axis += w->box.outer.height;
	}
	free_space = ctx->widget->box.content.height - ctx->line->main_size;
	free_space = main_axis - ctx->main_size;

	/* auto margin */

	if (free_space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = free_space / ctx->line->count_of_auto_margin_items;
		for (LinkedList_Each(node, &ctx->line->elements)) {
			w = node->data;
			if (ui_widget_has_auto_style(w, key_margin_top)) {
				w->margin.top = k;
				ui_widget_update_box_size(w);
			}
			if (ui_widget_has_auto_style(w, key_margin_bottom)) {
				w->margin.bottom = k;
				ui_widget_update_box_size(w);
			}
			main_axis += w->box.outer.height;
		}
		ctx->line->main_size = main_axis;
	}

	/* justify-content */

	main_axis = ctx->widget->padding.top;
	ui_flexbox_layout_compute_justify_content(ctx, &main_axis, &space);
	for (LinkedList_Each(node, &ctx->line->elements)) {
		w = node->data;
		main_axis += space;
		w->layout_y = main_axis;
		ui_widget_update_box_position(w);
		main_axis += w->box.outer.height;
	}
	ctx->line->main_size = ctx->main_size;
}

static void ui_flexbox_layout_align_items_center(ui_flexbox_layout_context_t* ctx,
					   float base_cross_axis)
{
	LinkedListNode *node;
	ui_widget_t* child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x =
			    base_cross_axis +
			    (ctx->line->cross_size - child->box.outer.width) *
				0.5f;
			ui_widget_update_box_position(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y =
		    base_cross_axis +
		    (ctx->line->cross_size - child->box.outer.height) * 0.5f;
		ui_widget_update_box_position(child);
	}
}

static void ui_flexbox_layout_align_items_stretch(ui_flexbox_layout_context_t* ctx,
					    float base_cross_axis)
{
	LinkedListNode *node;
	ui_widget_t* child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis;
			if (ui_widget_has_auto_style(child, key_width)) {
				child->width =
				    ctx->line->cross_size - margin_x(child);
				update_flex_item_size(child,
						   UI_LAYOUT_RULE_FIXED);
			}
			ui_widget_update_box_position(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis;
		if (ui_widget_has_auto_style(child, key_height)) {
			child->height = ctx->line->cross_size - margin_y(child);
			update_flex_item_size(child, UI_LAYOUT_RULE_FIXED);
		}
		ui_widget_update_box_position(child);
	}
}

static void ui_flexbox_layout_align_items_start(ui_flexbox_layout_context_t* ctx,
					  float base_cross_axis)
{
	LinkedListNode *node;
	ui_widget_t* child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis;
			ui_widget_update_box_position(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis;
		ui_widget_update_box_position(child);
	}
}

static void ui_flexbox_layout_align_items_end(ui_flexbox_layout_context_t* ctx,
					float base_cross_axis)
{
	LinkedListNode *node;
	ui_widget_t* child;

	if (ctx->widget->computed_style.flex.direction == SV_COLUMN) {
		for (LinkedList_Each(node, &ctx->line->elements)) {
			child = node->data;
			child->layout_x = base_cross_axis +
					  ctx->line->cross_size -
					  child->box.outer.width;
			ui_widget_update_box_position(child);
		}
		return;
	}
	for (LinkedList_Each(node, &ctx->line->elements)) {
		child = node->data;
		child->layout_y = base_cross_axis + ctx->line->cross_size -
				  child->box.outer.height;
		ui_widget_update_box_position(child);
	}
}

static void ui_flexbox_layout_align_items(ui_flexbox_layout_context_t* ctx)
{
	float cross_axis;
	float free_space = 0;

	ui_widget_t* w = ctx->widget;
	LinkedListNode *node;

	if (w->computed_style.flex.direction == SV_COLUMN) {
		cross_axis = w->padding.left;
		free_space = w->box.content.width - ctx->cross_size;
	} else {
		cross_axis = w->padding.top;
		free_space = w->box.content.height - ctx->cross_size;
	}
	if (free_space < 0) {
		free_space = 0;
	}
	for (LinkedList_Each(node, &ctx->lines)) {
		ctx->line = node->data;
		ctx->line->cross_size += free_space / ctx->lines.length;
		switch (w->computed_style.flex.align_items) {
		case SV_CENTER:
			ui_flexbox_layout_align_items_center(ctx, cross_axis);
			break;
		case SV_FLEX_START:
			ui_flexbox_layout_align_items_start(ctx, cross_axis);
			break;
		case SV_FLEX_END:
			ui_flexbox_layout_align_items_end(ctx, cross_axis);
			break;
		case SV_NORMAL:
		case SV_STRETCH:
		default:
			ui_flexbox_layout_align_items_stretch(ctx, cross_axis);
			break;
		}
		cross_axis += ctx->line->cross_size;
	}
	if (w->computed_style.flex.direction == SV_COLUMN) {
		ctx->cross_size = cross_axis - w->padding.left;
	} else {
		ctx->cross_size = cross_axis - w->padding.top;
	}
}

static void ui_flexbox_layout_update(ui_flexbox_layout_context_t* ctx)
{
	ui_widget_t* w = ctx->widget;
	LinkedListNode *node;

	DEBUG_MSG("widget: %s, start\n", w->id);
	for (LinkedList_Each(node, &ctx->lines)) {
		ctx->line = node->data;
		if (w->computed_style.flex.direction == SV_COLUMN) {
			ui_flexbox_layout_update_column(ctx);
		} else {
			ui_flexbox_layout_update_row(ctx);
		}
	}
	DEBUG_MSG("widget: %s, end\n", w->id);
	ui_flexbox_layout_align_items(ctx);
}

static void ui_flexbox_layout_update_free_elements(ui_flexbox_layout_context_t* ctx)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &ctx->free_elements)) {
		ui_widget_auto_reflow(node->data, UI_LAYOUT_RULE_FIXED);
	}
}

static void ui_flexbox_layout_apply_size(ui_flexbox_layout_context_t* ctx)
{
	float width = 0, height = 0;

	ui_widget_t* w = ctx->widget;

	DEBUG_MSG("widget: %s, main_size: %g, cross_size: %g\n", w->id,
		  ctx->main_size, ctx->cross_size);
	if (w->computed_style.flex.direction == SV_COLUMN) {
		switch (ctx->rule) {
		case UI_LAYOUT_RULE_FIXED:
			width = w->box.content.width;
			height = w->box.content.height;
			break;
		case UI_LAYOUT_RULE_FIXED_WIDTH:
			width = w->box.content.width;
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = w->box.content.width;
			height = max(height, ctx->main_size);
			break;
		case UI_LAYOUT_RULE_FIXED_HEIGHT:
			height = w->box.content.height;
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = w->box.content.height;
			width = max(width, ctx->cross_size);
			break;
		default:
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = max(height, ctx->main_size);
			width = max(width, ctx->cross_size);
			break;
		}
		w->width = to_border_box_width(w, width);
		w->height = to_border_box_height(w, height);
		ui_widget_update_box_size(w);
		if (ctx->is_initiative) {
			w->max_content_width = w->box.content.width;
			w->max_content_height = w->box.content.height;
		}
		w->proto->resize(w, w->box.content.width,
				 w->box.content.height);
		return;
	}
	switch (ctx->rule) {
	case UI_LAYOUT_RULE_FIXED:
		width = w->box.content.width;
		height = w->box.content.height;
		break;
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		width = w->box.content.width;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = w->box.content.width;
		height = max(height, ctx->cross_size);
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		height = w->box.content.height;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->main_size);
		height = w->box.content.height;
		break;
	default:
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->main_size);
		height = max(height, ctx->cross_size);
		break;
	}
	w->width = to_border_box_width(w, width);
	w->height = to_border_box_height(w, height);
	ui_widget_update_box_size(w);
	if (ctx->is_initiative) {
		w->max_content_width = w->box.content.width;
		w->max_content_height = w->box.content.height;
	}
	w->proto->resize(w, w->box.content.width, w->box.content.height);
}

void ui_flexbox_layout_reflow(ui_widget_t* w, ui_layout_rule_t rule)
{
	ui_flexbox_layout_context_t* ctx;

	ctx = ui_flexbox_layout_begin(w, rule);
	ui_flexbox_layout_load(ctx);
	ui_flexbox_layout_apply_size(ctx);
	ui_flexbox_layout_update(ctx);
	ui_flexbox_layout_update_free_elements(ctx);
	ui_flexbox_layout_end(ctx);
}

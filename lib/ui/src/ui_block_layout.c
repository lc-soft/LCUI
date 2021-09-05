
#include <LCUI.h>
#include "../include/ui.h"
#include "private.h"

typedef struct ui_block_layout_row_t {
	float width;
	float height;
	LinkedList elements;
} ui_block_layout_row_t;

typedef struct ui_block_layout_context_t {
	ui_widget_t* widget;
	ui_layout_rule_t rule;
	LCUI_BOOL is_initiative;

	float x, y;
	float content_width;
	float content_height;

	ui_widget_t* prev;
	int prev_display;

	/*
	 * LinkedList<ui_block_layout_row_t> rows
	 * Element rows in the static layout flow
	 */
	LinkedList rows;
	ui_block_layout_row_t* row;

	/*
	 * LinkedList<ui_widget_t*> free_elements
	 * A list of elements that do not exist in the static layout flow
	 */
	LinkedList free_elements;
} ui_block_layout_context_t;

static void ui_block_layout_update_free_element_position(
    ui_block_layout_context_t* ctx, ui_widget_t* w, float x, float y)
{
	if (w->computed_style.position != SV_ABSOLUTE &&
	    w->computed_style.display == SV_INLINE_BLOCK) {
		switch (w->computed_style.vertical_align) {
		case SV_MIDDLE:
			if (!w->parent) {
				break;
			}
			y += (ctx->row->height - w->height) / 2.f;
			break;
		case SV_BOTTOM:
			if (!w->parent) {
				break;
			}
			y += ctx->row->height - w->height;
		case SV_TOP:
		default:
			break;
		}
	}
	w->layout_x = x;
	w->layout_y = y;
	ui_widget_update_box_position(w);
}

static ui_block_layout_row_t* ui_block_layout_row_create(void)
{
	ui_block_layout_row_t* row;

	row = malloc(sizeof(ui_block_layout_row_t));
	row->width = 0;
	row->height = 0;
	LinkedList_Init(&row->elements);
	return row;
}

static void ui_block_layout_row_destroy(void* arg)
{
	ui_block_layout_row_t* row = arg;

	LinkedList_Clear(&row->elements, NULL);
	free(row);
}

static void ui_block_layout_next_row(ui_block_layout_context_t* ctx)
{
	if (ctx->row) {
		ctx->content_width = max(ctx->content_width, ctx->row->width);
		ctx->content_height += ctx->row->height;
		ctx->y += ctx->row->height;
	}
	ctx->prev_display = 0;
	ctx->x = ctx->widget->padding.left;
	ctx->row = ui_block_layout_row_create();
	LinkedList_Append(&ctx->rows, ctx->row);
}

static ui_block_layout_context_t* ui_block_layout_begin(ui_widget_t* w,
							ui_layout_rule_t rule)
{
	ui_widget_style_t *style = &w->computed_style;
	ui_block_layout_context_t *ctx;

	ctx = malloc(sizeof(ui_block_layout_context_t));
	if (!ctx) {
		return NULL;
	}
	if (rule == UI_LAYOUT_RULE_AUTO) {
		ctx->is_initiative = TRUE;
		if (style->width_sizing == UI_SIZING_RULE_FIXED) {
			if (style->height_sizing == UI_SIZING_RULE_FIXED) {
				rule = UI_LAYOUT_RULE_FIXED;
			} else {
				rule = UI_LAYOUT_RULE_FIXED_WIDTH;
			}
		} else if (style->height_sizing == UI_SIZING_RULE_FIXED) {
			rule = UI_LAYOUT_RULE_FIXED_HEIGHT;
		} else {
			rule = UI_LAYOUT_RULE_MAX_CONTENT;
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
	if (rule == UI_LAYOUT_RULE_MAX_CONTENT) {
		if (style->width_sizing == UI_SIZING_RULE_FIXED) {
			rule = UI_LAYOUT_RULE_FIXED_WIDTH;
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
	ctx->rule = rule;
	ctx->row = NULL;
	ctx->widget = w;
	ctx->x = w->padding.left;
	ctx->y = w->padding.right;
	ctx->content_width = 0;
	ctx->content_height = 0;
	ctx->prev_display = 0;
	ctx->prev = NULL;
	LinkedList_Init(&ctx->free_elements);
	LinkedList_Init(&ctx->rows);
	ui_block_layout_next_row(ctx);
	return ctx;
}

static void ui_block_layout_update_item_size(ui_widget_t* w,
					     ui_layout_rule_t rule)
{
	float content_width = w->box.content.width;
	float content_height = w->box.content.height;
	ui_widget_layout_diff_t diff;

	ui_widget_begin_layout_diff(w, &diff);
	ui_widget_compute_widget_limit_style(w, UI_LAYOUT_RULE_FIXED);
	ui_widget_compute_height_limit_style(w, UI_LAYOUT_RULE_FIXED);
	ui_widget_compute_width_style(w);
	ui_widget_compute_height_style(w);
	ui_widget_update_box_size(w);
	if (content_width == w->box.content.width &&
	    content_height == w->box.content.height) {
		return;
	}
	ui_widget_reflow(w, rule);
	ui_widget_end_layout_diff(w, &diff);
	w->update.states[UI_TASK_REFLOW] = FALSE;
}

static void ui_block_layout_load(ui_block_layout_context_t* ctx)
{
	float max_row_width = -1;

	ui_widget_t* child;
	ui_widget_t* w = ctx->widget;
	LinkedListNode* node;

	if (ctx->rule == UI_LAYOUT_RULE_FIXED_WIDTH ||
	    ctx->rule == UI_LAYOUT_RULE_FIXED) {
		max_row_width = ctx->widget->box.content.width;
	} else {
		if (w->computed_style.max_width != -1) {
			max_row_width = w->computed_style.max_width -
					Widget_padding_x(w) - Widget_border_x(w);
		}
	}
	DEBUG_MSG("%s, start\n", ctx->widget->id);
	DEBUG_MSG("%s, max_row_width: %g\n", ctx->widget->id, max_row_width);
	for (LinkedList_Each(node, &w->children)) {
		child = node->data;

		if (Widget_HasAbsolutePosition(child)) {
			LinkedList_Append(&ctx->free_elements, child);
			continue;
		}
		/*
		 * If the width of the child widget depends on the width of
		 * the parent widget, we need to calculate the intrinsic
		 * maximum width of its content to calculate the appropriate
		 * size of the widget's content area.
		 */
		if (child->computed_style.width_sizing !=
			UI_SIZING_RULE_FIXED &&
		    child->computed_style.width_sizing !=
			UI_SIZING_RULE_FIT_CONTENT) {
			ui_block_layout_update_item_size(
			    child, UI_LAYOUT_RULE_MAX_CONTENT);
		}
		DEBUG_MSG(
		    "row %lu, child %lu, static size: (%g, %g), display: %d\n",
		    ctx->rows.length, child->index, child->box.outer.width,
		    child->box.outer.height, child->computed_style.display);
		switch (child->computed_style.display) {
		case SV_INLINE_BLOCK:
			if (ctx->prev_display &&
			    ctx->prev_display != SV_INLINE_BLOCK) {
				DEBUG_MSG("next row\n");
				ui_block_layout_next_row(ctx);
			}
			if (max_row_width != -1 &&
			    ctx->row->elements.length > 0 &&
			    ctx->row->width + child->box.outer.width -
				    max_row_width >
				0.4f) {
				DEBUG_MSG("next row\n");
				ui_block_layout_next_row(ctx);
			}
			break;
		case SV_FLEX:
		case SV_BLOCK:
			ui_block_layout_next_row(ctx);
			break;
		case SV_NONE:
		default:
			continue;
		}
		DEBUG_MSG("row %lu, xy: (%g, %g)\n", ctx->rows.length, ctx->x,
			  ctx->y);
		ctx->row->width += child->box.outer.width;
		if (child->box.outer.height > ctx->row->height) {
			ctx->row->height = child->box.outer.height;
		}
		LinkedList_Append(&ctx->row->elements, child);
		ctx->prev_display = child->computed_style.display;
		ctx->prev = child;
	}
	ctx->content_width = max(ctx->content_width, ctx->row->width);
	ctx->content_height += ctx->row->height;
	DEBUG_MSG("content_size: %g, %g\n", ctx->content_width,
		  ctx->content_height);
	DEBUG_MSG("%s, end\n", ctx->widget->id);
}

static void ui_block_layout_update_element_margin(
    ui_block_layout_context_t* ctx, ui_widget_t* w)
{
	if (w->computed_style.display != SV_BLOCK) {
		return;
	}
	if (!ui_widget_has_auto_style(w, key_margin_left)) {
		return;
	}
	if (ui_widget_has_auto_style(w, key_margin_right)) {
		w->margin.left = (ctx->content_width - w->width) / 2.f;
		w->margin.right = w->margin.left;
		return;
	}
	w->margin.left = ctx->content_width - w->width;
	w->margin.left -= w->margin.right;
}

static void ui_widget_update_block_layout_row(ui_block_layout_context_t* ctx,
					      float row_y)
{
	float x = ctx->widget->padding.left;

	ui_widget_t* w;
	LinkedListNode* node;

	for (LinkedList_Each(node, &ctx->row->elements)) {
		w = node->data;
		ui_block_layout_update_item_size(w, UI_LAYOUT_RULE_FIXED);
		ui_block_layout_update_element_margin(ctx, w);
		ui_block_layout_update_free_element_position(ctx, w, x, row_y);
		ui_widget_add_state(w, LCUI_WSTATE_LAYOUTED);
		x += w->box.outer.width;
	}
}

static void ui_block_layout_update_free_elements(ui_block_layout_context_t* ctx)
{
	LinkedListNode* node;
	for (LinkedList_Each(node, &ctx->free_elements)) {
		ui_widget_auto_reflow(node->data, UI_LAYOUT_RULE_FIXED);
	}
}

static void BlockLayout_Reflow(ui_block_layout_context_t* ctx)
{
	float y;
	ui_widget_t* w = ctx->widget;
	LinkedListNode* node;

	y = w->padding.top;
	if (w->computed_style.display != SV_INLINE_BLOCK) {
		ctx->content_width = w->box.content.width;
	}
	for (LinkedList_Each(node, &ctx->rows)) {
		ctx->row = node->data;
		ui_widget_update_block_layout_row(ctx, y);
		y += ctx->row->height;
	}
	ctx->content_height = y - w->padding.top;
}

static void ui_block_layout_end(ui_block_layout_context_t* ctx)
{
	LinkedList_Clear(&ctx->rows, ui_block_layout_row_destroy);
	LinkedList_Clear(&ctx->free_elements, NULL);
	free(ctx);
}

static void ui_block_layout_apply_size(ui_block_layout_context_t* ctx)
{
	float width = 0, height = 0;

	ui_widget_t* w = ctx->widget;

	switch (ctx->rule) {
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		width = w->box.content.width;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = w->box.content.width;
		height = max(height, ctx->content_height);
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		height = w->box.content.height;
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->content_width);
		height = w->box.content.height;
		break;
	case UI_LAYOUT_RULE_MAX_CONTENT:
		w->proto->autosize(w, &width, &height, ctx->rule);
		width = max(width, ctx->content_width);
		height = max(height, ctx->content_height);
		break;
	default:
		width = w->box.content.width;
		height = w->box.content.height;
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

void ui_widget_update_block_layout(ui_widget_t* w, ui_layout_rule_t rule)
{
	ui_block_layout_context_t* ctx;

	ctx = ui_block_layout_begin(w, rule);
	ui_block_layout_load(ctx);
	ui_block_layout_apply_size(ctx);
	ui_block_layout_update(ctx);
	ui_block_layout_update_free_elements(ctx);
	ui_block_layout_end(ctx);
}

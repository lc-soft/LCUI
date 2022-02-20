// #define UI_DEBUG_ENABLED
#include "internal.h"

typedef struct ui_block_layout_row_t {
	float width;
	float height;

	/** list_t<ui_layout_item_t*> */
	list_t items;
} ui_block_layout_row_t;

typedef struct ui_block_layout_context_t {
	ui_widget_t *widget;
	ui_layout_rule_t rule;
	LCUI_BOOL is_initiative;

	float x, y;
	float content_width;
	float content_height;

	ui_widget_t *prev;
	int prev_display;

	ui_layout_item_t *items;

	/*
	 * list_t<ui_block_layout_row_t> rows
	 * Element rows in the static layout flow
	 */
	list_t rows;
	ui_block_layout_row_t *row;
} ui_block_layout_context_t;

static ui_block_layout_row_t *ui_block_layout_row_create(void)
{
	ui_block_layout_row_t *row;

	row = malloc(sizeof(ui_block_layout_row_t));
	row->width = 0;
	row->height = 0;
	list_create(&row->items);
	return row;
}

static void ui_block_layout_row_destroy(void *arg)
{
	ui_block_layout_row_t *row = arg;

	list_destroy(&row->items, NULL);
	free(row);
}

static void ui_block_layout_next_row(ui_block_layout_context_t *ctx)
{
	if (ctx->row) {
		ctx->content_width = y_max(ctx->content_width, ctx->row->width);
		ctx->content_height += ctx->row->height;
		ctx->y += ctx->row->height;
	}
	ctx->prev_display = 0;
	ctx->x = ctx->widget->computed_style.padding_left;
	ctx->row = ui_block_layout_row_create();
	list_append(&ctx->rows, ctx->row);
}

static void ui_block_layout_load(ui_block_layout_context_t *ctx)
{
	ui_widget_t *child;
	ui_layout_item_t *item;
	css_computed_style_t *s = &ctx->widget->computed_style;
	list_node_t *node;
	float max_row_width = -1;

	if ((ctx->rule & UI_LAYOUT_RULE_FIXED_WIDTH) ==
	    UI_LAYOUT_RULE_FIXED_WIDTH) {
		max_row_width = ctx->widget->content_box.width;
	} else {
		if (IS_CSS_FIXED_LENGTH(s, max_width)) {
			max_row_width = s->max_width;
			if (s->type_bits.box_sizing ==
			    CSS_BOX_SIZING_BORDER_BOX) {
				max_row_width -=
				    css_padding_x(s) + css_border_x(s);
			}
		}
	}

#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("max_row_width = %g", max_row_width);
#endif
	for (item = ctx->items, list_each(node, &ctx->widget->children),
	    ++item) {
		child = node->data;
		item->widget = node->data;
		ui_layout_diff_begin(&item->diff, child);
		ui_widget_prepare_reflow(child, ctx->rule);
		if (!ui_widget_in_layout_flow(child)) {
			list_append(&ctx->row->items, item);
			continue;
		}
		ui_widget_compute_style(child);
		ui_widget_update_box_size(child);
		ui_widget_update_box_position(child);
		ui_layout_diff_auto_reflow(&item->diff, child);
#ifdef UI_DEBUG_ENABLED
		{
			UI_WIDGET_STR(child, str);
			UI_DEBUG_MSG("[%lu] %s: row = %lu, xy = (%g, %g), "
				     "outer_box_size = (%g, %g), display: %d",
				     item - ctx->items, str, ctx->rows.length,
				     ctx->x, ctx->y, child->outer_box.width,
				     child->outer_box.height,
				     child->computed_style.type_bits.display);
		}
#endif
		switch (child->computed_style.type_bits.display) {
		case CSS_DISPLAY_INLINE_BLOCK:
		case CSS_DISPLAY_INLINE_FLEX:
			if (ctx->prev_display &&
			    ctx->prev_display != CSS_DISPLAY_INLINE_BLOCK &&
			    ctx->prev_display != CSS_DISPLAY_INLINE_FLEX) {
				ui_block_layout_next_row(ctx);
			}
			if (max_row_width != -1 && ctx->row->items.length > 0 &&
			    ctx->row->width + child->outer_box.width -
				    max_row_width >
				0.4f) {
				ui_block_layout_next_row(ctx);
			}
			break;
		case CSS_DISPLAY_FLEX:
		case CSS_DISPLAY_BLOCK:
			ui_block_layout_next_row(ctx);
			break;
		default:
			continue;
		}
		ctx->row->width += child->outer_box.width;
		if (child->outer_box.height > ctx->row->height) {
			ctx->row->height = child->outer_box.height;
		}
		list_append(&ctx->row->items, item);
		ctx->prev_display = child->computed_style.type_bits.display;
		ctx->prev = child;
	}
	// TODO: 更准确的计算内容宽高
	ctx->content_width = y_max(ctx->content_width, ctx->row->width);
	ctx->content_height += ctx->row->height;
}

static void ui_block_layout_update_item_margin(ui_block_layout_context_t *ctx,
					       ui_widget_t *w)
{
	css_computed_style_t *s = &w->computed_style;
	css_numeric_value_t space = ctx->content_width - w->border_box.width;

	if (s->type_bits.display == CSS_DISPLAY_BLOCK &&
	    s->type_bits.margin_left == CSS_MARGIN_AUTO) {
		if (s->type_bits.margin_right == CSS_MARGIN_AUTO) {
			CSS_SET_FIXED_LENGTH(s, margin_left, space / 2);
			CSS_SET_FIXED_LENGTH(s, margin_right, space / 2);
		} else {
			CSS_SET_FIXED_LENGTH(s, margin_left,
					     space - s->margin_right);
		}
	}
}

static void ui_block_layout_update_item_position(ui_block_layout_context_t *ctx,
						 ui_widget_t *w, float x,
						 float y)
{
	if (!ui_widget_has_absolute_position(w) &&
	    ui_widget_has_inline_block_display(w)) {
		switch (css_computed_vertical_align(&w->computed_style)) {
		case CSS_VERTICAL_ALIGN_MIDDLE:
			if (!w->parent) {
				break;
			}
			y += (ctx->row->height - w->border_box.width) / 2.f;
			break;
		case CSS_VERTICAL_ALIGN_BOTTOM:
			if (!w->parent) {
				break;
			}
			y += ctx->row->height - w->border_box.width;
		default:
			break;
		}
	}
	// TODO: 考虑删除 layout_x layout_y
	w->layout_x = x;
	w->layout_y = y;
	ui_widget_update_box_position(w);
}

static void ui_block_layout_update(ui_block_layout_context_t *ctx)
{
	float x, y;
	ui_widget_t *w = ctx->widget;
	ui_layout_item_t *item;
	list_node_t *row_node, *item_node;

#ifdef UI_DEBUG_ENABLED
	unsigned row = 0;
#endif

	y = w->computed_style.padding_top;
	if (!ui_widget_is_inline(w)) {
		ctx->content_width = w->content_box.width;
	}
	for (list_each(row_node, &ctx->rows)) {
		ctx->row = row_node->data;
		ctx->row->height = 0;
		x = ctx->widget->computed_style.padding_left;
		for (list_each(item_node, &ctx->row->items)) {
			item = item_node->data;
			ui_widget_prepare_reflow(item->widget, ctx->rule);

#ifdef UI_DEBUG_ENABLED
			{
				UI_WIDGET_STR(item->widget, str);
				UI_WIDGET_SIZE_STR(item->widget, size_str);
				UI_DEBUG_MSG("[%u] %s: old size = %s", row, str,
					     size_str);
			}
#endif
			ui_widget_compute_style(item->widget);
			ui_widget_update_box_size(item->widget);
			ui_layout_diff_auto_reflow(&item->diff, item->widget);
			ui_block_layout_update_item_margin(ctx, item->widget);
			ui_block_layout_update_item_position(ctx, item->widget,
							     x, y);
			ui_layout_diff_end(&item->diff, item->widget);
#ifdef UI_DEBUG_ENABLED
			{
				UI_WIDGET_STR(item->widget, str);
				UI_WIDGET_SIZE_STR(item->widget, size_str);
				UI_DEBUG_MSG("[%u] %s: new size = %s", row, str,
					     size_str);
			}
#endif
			if (ui_widget_in_layout_flow(item->widget)) {
				x += item->widget->outer_box.width;
			}
			ctx->row->height = y_max(item->widget->outer_box.height,
						 ctx->row->height);
		}
		y += ctx->row->height;

#ifdef UI_DEBUG_ENABLED
		row++;
#endif
	}
	ctx->content_height = y - w->computed_style.padding_top;
}

static void ui_block_layout_apply_size(ui_block_layout_context_t *ctx)
{
	float width = 0, height = 0;
	float hint_width = 0, hint_height = 0;
	ui_widget_t *w = ctx->widget;

	width = w->content_box.width;
	height = w->content_box.height;
	hint_width = w->content_box.width;
	hint_height = w->content_box.height;
	switch (ctx->rule) {
	case UI_LAYOUT_RULE_FIXED_WIDTH:
		hint_height = 0;
		w->proto->autosize(w, &hint_width, &hint_height, ctx->rule);
		height = y_max(hint_height, ctx->content_height);
		break;
	case UI_LAYOUT_RULE_FIXED_HEIGHT:
		hint_width = 0;
		w->proto->autosize(w, &hint_width, &hint_height, ctx->rule);
		width = y_max(hint_width, ctx->content_width);
		break;
	case UI_LAYOUT_RULE_MAX_CONTENT:
		hint_width = 0;
		hint_height = 0;
		w->proto->autosize(w, &hint_width, &hint_height, ctx->rule);
		width = y_max(hint_width, ctx->content_width);
		height = y_max(hint_height, ctx->content_height);
		break;
	default:
		break;
	}
	ui_widget_set_content_box_size(w, width, height);
	ctx->rule = UI_LAYOUT_RULE_FIXED;
}

void ui_block_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule)
{
	ui_block_layout_context_t ctx;

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(w, str);
		UI_WIDGET_SIZE_STR(w, size_str);
		UI_DEBUG_MSG("%s: block layout start, size = %s, rule = %d",
			     str, size_str, rule);
		ui_debug_msg_indent++;
	}
#endif

	ctx.rule = rule;
	ctx.row = NULL;
	ctx.widget = w;
	ctx.x = w->computed_style.padding_left;
	ctx.y = w->computed_style.padding_right;
	ctx.content_width = 0;
	ctx.content_height = 0;
	ctx.prev_display = 0;
	ctx.prev = NULL;
	ctx.items = malloc(sizeof(ui_layout_item_t) * (w->children.length + 1));
	list_create(&ctx.rows);
	ui_block_layout_next_row(&ctx);
#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("%s", "start load");
	ui_debug_msg_indent++;
#endif
	ui_block_layout_load(&ctx);
#ifdef UI_DEBUG_ENABLED
	ui_debug_msg_indent--;
	UI_DEBUG_MSG("end load, content_size = (%g, %g)", ctx.content_width,
		     ctx.content_height);
#endif
	ui_block_layout_apply_size(&ctx);
#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("%s", "start update");
	ui_debug_msg_indent++;
#endif
	ui_block_layout_update(&ctx);
#ifdef UI_DEBUG_ENABLED
	ui_debug_msg_indent--;
	UI_DEBUG_MSG("%s", "end update");
#endif
	list_destroy(&ctx.rows, ui_block_layout_row_destroy);
	free(ctx.items);

#ifdef UI_DEBUG_ENABLED
	{
		ui_debug_msg_indent--;
		UI_WIDGET_STR(w, str);
		UI_WIDGET_SIZE_STR(w, size_str);
		UI_DEBUG_MSG("%s: block layout end, size = %s", str, size_str);
	}
#endif
}

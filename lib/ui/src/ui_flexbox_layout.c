/**
 * 弹性布局
 *
 * 布局算法参考 W3C 的文档：https://www.w3.org/TR/css-flexbox/#layout-algorithm
 */

// #define UI_DEBUG_ENABLED
#include "internal.h"

typedef struct ui_flexbox_line_t {
	float main_size;

	float cross_axis;
	float cross_size;
	float max_cross_size;

	float sum_of_grow_value;
	float sum_of_shrink_value;
	size_t count_of_auto_margin_items;

	/** list_t<ui_layout_item_t*> items */
	list_t items;
} ui_flexbox_line_t;

typedef struct ui_flexbox_layout_context_t {
	ui_widget_t *widget;
	ui_layout_rule_t rule;
	LCUI_BOOL is_initiative;
	LCUI_BOOL direction_is_column;

	float main_size;
	float cross_size;

	/** list_t<ui_flexbox_line_t*> lines */
	list_t lines;
	ui_flexbox_line_t *line;
	ui_layout_item_t *items;
} ui_flexbox_layout_context_t;

static ui_flexbox_line_t *ui_flexbox_line_create(void)
{
	ui_flexbox_line_t *line;

	line = malloc(sizeof(ui_flexbox_line_t));
	line->main_size = 0;
	line->cross_size = 0;
	line->sum_of_grow_value = 0;
	line->sum_of_shrink_value = 0;
	line->count_of_auto_margin_items = 0;
	list_create(&line->items);
	return line;
}

static void ui_flexbox_line_destroy(void *arg)
{
	ui_flexbox_line_t *line = arg;

	list_destroy(&line->items, NULL);
	free(line);
}

static void ui_flexbox_line_load_item(ui_flexbox_line_t *line,
				      ui_layout_item_t *item)
{
	if (item->widget->computed_style.flex_grow > 0) {
		line->sum_of_grow_value +=
		    item->widget->computed_style.flex_grow;
	}
	if (item->widget->computed_style.flex_shrink > 0) {
		line->sum_of_shrink_value +=
		    item->widget->computed_style.flex_shrink;
	}
	list_append(&line->items, item);
}

static void ui_flexbox_layout_next_line(ui_flexbox_layout_context_t *ctx)
{
	if (ctx->line) {
		ctx->main_size = y_max(ctx->main_size, ctx->line->main_size);
		ctx->cross_size += ctx->line->cross_size;
	}
	ctx->line = ui_flexbox_line_create();
	list_append(&ctx->lines, ctx->line);
}

INLINE float ui_compute_row_item_main_size(css_computed_style_t *s)
{
	if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
	    if (s->flex_basis < css_padding_x(s) + css_border_x(s)) {
		return 0;
	    }
	    return s->flex_basis - css_padding_x(s) - css_border_x(s);
	}
	return s->flex_basis;
}

INLINE float ui_compute_column_item_main_size(css_computed_style_t *s)
{
	if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
	    if (s->flex_basis < css_padding_y(s) + css_border_y(s)) {
		return 0;
	    }
	    return s->flex_basis - css_padding_y(s) - css_border_y(s);
	}
	return s->flex_basis;
}

static void ui_flexbox_layout_load_rows(ui_flexbox_layout_context_t *ctx)
{
	list_node_t *node;
	ui_widget_t *child;
	ui_layout_item_t *item;
	css_computed_style_t *s = &ctx->widget->computed_style;
	css_computed_style_t *cs;

	float main_size;
	float max_main_size = -1;

	if (ctx->rule == UI_LAYOUT_RULE_FIXED ||
	    ctx->rule == UI_LAYOUT_RULE_FIXED_WIDTH) {
		max_main_size = ctx->widget->content_box.width;
	}
#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("max_main_size = %g", max_main_size);
#endif
	for (item = ctx->items, list_each(node, &ctx->widget->children),
	    ++item) {
		child = node->data;
		item->widget = child;
		cs = &child->computed_style;
		ui_layout_diff_begin(&item->diff, child);
		if (cs->type_bits.display == CSS_DISPLAY_NONE) {
			continue;
		}
		ui_widget_prepare_reflow(child, ctx->rule);
		if (!ui_widget_in_layout_flow(child)) {
			continue;
		}
		ui_widget_compute_style(child);
		ui_widget_update_box_size(child);
		switch (cs->type_bits.flex_basis) {
		case CSS_FLEX_BASIS_CONTENT:
			CSS_SET_FIXED_LENGTH(cs, flex_basis,
					     css_convert_content_box_width(
						 cs, child->max_content_width));
			break;
		default:
			break;
		}
		main_size = ui_compute_row_item_main_size(cs) +
			    css_margin_x(cs) + css_padding_x(cs) +
			    css_border_x(cs);
#ifdef UI_DEBUG_ENABLED
		{
			UI_WIDGET_STR(child, str);
			UI_WIDGET_SIZE_STR(child, size_str);
			UI_DEBUG_MSG("[%lu] %s: size = %s, cross_size = %g, "
				     "main_size = %g, basis_type = %d, "
				     "row[%lu].main_size = %g",
				     item - ctx->items, str, size_str,
				     child->outer_box.height, main_size,
				     cs->type_bits.flex_basis,
				     ctx->lines.length - 1,
				     ctx->line->main_size);
		}
#endif
		/* Check line wrap */
		if (s->type_bits.flex_wrap == CSS_FLEX_WRAP_WRAP &&
		    ctx->line->items.length > 0 && max_main_size != -1) {
			if (ctx->line->main_size + main_size - max_main_size >
			    0.4f) {
				ui_flexbox_layout_next_line(ctx);
			}
		}
		if (child->outer_box.height > ctx->line->cross_size) {
			ctx->line->cross_size = child->outer_box.height;
		}
		if (cs->type_bits.margin_left == CSS_MARGIN_AUTO) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (cs->type_bits.margin_right == CSS_MARGIN_AUTO) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += main_size;
		ui_flexbox_line_load_item(ctx->line, item);
	}
	ctx->main_size = y_max(ctx->main_size, ctx->line->main_size);
	ctx->cross_size += ctx->line->cross_size;
}

static void ui_flexbox_layout_load_columns(ui_flexbox_layout_context_t *ctx)
{
	list_node_t *node;
	ui_widget_t *child;
	ui_layout_item_t *item;
	css_computed_style_t *s = &ctx->widget->computed_style;
	css_computed_style_t *cs;

	float main_size;
	float max_main_size = -1;

	if ((ctx->rule & UI_LAYOUT_RULE_FIXED_HEIGHT) ==
	    UI_LAYOUT_RULE_FIXED_HEIGHT) {
		max_main_size = ctx->widget->content_box.height;
	}

#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("max_main_size: %g", max_main_size);
	UI_DEBUG_MSG("%s", "line start");
	ui_debug_msg_indent++;
#endif

	for (item = ctx->items, list_each(node, &ctx->widget->children),
	    ++item) {
		child = node->data;
		item->widget = child;
		cs = &child->computed_style;
		ui_layout_diff_begin(&item->diff, child);
		if (cs->type_bits.display == CSS_DISPLAY_NONE) {
			continue;
		}
		ui_widget_prepare_reflow(child, ctx->rule);
		if (!ui_widget_in_layout_flow(child)) {
			continue;
		}
		ui_widget_compute_style(child);
		ui_widget_update_box_size(child);
		if (cs->type_bits.flex_basis == CSS_FLEX_BASIS_CONTENT) {
			CSS_SET_FIXED_LENGTH(
			    cs, flex_basis,
			    css_convert_content_box_height(
				cs, child->max_content_height));
		}
		main_size = ui_compute_column_item_main_size(cs) +
			    css_margin_y(cs) + css_padding_y(cs) +
			    css_border_y(cs);
#ifdef UI_DEBUG_ENABLED
		{
			UI_WIDGET_STR(child, str);
			UI_DEBUG_MSG("[%lu] %s: column[%lu].main_size = %g, "
				     "main_size = %g",
				     item - ctx->items, str,
				     ctx->lines.length - 1,
				     ctx->line->main_size, main_size);
		}
#endif
		if (s->type_bits.flex_wrap == CSS_FLEX_WRAP_WRAP &&
		    ctx->line->items.length > 0 && max_main_size != -1) {
			if (ctx->line->main_size + main_size - max_main_size >
			    0.4f) {
#ifdef UI_DEBUG_ENABLED
				ui_debug_msg_indent--;
				UI_DEBUG_MSG("%s", "line end");
#endif
				ui_flexbox_layout_next_line(ctx);
#ifdef UI_DEBUG_ENABLED
				UI_DEBUG_MSG("%s", "line start");
				ui_debug_msg_indent++;
#endif
			}
		}
		if (child->outer_box.width > ctx->line->cross_size) {
			ctx->line->cross_size = child->outer_box.width;
		}
		if (cs->type_bits.margin_top == CSS_MARGIN_AUTO) {
			ctx->line->count_of_auto_margin_items++;
		}
		if (cs->type_bits.margin_bottom == CSS_MARGIN_AUTO) {
			ctx->line->count_of_auto_margin_items++;
		}
		ctx->line->main_size += main_size;
		ui_flexbox_line_load_item(ctx->line, item);
	}
#ifdef UI_DEBUG_ENABLED
	ui_debug_msg_indent--;
	UI_DEBUG_MSG("%s", "line end");
#endif
	ctx->main_size = y_max(ctx->main_size, ctx->line->main_size);
	ctx->cross_size += ctx->line->cross_size;
}

static void ui_flexbox_layout_load(ui_flexbox_layout_context_t *ctx)
{
	if (ctx->direction_is_column) {
		ui_flexbox_layout_load_columns(ctx);
	} else {
		ui_flexbox_layout_load_rows(ctx);
	}
}

static void ui_flexbox_layout_compute_justify_content(
    ui_flexbox_layout_context_t *ctx, float *start_axis, float *space)
{
	float free_space;

	if (ctx->direction_is_column) {
		*start_axis = ctx->widget->computed_style.padding_top;
		free_space = ctx->widget->content_box.height;
	} else {
		*start_axis = ctx->widget->computed_style.padding_left;
		free_space = ctx->widget->content_box.width;
	}
	*space = 0;
	free_space -= ctx->line->main_size;
	switch (ctx->widget->computed_style.type_bits.justify_content) {
	case CSS_JUSTIFY_CONTENT_SPACE_BETWEEN:
		if (ctx->line->items.length > 1) {
			*space = free_space / (ctx->line->items.length - 1);
		}
		*start_axis -= *space;
		break;
	case CSS_JUSTIFY_CONTENT_SPACE_AROUND:
		*space = free_space / ctx->line->items.length;
		*start_axis -= *space * 0.5f;
		break;
	case CSS_JUSTIFY_CONTENT_SPACE_EVENLY:
		*space = free_space / (ctx->line->items.length + 1);
		*start_axis += *space;
		break;
	case CSS_JUSTIFY_CONTENT_FLEX_END:
		*start_axis += free_space;
		break;
	case CSS_JUSTIFY_CONTENT_CENTER:
		*start_axis += free_space * 0.5f;
		break;
	default:
		break;
	}
}

static void ui_flexbox_layout_update_row(ui_flexbox_layout_context_t *ctx)
{
	ui_layout_item_t *item;
	css_computed_style_t *s;
	list_node_t *node;

	float k = 0;
	float main_axis = 0;
	float main_size;
	uint8_t align_items = ctx->widget->computed_style.type_bits.align_items;
	float space = ctx->widget->content_box.width - ctx->line->main_size;

	if (space >= 0) {
		if (ctx->line->sum_of_grow_value > 0) {
			k = space / ctx->line->sum_of_grow_value;
		}
	} else if (ctx->line->sum_of_shrink_value > 0) {
		k = space / ctx->line->sum_of_shrink_value;
	}

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(ctx->widget, str);
		UI_DEBUG_MSG("%s: cross_axis = %g, max_cross_size = %g, "
			     "free_space = %g, alloc "
			     "free space",
			     str, ctx->line->cross_axis,
			     ctx->line->max_cross_size, space);
	}
#endif

	for (list_each(node, &ctx->line->items)) {
		item = node->data;
		s = &item->widget->computed_style;
		ui_widget_compute_style(item->widget);
		main_size = ui_compute_row_item_main_size(s);
		if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
			main_size += css_border_x(s) + css_padding_x(s);
		}
		// 根据项目的 flex-grow 和 flex-shrink，调整它的宽度
		if (space >= 0) {
			if (s->type_bits.flex_grow > 0) {
				CSS_SET_FIXED_LENGTH(
				    s, width, main_size + k * s->flex_grow);
			}
		} else if (s->type_bits.flex_shrink > 0) {
			CSS_SET_FIXED_LENGTH(s, width,
					     main_size + k * s->flex_shrink);
		}
		// 计算拉伸的高度
		if (align_items == CSS_ALIGN_ITEMS_STRETCH &&
		    s->type_bits.height == CSS_HEIGHT_AUTO) {
			CSS_SET_FIXED_LENGTH(s, height,
					     css_convert_border_box_height(
						 s, ctx->line->max_cross_size -
							css_margin_y(s)));
		}
		ui_widget_update_box_size(item->widget);
		ui_layout_diff_auto_reflow(&item->diff, item->widget);
		main_axis += item->widget->outer_box.width;
	}
	ctx->line->main_size = main_axis;
	space = ctx->widget->content_box.width - main_axis;

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(ctx->widget, str);
		UI_DEBUG_MSG("%s: free_space = %g, auto_margin_items = %lu",
			     str, space, ctx->line->count_of_auto_margin_items);
	}
#endif

	// 计算 marign-left 和 margin-right
	if (space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = space / ctx->line->count_of_auto_margin_items;
		for (list_each(node, &ctx->line->items)) {
			item = node->data;
			s = &item->widget->computed_style;
			if (s->type_bits.margin_left == CSS_MARGIN_AUTO) {
				CSS_SET_FIXED_LENGTH(s, margin_left, k);
			}
			if (s->type_bits.margin_right == CSS_MARGIN_AUTO) {
				CSS_SET_FIXED_LENGTH(s, margin_right, k);
			}
			ui_widget_update_box_size(item->widget);
			main_axis += item->widget->outer_box.width;
		}
		ctx->line->main_size = main_axis;
	}

	// 根据 justify-content 和 align-items，更新每个项目的位置和尺寸
	ui_flexbox_layout_compute_justify_content(ctx, &main_axis, &space);
	for (list_each(node, &ctx->line->items)) {
		main_axis += space;
		item = node->data;
		item->widget->layout_x = main_axis;
		item->widget->layout_y = ctx->line->cross_axis;
		switch (align_items) {
		case CSS_ALIGN_ITEMS_CENTER:
			item->widget->layout_y +=
			    (ctx->line->max_cross_size -
			     item->widget->outer_box.height) *
			    0.5f;
			break;
		case CSS_ALIGN_ITEMS_FLEX_END:
			item->widget->layout_y +=
			    ctx->line->max_cross_size -
			    item->widget->outer_box.height;
			break;
		default:
			break;
		}
		ui_widget_update_box_position(item->widget);
		main_axis += item->widget->outer_box.width;
	}
}

static void ui_flexbox_layout_update_column(ui_flexbox_layout_context_t *ctx)
{
	ui_layout_item_t *item;
	css_computed_style_t *s;
	list_node_t *node;

	float k = 0;
	float main_axis = 0;
	float main_size;
	uint8_t align_items = ctx->widget->computed_style.type_bits.align_items;
	float space = ctx->widget->content_box.height - ctx->line->main_size;

	if (space >= 0) {
		if (ctx->line->sum_of_grow_value > 0) {
			k = space / ctx->line->sum_of_grow_value;
		}
	} else if (ctx->line->sum_of_shrink_value > 0) {
		k = space / ctx->line->sum_of_shrink_value;
	}

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(ctx->widget, str);
		UI_DEBUG_MSG("%s: cross_axis = %g, max_cross_size = %g, "
			     "free_space = %g, alloc "
			     "free space",
			     str, ctx->line->cross_axis,
			     ctx->line->max_cross_size, space);
	}
#endif

	for (list_each(node, &ctx->line->items)) {
		item = node->data;
		s = &item->widget->computed_style;
		ui_widget_compute_style(item->widget);
		main_size = ui_compute_column_item_main_size(s);
		if (css_computed_box_sizing(s) == CSS_BOX_SIZING_BORDER_BOX) {
			main_size += css_border_y(s) + css_padding_y(s);
		}
		// 根据项目的 flex-grow 和 flex-shrink，调整它的高度
		if (space >= 0) {
			if (s->type_bits.flex_grow > 0) {
				CSS_SET_FIXED_LENGTH(
				    s, height, main_size + k * s->flex_grow);
			}
		} else if (s->type_bits.flex_shrink > 0) {
			CSS_SET_FIXED_LENGTH(s, height,
					     main_size + k * s->flex_shrink);
		}
		// 计算拉伸的宽度
		if (align_items == CSS_ALIGN_ITEMS_STRETCH &&
		    s->type_bits.height == CSS_HEIGHT_AUTO) {
			CSS_SET_FIXED_LENGTH(s, width,
					     css_convert_border_box_height(
						 s, ctx->line->max_cross_size -
							css_margin_x(s)));
		}
		ui_widget_update_box_size(item->widget);
		ui_layout_diff_auto_reflow(&item->diff, item->widget);
		main_axis += item->widget->outer_box.height;
	}
	ctx->line->main_size = main_axis;
	space = ctx->widget->content_box.height - main_axis;

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(ctx->widget, str);
		UI_DEBUG_MSG("%s: free_space = %g, auto_margin_items = %lu",
			     str, space, ctx->line->count_of_auto_margin_items);
	}
#endif

	// 计算 marign-top 和 margin-bottom
	if (space > 0 && ctx->line->count_of_auto_margin_items > 0) {
		main_axis = 0;
		k = space / ctx->line->count_of_auto_margin_items;
		for (list_each(node, &ctx->line->items)) {
			item = node->data;
			s = &item->widget->computed_style;
			if (s->type_bits.margin_top == CSS_MARGIN_AUTO) {
				CSS_SET_FIXED_LENGTH(s, margin_top, k);
			}
			if (s->type_bits.margin_bottom == CSS_MARGIN_AUTO) {
				CSS_SET_FIXED_LENGTH(s, margin_bottom, k);
			}
			ui_widget_update_box_size(item->widget);
			main_axis += item->widget->outer_box.height;
		}
		ctx->line->main_size = main_axis;
	}

	// 根据 justify-content 和 align-items，更新每个项目的位置和尺寸
	ui_flexbox_layout_compute_justify_content(ctx, &main_axis, &space);
	for (list_each(node, &ctx->line->items)) {
		main_axis += space;
		item = node->data;
		item->widget->layout_y = main_axis;
		item->widget->layout_x = ctx->line->cross_axis;
		switch (align_items) {
		case CSS_ALIGN_ITEMS_CENTER:
			item->widget->layout_x +=
			    (ctx->line->max_cross_size -
			     item->widget->outer_box.width) *
			    0.5f;
			break;
		case CSS_ALIGN_ITEMS_FLEX_END:
			item->widget->layout_x += ctx->line->max_cross_size -
						  item->widget->outer_box.width;
			break;
		default:
			break;
		}
		ui_widget_update_box_position(item->widget);
		main_axis += item->widget->outer_box.height;
	}
}

static void ui_flexbox_layout_update(ui_flexbox_layout_context_t *ctx)
{
	list_node_t *node;
	ui_layout_item_t *item;

	float cross_axis;
	float cross_space = 0;

	if (ctx->direction_is_column) {
		cross_axis = ctx->widget->computed_style.padding_left;
		cross_space = ctx->widget->content_box.width;
	} else {
		cross_axis = ctx->widget->computed_style.padding_top;
		cross_space = ctx->widget->content_box.height;
	}
	cross_space = (cross_space - ctx->cross_size) / ctx->lines.length;
	for (list_each(node, &ctx->lines)) {
		ctx->line = node->data;
		ctx->line->cross_axis = cross_axis;
		ctx->line->max_cross_size = ctx->line->cross_size + cross_space;
		if (ctx->direction_is_column) {
			ui_flexbox_layout_update_column(ctx);
		} else {
			ui_flexbox_layout_update_row(ctx);
		}
		cross_axis += ctx->line->max_cross_size;
	}
	for (item = ctx->items; item->widget; ++item) {
		if (!ui_widget_in_layout_flow(item->widget)) {
			ui_widget_compute_style(item->widget);
			ui_widget_update_box_size(item->widget);
			ui_widget_update_box_position(item->widget);
			ui_layout_diff_auto_reflow(&item->diff, item->widget);
		}
		ui_layout_diff_end(&item->diff, item->widget);
	}
}

static void ui_flexbox_layout_apply_size(ui_flexbox_layout_context_t *ctx)
{
	float width = 0, height = 0;
	ui_widget_t *w = ctx->widget;

	if (ctx->direction_is_column) {
		switch (ctx->rule) {
		case UI_LAYOUT_RULE_FIXED:
			width = w->content_box.width;
			height = w->content_box.height;
			break;
		case UI_LAYOUT_RULE_FIXED_WIDTH:
			width = w->content_box.width;
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = w->content_box.width;
			height = y_max(height, ctx->main_size);
			break;
		case UI_LAYOUT_RULE_FIXED_HEIGHT:
			height = w->content_box.height;
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = w->content_box.height;
			width = y_max(width, ctx->cross_size);
			break;
		default:
			w->proto->autosize(w, &width, &height, ctx->rule);
			height = y_max(height, ctx->main_size);
			width = y_max(width, ctx->cross_size);
			break;
		}
	} else {
		switch (ctx->rule) {
		case UI_LAYOUT_RULE_FIXED:
			width = w->content_box.width;
			height = w->content_box.height;
			break;
		case UI_LAYOUT_RULE_FIXED_WIDTH:
			width = w->content_box.width;
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = w->content_box.width;
			height = y_max(height, ctx->cross_size);
			break;
		case UI_LAYOUT_RULE_FIXED_HEIGHT:
			height = w->content_box.height;
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = y_max(width, ctx->main_size);
			height = w->content_box.height;
			break;
		default:
			w->proto->autosize(w, &width, &height, ctx->rule);
			width = y_max(width, ctx->main_size);
			height = y_max(height, ctx->cross_size);
			break;
		}
	}
	ui_widget_set_content_box_size(w, width, height);
	ctx->rule = UI_LAYOUT_RULE_FIXED;
}

void ui_flexbox_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule)
{
	css_computed_style_t *s = &w->computed_style;
	ui_flexbox_layout_context_t ctx;

#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_STR(w, str);
		UI_WIDGET_SIZE_STR(w, size_str);
		UI_DEBUG_MSG("%s: flexbox layout start, size = %s, rule = %d",
			     str, size_str, rule);
		ui_debug_msg_indent++;
	}
#endif

	ctx.direction_is_column =
	    s->type_bits.flex_direction == CSS_FLEX_DIRECTION_COLUMN;
	ctx.rule = rule;
	ctx.line = NULL;
	ctx.widget = w;
	ctx.main_size = 0;
	ctx.cross_size = 0;
	ctx.items = malloc(sizeof(ui_layout_item_t) * (w->children.length + 1));
	ctx.items[w->children.length].widget = NULL;
	list_create(&ctx.lines);

	ui_flexbox_layout_next_line(&ctx);
#ifdef UI_DEBUG_ENABLED
	UI_DEBUG_MSG("%s", "start load");
	ui_debug_msg_indent++;
#endif
	ui_flexbox_layout_load(&ctx);
#ifdef UI_DEBUG_ENABLED
	ui_debug_msg_indent--;
	UI_DEBUG_MSG("end load, main_size: %g, cross_size: %g", ctx.main_size,
		     ctx.cross_size);
#endif
	ui_flexbox_layout_apply_size(&ctx);
#ifdef UI_DEBUG_ENABLED
	{
		UI_WIDGET_SIZE_STR(w, size_str);
		UI_DEBUG_MSG("start update, size = %s", size_str);
		ui_debug_msg_indent++;
	}
#endif
	ui_flexbox_layout_update(&ctx);
#ifdef UI_DEBUG_ENABLED
	ui_debug_msg_indent--;
	UI_DEBUG_MSG("%s", "end update");
#endif
	list_destroy(&ctx.lines, ui_flexbox_line_destroy);
	free(ctx.items);

#ifdef UI_DEBUG_ENABLED
	{
		ui_debug_msg_indent--;
		UI_WIDGET_STR(w, str);
		UI_WIDGET_SIZE_STR(w, size_str);
		UI_DEBUG_MSG("%s: flexbox layout end, size = %s", str,
			     size_str);
	}
#endif
}

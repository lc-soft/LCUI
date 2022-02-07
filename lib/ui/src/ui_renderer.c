#include <LCUI.h>
#include <LCUI/pandagl.h>
#include "../include/ui.h"
#include "internal.h"

//#define DEBUG_FRAME_RENDER
#define MAX_VISIBLE_WIDTH 20000
#define MAX_VISIBLE_HEIGHT 20000

#ifdef DEBUG_FRAME_RENDER
#include <LCUI/image.h>
#endif

typedef struct ui_renderer_t {
	/* target widget position, it relative to root canvas */
	float x, y;

	/* content area top spacing, it relative to widget canvas */
	float content_top;

	/* content area left spacing, it relative to widget canvas */
	float content_left;

	/* target widget */
	ui_widget_t *target;

	/* computed actual style */
	ui_widget_actual_style_t *style;

	/* current target widget paint context */
	pd_paint_context_t* paint;

	/* root paint context */
	pd_paint_context_t* root_paint;

	/* content canvas */
	pd_canvas_t content_graph;

	/* target widget canvas */
	pd_canvas_t self_graph;

	/* layer canvas, used to mix content and self canvas with widget
	 * opacity */
	pd_canvas_t layer_graph;

	/* actual paint rectangle in widget canvas rectangle, it relative to
	 * root canvas */
	pd_rect_t actual_paint_rect;

	/* actual paint rectangle in widget content rectangle, it relative to
	 * root canvas */
	pd_rect_t actual_content_rect;

	/* Paint rectangle in widget content rectangle, it relative to
	 * root canvas */
	ui_rect_t content_rect;

	LCUI_BOOL has_content_graph;
	LCUI_BOOL has_self_graph;
	LCUI_BOOL has_layer_graph;
	LCUI_BOOL can_render_self;
	LCUI_BOOL can_render_centent;
} ui_renderer_t;

/** 判断部件是否有可绘制内容 */
static LCUI_BOOL ui_widget_is_paintable(ui_widget_t *w)
{
	const ui_widget_style_t *s = &w->computed_style;
	if (s->background.color.alpha > 0 ||
	    pd_canvas_is_valid(s->background.image) || s->border.top.width > 0 ||
	    s->border.right.width > 0 || s->border.bottom.width > 0 ||
	    s->border.left.width > 0 || s->shadow.blur > 0 ||
	    s->shadow.spread > 0) {
		return TRUE;
	}
	return w->proto != ui_get_widget_prototype(NULL);
}

static LCUI_BOOL ui_widget_has_round_border(ui_widget_t *w)
{
	const ui_border_style_t *s = &w->computed_style.border;

	return s->top_left_radius || s->top_right_radius ||
	       s->bottom_left_radius || s->bottom_right_radius;
}

LCUI_BOOL ui_widget_mark_dirty_rect(ui_widget_t *w, ui_rect_t *in_rect,
				    int box_type)
{
	ui_rect_t rect;
	ui_dirty_rect_type_t type;

	if (!w->computed_style.visible) {
		return FALSE;
	}
	if (!in_rect) {
		switch (box_type) {
		case CSS_KEYWORD_BORDER_BOX:
			type = UI_DIRTY_RECT_TYPE_BORDER_BOX;
			break;
		case CSS_KEYWORD_GRAPH_BOX:
			type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
			break;
		case CSS_KEYWORD_PADDING_BOX:
		case CSS_KEYWORD_CONTENT_BOX:
		default:
			type = UI_DIRTY_RECT_TYPE_PADDING_BOX;
			break;
		}
		if (w->dirty_rect_type >= type) {
			return FALSE;
		}
		w->dirty_rect_type = type;
		while (w->parent) {
			w->parent->has_child_dirty_rect = TRUE;
			w = w->parent;
		}
		return TRUE;
	}

	rect = *in_rect;
	switch (box_type) {
	case CSS_KEYWORD_GRAPH_BOX:
		if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_CANVAS_BOX) {
			return FALSE;
		}
		ui_rect_correct(&rect, w->box.canvas.width,
				       w->box.canvas.height);
		break;
	case CSS_KEYWORD_BORDER_BOX:
		if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_BORDER_BOX) {
			return FALSE;
		}
		ui_rect_correct(&rect, w->box.border.width,
				       w->box.border.height);
		rect.x += w->box.border.x - w->box.canvas.x;
		rect.y += w->box.border.y - w->box.canvas.y;
		break;
	case CSS_KEYWORD_PADDING_BOX:
		if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_PADDING_BOX) {
			return FALSE;
		}
		ui_rect_correct(&rect, w->box.padding.width,
				       w->box.padding.height);
		rect.x += w->box.padding.x - w->box.canvas.x;
		rect.y += w->box.padding.y - w->box.canvas.y;
		break;
	case CSS_KEYWORD_CONTENT_BOX:
	default:
		ui_rect_correct(&rect, w->box.content.width,
				       w->box.content.height);
		rect.x += w->box.content.x - w->box.canvas.x;
		rect.y += w->box.content.y - w->box.canvas.y;
		break;
	}
	rect.x += w->box.canvas.x;
	rect.y += w->box.canvas.y;
	if (w->dirty_rect_type > UI_DIRTY_RECT_TYPE_NONE) {
		ui_rect_merge(&w->dirty_rect, &rect, &w->dirty_rect);
	} else {
		w->dirty_rect = rect;
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CUSTOM;
		while (w->parent) {
			w->parent->has_child_dirty_rect = TRUE;
			w = w->parent;
		}
	}
	return TRUE;
}

#define add_dirty_rect()                                               \
	do {                                                           \
		rect.x += x;                                           \
		rect.y += y;                                           \
		ui_rect_overlap(&rect, &visible_area, &rect); \
		if (rect.width > 0 && rect.height > 0) {               \
			actual_rect = malloc(sizeof(pd_rect_t));       \
			ui_compute_rect_actual(actual_rect, &rect);    \
			list_append(rects, actual_rect);         \
		}                                                      \
	} while (0)

static void ui_widget_collect_dirty_rect(ui_widget_t *w, list_t *rects,
					 float x, float y,
					 ui_rect_t visible_area)
{
	ui_rect_t rect;
	pd_rect_t *actual_rect;
	list_node_t *node;

	if (w->parent &&
	    w->parent->dirty_rect_type >= UI_DIRTY_RECT_TYPE_PADDING_BOX) {
		w->dirty_rect_type = UI_DIRTY_RECT_TYPE_CANVAS_BOX;
	} else if (w->dirty_rect_type >= UI_DIRTY_RECT_TYPE_PADDING_BOX) {
		switch (w->dirty_rect_type) {
		case UI_DIRTY_RECT_TYPE_PADDING_BOX:
			rect = w->box.padding;
			break;
		case UI_DIRTY_RECT_TYPE_BORDER_BOX:
			rect = w->box.border;
			break;
		default:
			rect = w->box.canvas;
			break;
		}
		if (!ui_rect_is_cover(&rect, &w->dirty_rect)) {
			add_dirty_rect();
			rect = w->dirty_rect;
			add_dirty_rect();
		} else {
			ui_rect_merge(&rect, &rect, &w->dirty_rect);
			add_dirty_rect();
		}
	} else if (w->dirty_rect_type == UI_DIRTY_RECT_TYPE_CUSTOM) {
		rect = w->dirty_rect;
		add_dirty_rect();
	}
	if (w->has_child_dirty_rect) {
		visible_area.x -= x;
		visible_area.y -= y;
		ui_rect_overlap(&visible_area, &w->box.padding,
					 &visible_area);
		visible_area.x += x;
		visible_area.y += y;
		for (list_each(node, &w->stacking_context)) {
			ui_widget_collect_dirty_rect(
			    node->data, rects, x + w->box.padding.x,
			    y + w->box.padding.y, visible_area);
		}
	}
	w->dirty_rect_type = UI_DIRTY_RECT_TYPE_NONE;
	w->has_child_dirty_rect = FALSE;
}

size_t ui_widget_get_dirty_rects(ui_widget_t *w, list_t *rects)
{
	pd_rect_t *rect;
	list_node_t *node;

	float scale = ui_get_scale();
	int x = y_iround(w->box.padding.x * scale);
	int y = y_iround(w->box.padding.y * scale);

	ui_widget_collect_dirty_rect(w, rects, 0, 0, w->box.padding);
	for (list_each(node, rects)) {
		rect = node->data;
		rect->x -= x;
		rect->y -= y;
	}
	return rects->length;
}

/** 当前部件的绘制函数 */
static void ui_widget_on_paint(ui_widget_t *w, pd_paint_context_t *paint,
			       ui_widget_actual_style_t *style)
{
	ui_widget_paint_background(w, paint, style);
	ui_widget_paint_border(w, paint, style);
	ui_widget_paint_box_shadow(w, paint, style);
	if (w->proto && w->proto->paint) {
		w->proto->paint(w, paint, style);
	}
}

static ui_renderer_t *ui_renderer_create(ui_widget_t *w,
					 pd_paint_context_t *paint,
					 ui_widget_actual_style_t *style,
					 ui_renderer_t *parent)
{
	ui_renderer_t *that = malloc(sizeof(ui_renderer_t));

	if (!that) {
		return NULL;
	}
	that->target = w;
	that->style = style;
	that->paint = paint;
	that->has_self_graph = FALSE;
	that->has_layer_graph = FALSE;
	that->has_content_graph = FALSE;
	if (parent) {
		that->root_paint = parent->root_paint;
		that->x = parent->x + parent->content_left + w->box.canvas.x;
		that->y = parent->y + parent->content_top + w->box.canvas.y;
	} else {
		that->x = that->y = 0;
		that->root_paint = that->paint;
	}
	if (w->computed_style.opacity < 1.0) {
		that->has_self_graph = TRUE;
		that->has_content_graph = TRUE;
		that->has_layer_graph = TRUE;
	} else if (ui_widget_has_round_border(w)) {
		that->has_content_graph = TRUE;
	}
	pd_canvas_init(&that->self_graph);
	pd_canvas_init(&that->layer_graph);
	pd_canvas_init(&that->content_graph);
	that->layer_graph.color_type = PD_COLOR_TYPE_ARGB;
	that->can_render_self = ui_widget_is_paintable(w);
	if (that->can_render_self) {
		that->self_graph.color_type = PD_COLOR_TYPE_ARGB;
		pd_canvas_create(&that->self_graph, that->paint->rect.width,
			     that->paint->rect.height);
	}
	/* get content rectangle left spacing and top */
	that->content_left = w->box.padding.x - w->box.canvas.x;
	that->content_top = w->box.padding.y - w->box.canvas.y;
	/* convert position of paint rectangle to root canvas relative */
	that->actual_paint_rect.x =
	    that->style->canvas_box.x + that->paint->rect.x;
	that->actual_paint_rect.y =
	    that->style->canvas_box.y + that->paint->rect.y;
	that->actual_paint_rect.width = that->paint->rect.width;
	that->actual_paint_rect.height = that->paint->rect.height;
	DEBUG_MSG("[%s] paint_rect: (%d, %d, %d, %d)\n", w->id,
		  that->actual_paint_rect.x, that->actual_paint_rect.y,
		  that->actual_paint_rect.width,
		  that->actual_paint_rect.height);
	/* get actual paint rectangle in widget content rectangle */
	that->can_render_centent = pd_rect_overlap(
	    &that->style->padding_box, &that->actual_paint_rect,
	    &that->actual_content_rect);
	;
	ui_convert_rect(&that->actual_content_rect, &that->content_rect,
			 1.0f / ui_get_scale());
	DEBUG_MSG("[%s] content_rect: (%d, %d, %d, %d)\n", w->id,
		  that->actual_content_rect.x, that->actual_content_rect.y,
		  that->actual_content_rect.width,
		  that->actual_content_rect.height);
	DEBUG_MSG("[%s] content_box: (%d, %d, %d, %d)\n", w->id,
		  style->content_box.x, style->content_box.y,
		  style->content_box.width, style->content_box.height);
	DEBUG_MSG("[%s] border_box: (%d, %d, %d, %d)\n", w->id,
		  style->border_box.x, style->border_box.y,
		  style->border_box.width, style->border_box.height);
	DEBUG_MSG(
	    "[%s][%d/%d] content_rect: (%d,%d,%d,%d), "
	    "canvas_rect: (%d,%d,%d,%d)\n",
	    w->id, w->index, w->parent ? w->parent->stacking_context.length : 1,
	    that->actual_content_rect.x, that->actual_content_rect.y,
	    that->actual_content_rect.width, that->actual_content_rect.height,
	    that->paint->canvas.quote.left, that->paint->canvas.quote.top,
	    that->paint->canvas.width, that->paint->canvas.height);
	if (!that->can_render_centent) {
		return that;
	}
	if (that->has_content_graph) {
		that->content_graph.color_type = PD_COLOR_TYPE_ARGB;
		pd_canvas_create(&that->content_graph,
			     that->actual_content_rect.width,
			     that->actual_content_rect.height);
	}
	return that;
}

static void ui_renderer_destroy(ui_renderer_t *renderer)
{
	pd_canvas_destroy(&renderer->layer_graph);
	pd_canvas_destroy(&renderer->self_graph);
	pd_canvas_destroy(&renderer->content_graph);
	free(renderer);
}

static size_t ui_renderer_render(ui_renderer_t *renderer);

static size_t ui_renderer_render_children(ui_renderer_t *that)
{
	size_t total = 0, count = 0;
	ui_widget_t *child;
	pd_rect_t paint_rect;
	ui_rect_t child_rect;
	list_node_t *node;
	pd_paint_context_t child_paint;
	ui_renderer_t *renderer;
	ui_widget_actual_style_t style;

	/* Render the child widgets from bottom to top in stack order */
	for (list_each_reverse(node, &that->target->stacking_context)) {
		child = node->data;
		if (!child->computed_style.visible ||
		    child->state != UI_WIDGET_STATE_NORMAL) {
			continue;
		}
		if (that->target->extra &&
		    that->target->extra->rules.max_render_children_count &&
		    count > that->target->extra->rules.max_render_children_count) {
			break;
		}
		/*
		 * The actual style calculation is time consuming, so here we
		 * use the existing properties to determine whether we need to
		 * render.
		 */
		style.x = that->x + that->content_left;
		style.y = that->y + that->content_top;
		child_rect.x = style.x + child->box.canvas.x;
		child_rect.y = style.y + child->box.canvas.y;
		child_rect.width = child->box.canvas.width;
		child_rect.height = child->box.canvas.height;
		if (!ui_rect_overlap(&that->content_rect, &child_rect,
					      &child_rect)) {
			continue;
		}
		ui_widget_compute_border_box_actual(child, &style);
		ui_widget_compute_canvas_box_actual(child, &style);
		DEBUG_MSG("content: %g, %g\n", that->content_left,
			  that->content_top);
		DEBUG_MSG("content rect: (%d, %d, %d, %d)\n",
			  that->actual_content_rect.x,
			  that->actual_content_rect.y,
			  that->actual_content_rect.width,
			  that->actual_content_rect.height);
		DEBUG_MSG("child canvas rect: (%d, %d, %d, %d)\n",
			  style.canvas_box.x, style.canvas_box.y,
			  style.canvas_box.width, style.canvas_box.height);
		if (!pd_rect_overlap(&that->actual_content_rect,
					     &style.canvas_box, &paint_rect)) {
			continue;
		}
		++count;
		ui_widget_compute_padding_box_actual(child, &style);
		ui_widget_compute_content_box_actual(child, &style);
		child_paint.rect = paint_rect;
		child_paint.rect.x -= style.canvas_box.x;
		child_paint.rect.y -= style.canvas_box.y;
		if (that->has_content_graph) {
			child_paint.with_alpha = TRUE;
			paint_rect.x -= that->actual_content_rect.x;
			paint_rect.y -= that->actual_content_rect.y;
			pd_canvas_quote(&child_paint.canvas, &that->content_graph,
				    &paint_rect);
		} else {
			child_paint.with_alpha = that->paint->with_alpha;
			paint_rect.x -= that->actual_paint_rect.x;
			paint_rect.y -= that->actual_paint_rect.y;
			pd_canvas_quote(&child_paint.canvas, &that->paint->canvas,
				    &paint_rect);
		}
		DEBUG_MSG("child paint rect: (%d, %d, %d, %d)\n", paint_rect.x,
			  paint_rect.y, paint_rect.width, paint_rect.height);
		renderer =
		    ui_renderer_create(child, &child_paint, &style, that);
		total += ui_renderer_render(renderer);
		ui_renderer_destroy(renderer);
	}
	return total;
}

static size_t ui_renderer_render(ui_renderer_t *renderer)
{
	size_t count = 0;
	pd_paint_context_t self_paint;
	ui_renderer_t *that = renderer;

	int content_x = that->actual_content_rect.x - that->actual_paint_rect.x;
	int content_y = that->actual_content_rect.y - that->actual_paint_rect.y;

#ifdef DEBUG_FRAME_RENDER
	char filename[256];
	static size_t frame = 0;
#endif
	DEBUG_MSG("[%d] %s: start render\n", that->target->index,
		  that->target->type);
	/* 如果部件有需要绘制的内容 */
	if (that->can_render_self) {
		count += 1;
		self_paint = *that->paint;
		self_paint.with_alpha = TRUE;
		self_paint.canvas = that->self_graph;
		ui_widget_on_paint(that->target, &self_paint, that->style);
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename,
			"frame-%lu-L%d-%s-self-paint-(%d,%d,%d,%d).png",
			frame++, __LINE__, renderer->target->id,
			self_paint.rect.x, self_paint.rect.y,
			self_paint.rect.width, self_paint.rect.height);
		LCUI_WritePNGFile(filename, &self_paint.canvas);
#endif
		/* 若不需要缓存自身位图则直接绘制到画布上 */
		if (!that->has_self_graph) {
			pd_canvas_mix(&that->paint->canvas, &that->self_graph, 0, 0,
				  that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
			pd_canvas_print_info(&that->paint->canvas);
			sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png",
				frame++, __LINE__, renderer->target->id);
			LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
		}
	}
	if (that->can_render_centent) {
		count += ui_renderer_render_children(that);
	}
	if (that->has_content_graph &&
	    ui_widget_has_round_border(that->target)) {
		self_paint.rect = that->actual_content_rect;
		self_paint.rect.x -= that->style->canvas_box.x;
		self_paint.rect.y -= that->style->canvas_box.y;
		self_paint.canvas = that->content_graph;
		ui_widget_crop_content(that->target, &self_paint, that->style);
	}
	if (!that->has_layer_graph) {
		if (that->has_content_graph) {
			pd_canvas_mix(&that->paint->canvas, &that->content_graph,
				  content_x, content_y, TRUE);
		}
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename, "frame-%lu-L%d-%s-canvas.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->paint->canvas);
		sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
		DEBUG_MSG("[%d] %s: end render, count: %lu\n",
			  that->target->index, that->target->type, count);
		return count;
	}
	/* 若需要绘制的是当前部件图层，则先混合部件自身位图和内容位图，得出当
	 * 前部件的图层，然后将该图层混合到输出的位图中
	 */
	if (that->can_render_self) {
		pd_canvas_copy(&that->layer_graph, &that->self_graph);
		pd_canvas_mix(&that->layer_graph, &that->content_graph, content_x,
			  content_y, TRUE);
#ifdef DEBUG_FRAME_RENDER
		sprintf(filename, "frame-%lu-L%d-%s-content-grpah-%d-%d.png",
			frame++, __LINE__, renderer->target->id, content_x,
			content_y);
		LCUI_WritePNGFile(filename, &that->content_graph);
		sprintf(filename, "frame-%lu-L%d-%s-self-graph.png", frame++,
			__LINE__, renderer->target->id);
		LCUI_WritePNGFile(filename, &that->layer_graph);
#endif
	} else {
		pd_canvas_create(&that->layer_graph, that->paint->rect.width,
			     that->paint->rect.height);
		pd_canvas_replace(&that->layer_graph, &that->content_graph,
			      content_x, content_y);
	}
	that->layer_graph.opacity = that->target->computed_style.opacity;
	pd_canvas_mix(&that->paint->canvas, &that->layer_graph, 0, 0,
		  that->paint->with_alpha);
#ifdef DEBUG_FRAME_RENDER
	sprintf(filename, "frame-%lu-%s-layer.png", frame++,
		renderer->target->id);
	LCUI_WritePNGFile(filename, &that->layer_graph);
	sprintf(filename, "frame-%lu-L%d-%s-root-canvas.png", frame++, __LINE__,
		renderer->target->id);
	LCUI_WritePNGFile(filename, &that->root_paint->canvas);
#endif
	DEBUG_MSG("[%d] %s: end render, count: %lu\n", that->target->index,
		  that->target->type, count);
	return count;
}

size_t ui_widget_render(ui_widget_t *w, pd_paint_context_t *paint)
{
	size_t count;
	ui_renderer_t *ctx;
	ui_widget_actual_style_t style;

	/* compute actual canvas box */
	style.x = style.y = 0;
	ui_widget_compute_border_box_actual(w, &style);
	ui_widget_compute_canvas_box_actual(w, &style);
	/* reset widget position to relative paint rect */
	style.x = (float)-style.canvas_box.x;
	style.y = (float)-style.canvas_box.y;
	ui_widget_compute_border_box_actual(w, &style);
	ui_widget_compute_canvas_box_actual(w, &style);
	ui_widget_compute_padding_box_actual(w, &style);
	ui_widget_compute_content_box_actual(w, &style);
	ctx = ui_renderer_create(w, paint, &style, NULL);
	DEBUG_MSG("[%d] %s: start render\n", ctx->target->index,
		  ctx->target->type);
	count = ui_renderer_render(ctx);
	DEBUG_MSG("[%d] %s: end render, count: %lu\n", ctx->target->index,
		  ctx->target->type, count);
	ui_renderer_destroy(ctx);
	return count;
}

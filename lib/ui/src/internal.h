#include <LCUI/css/computed.h>
#include "../include/ui.h"
#include "./ui_debug.h"

typedef struct ui_mutation_connection_t {
	ui_widget_t *widget;
	ui_mutation_observer_t *observer;
	ui_mutation_observer_init_t options;
	list_node_t node;
} ui_mutation_connection_t;

// Metrics

void ui_init_metrics(void);

// Prototype

void ui_init_widget_prototype(void);
void ui_destroy_widget_prototype(void);
void ui_widget_destroy_prototype(ui_widget_t *widget);

// Root

void ui_init_root(void);
void ui_destroy_root(void);

// Base

void ui_widget_add_state(ui_widget_t *w, ui_widget_state_t state);

// Trash

size_t ui_trash_clear(void);
void ui_trash_add(ui_widget_t *w);

// Id

int ui_widget_destroy_id(ui_widget_t *w);
void ui_init_widget_id(void);
void ui_destroy_widget_id(void);

// Attributes

void ui_widget_destroy_attributes(ui_widget_t *w);

// Classes

void ui_widget_destroy_classes(ui_widget_t *w);

// Status

void ui_widget_destroy_status(ui_widget_t *w);

// Background

void ui_widget_paint_background(ui_widget_t *w, pd_context_t *paint,
				ui_widget_actual_style_t *style);

// Border

void ui_widget_paint_border(ui_widget_t *w, pd_context_t *paint,
			    ui_widget_actual_style_t *style);
void ui_widget_crop_content(ui_widget_t *w, pd_context_t *paint,
			    ui_widget_actual_style_t *style);

// Box Shadow

void ui_widget_paint_box_shadow(ui_widget_t *w, pd_context_t *paint,
				ui_widget_actual_style_t *style);

// Box

void ui_widget_update_box_position(ui_widget_t *w);
void ui_widget_update_canvas_box(ui_widget_t *w);
void ui_widget_update_box_size(ui_widget_t *w);
void ui_widget_set_content_box_size(ui_widget_t *w, float width, float height);

// Diff

/** for check widget difference */
typedef struct ui_widget_style_diff_t_ {
	css_computed_style_t style;
	ui_rect_t content_box;
	ui_rect_t padding_box;
	ui_rect_t border_box;
	ui_rect_t canvas_box;
	ui_rect_t outer_box;
	LCUI_BOOL visible;
	LCUI_BOOL should_add_dirty_rect;
} ui_style_diff_t;

typedef struct ui_layout_diff_t_ {
	LCUI_BOOL should_add_dirty_rect;
	ui_rect_t content_box;
	ui_rect_t padding_box;
	ui_rect_t border_box;
	ui_rect_t canvas_box;
	ui_rect_t outer_box;
} ui_layout_diff_t;

typedef struct ui_layout_item_t {
	ui_widget_t *widget;
	ui_layout_diff_t diff;
} ui_layout_item_t;

void ui_style_diff_init(ui_style_diff_t *diff, ui_widget_t *w);
void ui_style_diff_begin(ui_style_diff_t *diff, ui_widget_t *w);
void ui_style_diff_end(ui_style_diff_t *diff, ui_widget_t *w);
void ui_layout_diff_begin(ui_layout_diff_t *diff, ui_widget_t *w);
void ui_layout_diff_auto_reflow(ui_layout_diff_t *diff, ui_widget_t *w);
void ui_layout_diff_end(ui_layout_diff_t *diff, ui_widget_t *w);

// Layout

void ui_block_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule);
void ui_flexbox_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule);

// Updater

void ui_init_updater(void);

// Events

/** 初始化 LCUI 部件的事件系统 */
void ui_init_events(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void ui_destroy_events(void);

// Style

INLINE float padding_x(ui_widget_t *w)
{
	return w->computed_style.padding_left + w->computed_style.padding_right;
}

INLINE float padding_y(ui_widget_t *w)
{
	return w->computed_style.padding_top + w->computed_style.padding_bottom;
}

INLINE float border_x(ui_widget_t *w)
{
	return w->computed_style.border_left_width +
	       w->computed_style.border_right_width;
}

INLINE float border_y(ui_widget_t *w)
{
	return w->computed_style.border_top_width +
	       w->computed_style.border_bottom_width;
}

INLINE float margin_x(ui_widget_t *w)
{
	return w->computed_style.margin_left + w->computed_style.margin_right;
}

INLINE float margin_y(ui_widget_t *w)
{
	return w->computed_style.margin_top + w->computed_style.margin_bottom;
}

INLINE float to_border_box_width(ui_widget_t *w, float content_width)
{
	return content_width + padding_x(w) + border_x(w);
}

INLINE float to_border_box_height(ui_widget_t *w, float content_height)
{
	return content_height + padding_y(w) + border_y(w);
}

INLINE LCUI_BOOL ui_widget_is_inline(ui_widget_t *w)
{
	return w->computed_style.type_bits.display ==
		   CSS_DISPLAY_INLINE_BLOCK ||
	       w->computed_style.type_bits.display == CSS_DISPLAY_INLINE_FLEX;
}

INLINE LCUI_BOOL ui_widget_has_absolute_position(ui_widget_t *w)
{
	return css_computed_position(&w->computed_style) ==
	       CSS_POSITION_ABSOLUTE;
}

INLINE LCUI_BOOL ui_widget_in_layout_flow(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) != CSS_DISPLAY_NONE &&
	       !ui_widget_has_absolute_position(w);
}

INLINE LCUI_BOOL ui_widget_has_block_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) == CSS_DISPLAY_BLOCK;
}

INLINE LCUI_BOOL ui_widget_has_flex_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) == CSS_DISPLAY_FLEX ||
	       css_computed_display(&w->computed_style) ==
		   CSS_DISPLAY_INLINE_FLEX;
}

INLINE LCUI_BOOL ui_widget_has_inline_block_display(ui_widget_t *w)
{
	return css_computed_display(&w->computed_style) ==
	       CSS_DISPLAY_INLINE_BLOCK;
}

INLINE LCUI_BOOL ui_widget_is_flex_item(ui_widget_t *w)
{
	return ui_widget_has_flex_display(w) ||
	       (!ui_widget_has_absolute_position(w) && w->parent &&
		ui_widget_has_flex_display(w->parent));
}

INLINE LCUI_BOOL ui_widget_has_fill_available_width(ui_widget_t *w)
{
	return (ui_widget_has_block_display(w) ||
		ui_widget_has_flex_display(w)) &&
	       !ui_widget_has_absolute_position(w);
}

void ui_widget_destroy_style(ui_widget_t *w);

// CSS

void ui_init_css(void);
void ui_destroy_css(void);

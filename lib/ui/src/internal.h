// Metrics

void ui_init_metrics(void);


// Prototype

void ui_init_widget_prototype(void);
void ui_destroy_widget_prototype(void);
void ui_widget_destroy_prototype(ui_widget_t* widget);

// Root

void ui_init_root(void);
void ui_destroy_root(void);

// Base

void ui_widget_add_state(ui_widget_t* w, ui_widget_state_t state);

// Trash

size_t ui_trash_clear(void);
void ui_trash_add(ui_widget_t* w);

// Id

int ui_widget_destroy_id(ui_widget_t* w);
void ui_init_widget_id(void);
void ui_destroy_widget_id(void);

// Attributes

void ui_widget_destroy_attributes(ui_widget_t* w);

// Classes

void ui_widget_destroy_classes(ui_widget_t* w);

// Status

void ui_widget_destroy_status(ui_widget_t* w);

// Background

void ui_widget_init_background(ui_widget_t* w);
void ui_widget_destroy_background(ui_widget_t* w);
void ui_widget_compute_background_style(ui_widget_t* w);
void ui_widget_compute_background(ui_widget_t* w, pd_background_t* out);
void ui_widget_paint_background(ui_widget_t* w, pd_context_t *paint,
				ui_widget_actual_style_t* style);

// Border

void ui_widget_compute_border_style(ui_widget_t* w);
void ui_widget_compute_border(ui_widget_t* w, pd_border_t* b);
void ui_widget_paint_border(ui_widget_t* w, pd_context_t *paint,
			    ui_widget_actual_style_t* style);
void ui_widget_crop_content(ui_widget_t* w, pd_context_t *paint,
			    ui_widget_actual_style_t* style);

// Box Shadow

void ui_widget_paint_box_shadow(ui_widget_t* w, pd_context_t *paint,
				ui_widget_actual_style_t* style);
void ui_widget_compute_box_shadow(ui_widget_t* w, pd_boxshadow_t* out);
void ui_widget_compute_box_shadow_style(ui_widget_t* w);

// Box

void ui_widget_update_box_position(ui_widget_t* w);
float ui_widget_get_canvas_box_width(ui_widget_t* widget);
float ui_widget_get_canvas_box_height(ui_widget_t* widget);
void ui_widget_update_canvas_box(ui_widget_t* w);
void ui_widget_update_box_size(ui_widget_t* w);
float ui_widget_get_box_shadow_offset_x(ui_widget_t* w);
float ui_widget_get_box_shadow_offset_y(ui_widget_t* w);
void ui_widget_compute_border_box_actual(ui_widget_t* w,
					 ui_widget_actual_style_t* s);
void ui_widget_compute_canvas_box_actual(ui_widget_t* w,
					 ui_widget_actual_style_t* s);
void ui_widget_compute_padding_box_actual(ui_widget_t* w,
					  ui_widget_actual_style_t* s);
void ui_widget_compute_content_box_actual(ui_widget_t* w,
					  ui_widget_actual_style_t* s);

// Diff

/** for check widget difference */
typedef struct ui_widget_style_diff_t_ {
	int z_index;
	int display;
	float left;
	float right;
	float top;
	float bottom;
	float width;
	float height;
	float opacity;
	LCUI_BOOL visible;
	ui_area_t margin;
	ui_area_t padding;
	css_keyword_value_t position;
	ui_border_style_t border;
	ui_boxshadow_style_t shadow;
	ui_background_style_t background;
	ui_widget_box_model_t box;
	ui_flexbox_layout_style_t flex;
	LCUI_BOOL should_add_dirty_rect;
} ui_widget_style_diff_t;

typedef struct ui_widget_layout_diff_t_ {
	LCUI_BOOL should_add_dirty_rect;
	ui_widget_box_model_t box;
} ui_widget_layout_diff_t;

void ui_widget_init_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff);
void ui_widget_begin_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff);
int ui_widget_end_style_diff(ui_widget_t* w, ui_widget_style_diff_t* diff);
void ui_widget_begin_layout_diff(ui_widget_t* w, ui_widget_layout_diff_t* diff);
int ui_widget_end_layout_diff(ui_widget_t* w, ui_widget_layout_diff_t* diff);

// Layout

void ui_block_layout_reflow(ui_widget_t* w, ui_layout_rule_t rule);
void ui_flexbox_layout_reflow(ui_widget_t* w, ui_layout_rule_t rule);

// Updater


void ui_init_updater(void);

// Events

/** 初始化 LCUI 部件的事件系统 */
void ui_init_events(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void ui_destroy_events(void);

// Style

INLINE LCUI_BOOL ui_widget_has_absolute_position(ui_widget_t* w)
{
	return w->computed_style.position == CSS_KEYWORD_ABSOLUTE;
}

INLINE LCUI_BOOL ui_widget_has_block_display(ui_widget_t* w)
{
	return w->computed_style.display == CSS_KEYWORD_BLOCK;
}

INLINE LCUI_BOOL ui_widget_has_flex_display(ui_widget_t* w)
{
	return w->computed_style.display == CSS_KEYWORD_FLEX;
}

INLINE LCUI_BOOL ui_widget_has_inline_block_display(ui_widget_t* w)
{
	return w->computed_style.display == CSS_KEYWORD_INLINE_BLOCK;
}

INLINE LCUI_BOOL ui_widget_has_fill_available_width(ui_widget_t* w)
{
	return (ui_widget_has_block_display(w) ||
		ui_widget_has_flex_display(w)) &&
	       !ui_widget_has_absolute_position(w);
}

INLINE LCUI_BOOL ui_widget_has_scale_size(ui_widget_t* w)
{
	return ui_widget_check_style_type(w, css_key_width, SCALE) ||
	       ui_widget_check_style_type(w, css_key_height, SCALE);
}

INLINE LCUI_BOOL ui_widget_has_valid_flexbox_style(ui_widget_t* w)
{
	return ui_widget_has_flex_display(w) ||
	       (!ui_widget_has_absolute_position(w) && w->parent &&
		ui_widget_has_flex_display(w->parent));
}

void ui_widget_compute_padding_style(ui_widget_t* w);
void ui_widget_compute_margin_style(ui_widget_t* w);
void ui_widget_compute_properties(ui_widget_t* w);
void ui_widget_compute_widget_limit_style(ui_widget_t* w,
					  ui_layout_rule_t rule);
void ui_widget_compute_height_limit_style(ui_widget_t* w,
					  ui_layout_rule_t rule);
void ui_widget_compute_width_style(ui_widget_t* w);
void ui_widget_compute_height_style(ui_widget_t* w);
void ui_widget_compute_size_style(ui_widget_t* w);
void ui_widget_compute_flex_basis_style(ui_widget_t* w);
void ui_widget_compute_visibility_style(ui_widget_t* w);
void ui_widget_compute_display_style(ui_widget_t* w);
void ui_widget_compute_opacity_style(ui_widget_t* w);
void ui_widget_compute_zindex_style(ui_widget_t* w);
void ui_widget_compute_position_style(ui_widget_t* w);
void ui_widget_compute_flex_style(ui_widget_t* w);
void ui_widget_destroy_style(ui_widget_t* w);

// CSS

void ui_init_css(void);
void ui_destroy_css(void);

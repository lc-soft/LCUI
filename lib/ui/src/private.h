
// Prototype

void ui_init_widget_prototype(void);
void ui_destroy_widget_prototype(void);
void ui_widget_destroy_prototype(ui_widget_t* widget);

// Root

void ui_init_root(void);


// Base

void ui_widget_add_state(ui_widget_t* w, ui_widget_state_t state);


// Trash

size_t ui_trash_clear(void);
void ui_trash_add(ui_widget_t *w);


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
void ui_widget_compute_background(ui_widget_t* w, LCUI_Background* out);
void ui_widget_paint_background(ui_widget_t* w, LCUI_PaintContext paint,
				ui_widget_actual_style_t* style);

// Border

void ui_widget_compute_border_style(ui_widget_t* w);
void ui_widget_compute_border(ui_widget_t* w, LCUI_Border* b);
void ui_widget_paint_border(ui_widget_t* w, LCUI_PaintContext paint,
			    ui_widget_actual_style_t* style);
void ui_widget_crop_content(ui_widget_t* w, LCUI_PaintContext paint,
			    ui_widget_actual_style_t* style);

// Box Shadow

void ui_widget_paint_box_shadow(ui_widget_t* w, LCUI_PaintContext paint,
				ui_widget_actual_style_t* style);
void ui_widget_compute_box_shadow(ui_widget_t* w, LCUI_BoxShadow* out);
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
	LCUI_Rect2F margin;
	LCUI_Rect2F padding;
	LCUI_StyleValue position;
	LCUI_BorderStyle border;
	LCUI_BoxShadowStyle shadow;
	LCUI_BackgroundStyle background;
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

void ui_widget_update_block_layout(ui_widget_t* w, ui_layout_rule_t rule);


// Util

INLINE float padding_x(ui_widget_t *w)
{
	return w->padding.left + w->padding.right;
}

INLINE float padding_y(ui_widget_t *w)
{
	return w->padding.top + w->padding.bottom;
}

INLINE float border_x(ui_widget_t *w)
{
	return w->computed_style.border.left.width +
	       w->computed_style.border.right.width;
}

INLINE float border_y(ui_widget_t *w)
{
	return w->computed_style.border.top.width +
	       w->computed_style.border.bottom.width;
}

INLINE float margin_x(ui_widget_t *w)
{
	return w->margin.left + w->margin.right;
}

INLINE float margin_y(ui_widget_t *w)
{
	return w->margin.top + w->margin.bottom;
}

INLINE float to_border_box_width(ui_widget_t *w, float content_width)
{
	return content_width + padding_x(w) + border_x(w);
}

INLINE float to_border_box_height(ui_widget_t *w, float content_height)
{
	return content_height + padding_y(w) + border_y(w);
}

INLINE float ui_widget_get_limited_width(ui_widget_t *w, float width)
{
	if (w->computed_style.max_width > -1 &&
	    width > w->computed_style.max_width) {
		width = w->computed_style.max_width;
	}
	if (w->computed_style.min_width > -1 &&
	    width < w->computed_style.min_width) {
		width = w->computed_style.min_width;
	}
	return width;
}

INLINE float ui_widget_get_limited_height(ui_widget_t *w, float height)
{
	if (w->computed_style.max_height > -1 &&
	    height > w->computed_style.max_height) {
		height = w->computed_style.max_height;
	}
	if (w->computed_style.min_height > -1 &&
	    height < w->computed_style.min_height) {
		height = w->computed_style.min_height;
	}
	return height;
}


// Events

/** 初始化 LCUI 部件的事件系统 */
void ui_init_events(void);

/** 销毁（释放） LCUI 部件的事件系统的相关资源 */
void ui_destroy_events(void);


// Style

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

// Image Loader

void ui_init_image_loader(void);
void ui_destroy_image_loader(void);
void ui_process_image_events(void);

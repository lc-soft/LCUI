
/** for check widget difference */
typedef struct ui_widget_style_diff_t_ {
	css_computed_style_t style;
	ui_rect_t content_box;
	ui_rect_t padding_box;
	ui_rect_t border_box;
	ui_rect_t canvas_box;
	ui_rect_t outer_box;
	bool visible;
	bool should_add_dirty_rect;
} ui_style_diff_t;

typedef struct ui_layout_diff_t_ {
	bool should_add_dirty_rect;
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

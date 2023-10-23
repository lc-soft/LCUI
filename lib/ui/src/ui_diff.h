
typedef struct ui_diff_item_t {
        ui_widget_t *widget;
        ui_rect_t border_box;
        ui_rect_t canvas_box;
        bool visible;
        bool should_add_dirty_rect;
} ui_diff_item_t;

/** for check widget difference */
typedef struct ui_widget_style_diff_t_ {
        css_computed_style_t style;
        ui_rect_t padding_box;
        ui_rect_t canvas_box;
        bool visible;
        bool should_add_dirty_rect;
} ui_style_diff_t;

void ui_style_diff_init(ui_style_diff_t *diff, ui_widget_t *w);
void ui_style_diff_begin(ui_style_diff_t *diff, ui_widget_t *w);
void ui_style_diff_end(ui_style_diff_t *diff, ui_widget_t *w);

// Layout

void ui_block_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule);
void ui_flexbox_layout_reflow(ui_widget_t *w, ui_layout_rule_t rule);

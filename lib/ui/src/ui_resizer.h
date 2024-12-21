#include <ui.h>

typedef struct ui_resizer {
        ui_widget_t *target;
        ui_sizehint_t hint;
        bool wrap;
        float line_min_main_size, line_min_cross_size;
        float line_main_size, line_cross_size;
        float min_main_size, min_cross_size;
        float main_size, cross_size;
        float max_main_size;
} ui_resizer_t;

void ui_widget_get_sizehint(ui_widget_t *w, ui_sizehint_t *hint);
void ui_widget_set_width_fit_content(ui_widget_t *w);
void ui_widget_set_width_fill_available(ui_widget_t *w);
void ui_resizer_load_row_minmaxinfo(ui_resizer_t *resizer);
void ui_resizer_load_column_minmaxinfo(ui_resizer_t *resizer);
void ui_resizer_init(ui_resizer_t *resizer, ui_widget_t *target);
void ui_resizer_update(ui_resizer_t *resizer);
void ui_resizer_load_item_main_size(ui_resizer_t *resizer, float main_size,
                                    float min_main_size);
void ui_resizer_load_item_cross_size(ui_resizer_t *resizer, float cross_size,
                                     float min_cross_size);
void ui_resizer_commit_row_main_size(ui_resizer_t *resizer);
void ui_resizer_commit_column_main_size(ui_resizer_t *resizer);

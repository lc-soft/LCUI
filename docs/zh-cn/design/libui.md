# libui

图形界面库。

## 接口设计

```c
typedef struct ui_event_t ui_event_t;
typedef struct ui_widget_t ui_widget_t;


void ui_init(void);
void ui_free(void);

void ui_update(rect_t **dirty_rects);

// Event

void ui_event_init(ui_event_t *e);
void ui_process_event(ui_event_t *e);

// Metrics

void ui_set_dpi(float dpi);
void ui_set_scale(float scale);
void ui_set_density(float density);
void ui_set_scaled_density(float density);
void ui_set_density_level(ui_density_level_t level);
void ui_set_scaled_density_level(ui_density_level_t level);
float ui_get_scale(void);

// https://developer.mozilla.org/en-US/docs/Web/CSS/length
static inline float ui_compute_length(float value, ui_length_unit_t unit);

// Widget

ui_widget_t *ui_create_widget(const char *type);
ui_widget_t *ui_get_widget(const char *id);
void ui_destroy_widget(ui_widget_t *w);
void ui_widget_empty(ui_widget_t *w);
void ui_widget_offset(ui_widget_t *w, ui_widget_t *parent, float *offset_x, float *offset_y);
void ui_widget_set_title(ui_widget_t *w, const wchar_t *title);

// Widget attribute

int ui_widget_set_attribute(ui_widget_t *w, const char *name, const char *value);
const char *ui_widget_get_attribute_value(ui_widget_t *w, const char *name);
void ui_widget_clear_attributes(ui_widget_t *w);

// Widget tree

void ui_root_append(ui_widget_t *w);
void ui_widget_append(ui_widget_t *w, ui_widget_t *child);
void ui_widget_prepend(ui_widget_t *w, ui_widget_t *child);
void ui_widget_unwrap(ui_widget_t *w);
void ui_widget_unlink(ui_widget_t *w);
void ui_widget_at(ui_widget_t *w, float x, float y);
void ui_widget_print_tree(ui_widget *w);
ui_widget_t *ui_widget_next(ui_widget_t *w);
ui_widget_t *ui_widget_prev(ui_widget_t *w);
ui_widget_t *ui_widget_get_child(ui_widget_t *w, size_t index);

// Widget box model

// CSS

int ui_load_css_file(const char *path);
int ui_load_css_string(const char *str, const char *space);

```

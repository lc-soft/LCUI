# libui

图形界面库。

问题：

- 命名不规范
- 依赖事件循环的函数

待办：

- 背景、边框和阴影的实际样式可改为在重绘时计算，以节省 widget 实例的内存占用
- 一些内部使用的函数没必要公开在头文件中
- 背景图的异步加载功能可以改为独立的图片加载模块，基于异步任务接口实现
- 移除 LCUI_StyleValue
- 样式和样式属性值的操作函数命名不严谨，有待重命名
- 合并性能记录代码

改动：

- `LCUI_PaintContext` -> `ui_painter_t`
- `InvalidArea` -> `InvalidRect`
- 将一些简单函数改为内联函数并移动到头文件中
- 为每个组件任务定义内联函数以简化调用代码，例如：`ui_widget_add_task(w, UI_TASK_REFRESH_STYLE)` 可以简化成 `ui_widget_refresh_style(w)`
- 所有组件任务的执行函数命名由 `ui_widget_exec_*` 改为 `ui_widget_force_*`

## 接口设计

示例：

```c
typedef struct ui_event_t ui_event_t;
typedef struct ui_widget_t ui_widget_t;


void ui_init(void);
void ui_free(void);

void ui_widget_update_with_profile(rect_t **dirty_rects);

// Events

void ui_init_events(void);
void ui_process_events(void);
void ui_destroy_events(void);
void ui_dispatch_event(ui_event_t *e);

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
void ui_delete_widget(ui_widget_t *w);
void ui_widget_remove(ui_widget_t *w);
void ui_widget_destroy(ui_widget_t *w);
void ui_widget_empty(ui_widget_t *w);
void ui_widget_offset(ui_widget_t *w, ui_widget_t *parent, float *offset_x, float *offset_y);
void ui_widget_set_title(ui_widget_t *w, const wchar_t *title);
void ui_widget_get_closest(ui_widget_t *w, const char *type);
Dict *ui_widget_collect_references(ui_widget_t *w);

#define ui_widget_collect_refs ui_widget_collect_references

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
size_t ui_widget_each(ui_widget w, void (*callback)(ui_widget*, void *));

// Widget task

void ui_refresh_style(void);

size_t ui_widget_update_with_profile(ui_widget_t *w);

size_t ui_widget_add_task(ui_widget_t *w, ui_widget_update_t task);

size_t ui_widget_add_task_to_children(ui_widget_t *w, ui_widget_update_t task);

size_t ui_widget_update_task_status(ui_widget_t *w);

size_t ui_widget_update_with_profile(void);

// Widget style

void ui_widget_compute_margin_style(ui_widget *w);
void ui_widget_compute_padding_style(ui_widget *w);
void ui_widget_compute_properties(ui_widget *w);
void ui_widget_compute_widget_limit_style(ui_widget *w, ui_layout_rule_t rule);
void ui_widget_compute_height_limit_style(ui_widget *w, ui_layout_rule_t rule);
void ui_widget_compute_widget_style(ui_widget *w);
void ui_widget_compute_height_style(ui_widget *w);
void ui_widget_compute_size_style(ui_widget *w);
void ui_widget_compute_flex_basis_style(ui_widget *w);
void ui_widget_compute_visibility_style(ui_widget *w);
void ui_widget_compute_opacity_style(ui_widget *w);
void ui_widget_compute_display_style(ui_widget *w);
size_t ui_widget_get_children_style_changes(ui_widget_t *w, int type, const char *name);
void ui_widget_print_style(ui_widget_t *w);
void ui_widget_update_style(ui_widget_t *w);
void ui_widget_refresh_style(ui_widget_t *w);
void ui_widget_destroy_style(ui_widget_t *w);

void ui_load_css_file(const char *filename);
void ui_load_css(const char *css_str, const char *space);
void ui_init_css(void);
void ui_destroy_css(void);

// Widget style helper

void ui_widget_set_padding(ui_widget_t *w, float top, float right, float bottom, float left);
void ui_widget_set_margin(ui_widget_t *w, float top, float right, float bottom, float left);

void ui_widget_set_border_color(ui_widget_t *w, LCUI_Color color);
void ui_widget_set_border_width(ui_widget_t *w, float width);
void ui_widget_set_border_style(ui_widget_t *w, ui_border_style_t style);
void ui_widget_set_border_style(ui_widget_t *w, float width, ui_border_style_t style, LCUI_Color color);
void ui_widget_set_box_shadow(ui_widget_t *w, float x, float y, float blur, LCUI_Color color);
void ui_widget_move(ui_widget_t *w, float y, float y);
void ui_widget_resize(ui_widget_t *w, float width, float height);

ui_style_t *ui_widget_get_style(ui_widget_t *w, css_key_t key);
int ui_widget_unset_style(ui_widget_t *w, css_key_t key);

void ui_widget_set_visibility(ui_widget_t *w, const char *value);
void ui_widget_set_hidden(ui_widget_t *w);
void ui_widget_set_visible(ui_widget_t *w);
void ui_widget_show(ui_widget_t *w);
void ui_widget_hide(ui_widget_t *w);
void ui_widget_set_position(ui_widget_t *w, LCUI_StyleValue position);
void ui_widget_set_opacity(ui_widget_t *w, float opacity);
void ui_widget_set_box_sizing(ui_widget_t *w, LCUI_StyleValue sizing);


// Widget status

void ui_widget_set_disabled(ui_widget_t *w, bool_t disbaled);
void ui_widget_has_status(ui_widget_t *w, const char *status_name);
void ui_widget_add_status(ui_widget_t *w, const char *status_name);
void ui_widget_remove_status(ui_widget_t *w, const char *status_name);
void ui_widget_update_status(ui_widget_t *w);
void ui_widget_destroy_status(ui_widget_t *w);

// Widget class

void ui_widget_has_class(ui_widget_t *w, const char *status_name);
void ui_widget_add_class(ui_widget_t *w, const char *status_name);
void ui_widget_remove_class(ui_widget_t *w, const char *status_name);
void ui_widget_update_class(ui_widget_t *w);
void ui_widget_destroy_classes(ui_widget_t *w);

// Widget render

LCUI_BOOL ui_widget_invalidate_rect(ui_widget_t *w, LCUI_RectF *rect, ui_box_type_t box_type);
size_t ui_widget_get_invalid_rect(ui_widget_t *w, LinkedList *rects);
size_t ui_widget_render(ui_widget_t *w, ui_painter_t *painter);

// Widget layout

void ui_widget_froce_reflow(ui_widget_t *w, ui_layout_rule_t rule);

void ui_widget_reflow(ui_widget_t *w, ui_layout_rule_t rule);

// Widget id

ui_widget_t *ui_get_widget(const char *id);
void ui_widget_set_id(ui_widget_t *w, const char *id_str);
void ui_widget_destroy_id(ui_widget_t *w);
void ui_init_id_map(void);
void ui_destroy_id_map(void);

// Widget hash

void ui_widget_generate_hash(ui_widget_t *w);
void ui_widget_tree_generate_hash(ui_widget_t *w);
size_t ui_widget_set_hash_list(ui_widget_t *w, unsigned *hash_list, size_t len);
size_t ui_widget_get_hash_list(ui_widget_t *w, unsigned *hash_list, size_t len);

// Widget event

void ui_widget_set_mouse_capture(ui_widget_t *w);
void ui_widget_release_mouse_capture(ui_widget_t *w);
void ui_widget_set_touch_capture(ui_widget_t *w, int point_id);
void ui_widget_release_Touch_capture(ui_widget_t *w);
void ui_widget_destroy_event_emitter(ui_widget_t *w);
void ui_set_focus(ui_widget_t *w);
void ui_get_focus(ui_widget_t *w);
void ui_init_event(ui_event_t *e, const char *name);
void ui_init_events(void);
void ui_destroy_events(void);

// Widget diff

void ui_widget_init_style_diff(ui_widget_t *w, ui_widget_style_diff_t *diff);
void ui_widget_begin_style_diff(ui_widget_t *w, ui_widget_style_diff_t *diff);
void ui_widget_end_style_diff(ui_widget_t *w, ui_widget_style_diff_t *diff);

void ui_widget_init_layout_diff(ui_widget_t *w, ui_widget_layout_diff_t* *diff);
void ui_widget_begin_layout_diff(ui_widget_t *w, ui_widget_layout_diff_t* *diff);
void ui_widget_end_layout_diff(ui_widget_t *w, ui_widget_layout_diff_t* *diff);

// widget Prototype

void ui_init_widget_prototype(void);
void ui_destroy_widget_prototype(void);
void ui_get_widget_prototype(const char *name);
ui_widget_prototype_t *ui_add_widget_prototype(const char *name, const char *parent_name);
LCUI_BOOL ui_check_widget_type(ui_widget_t *w, const char *type);
LCUI_BOOL ui_check_widget_prototype(ui_widget_t *w, const ui_widget_prototype_t *proto);
void ui_widget_get_data(ui_widget_t *w, ui_widget_prototype_t *proto);
void ui_widget_add_data(ui_widget_t *w, ui_widget_prototype_t *proto, size_t data_size);
void ui_widget_destroy_prototype(ui_widget_t *w);

// Image loader

void ui_init_image_loader(void);
void ui_destroy_image_loader(void);

// Widget background

void ui_widget_init_background(void);
void ui_widget_destroy_background(void);
void ui_widget_compute_backgorund_style(ui_widget_t *w);
void ui_widget_compute_backgorund(ui_widget_t *w, ui_background_t *bg);
void ui_widget_paint_background(ui_widget_t *w, ui_painter_t painter, ui_widget_actual_style_t style);

// Widget border

void ui_widget_compute_border_style(ui_widget_t *w);
void ui_widget_compute_border(ui_widget_t *w, ui_border_t *border);
void ui_widget_paint_border(ui_widget_t *w, ui_painter_t painter, ui_widget_actual_style_t style);
void ui_widget_crop_content(ui_widget_t *w, ui_painter_t painter, ui_widget_actual_style_t style);

// Widget shadow

void ui_widget_compute_box_shadow_style(ui_widget_t *w);
void ui_widget_compute_box_shadow(ui_widget_t *w, ui_box_shadow_t *shadow);
void ui_widget_paint_box_shadow(ui_widget_t *w, ui_painter_t painter, ui_widget_actual_style_t style);

```

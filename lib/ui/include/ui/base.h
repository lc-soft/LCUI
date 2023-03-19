#ifndef LIB_UI_INCLUDE_UI_BASE_H
#define LIB_UI_INCLUDE_UI_BASE_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC void ui_init(void);
LIBUI_PUBLIC void ui_destroy(void);
LIBUI_PUBLIC int ui_dispatch_event(ui_event_t *e);
LIBUI_PUBLIC void ui_process_events(void);

LIBUI_PUBLIC ui_widget_t *ui_create_widget(const char *type);
LIBUI_PUBLIC ui_widget_t *ui_create_widget_with_prototype(
    const ui_widget_prototype_t *proto);
LIBUI_PUBLIC void ui_widget_destroy(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_set_title(ui_widget_t *w, const wchar_t *title);
LIBUI_PUBLIC void ui_widget_set_text(ui_widget_t *w, const char *text);
LIBUI_PUBLIC void ui_widget_get_offset(ui_widget_t *w, ui_widget_t *parent,
				       float *offset_x, float *offset_y);
LIBUI_PUBLIC bool ui_widget_in_viewport(ui_widget_t *w);

// Root

LIBUI_PUBLIC ui_widget_t *ui_root(void);
LIBUI_PUBLIC int ui_root_append(ui_widget_t *w);

// Id

LIBUI_PUBLIC ui_widget_t *ui_get_widget(const char *id);
LIBUI_PUBLIC int ui_widget_set_id(ui_widget_t *w, const char *idstr);

// Extra Data

LIBUI_PUBLIC ui_widget_extra_data_t *ui_create_extra_data(ui_widget_t *widget);

LIBUI_INLINE ui_widget_extra_data_t *ui_widget_use_extra_data(ui_widget_t *widget)
{
	return widget->extra ? widget->extra : ui_create_extra_data(widget);
}

// Attributes

LIBUI_PUBLIC int ui_widget_set_attribute_ex(ui_widget_t *w, const char *name,
					    void *value, int value_type,
					    void (*value_destructor)(void *));
LIBUI_PUBLIC int ui_widget_set_attribute(ui_widget_t *w, const char *name,
					 const char *value);
LIBUI_PUBLIC const char *ui_widget_get_attribute_value(ui_widget_t *w,
						       const char *name);

// Classes

LIBUI_PUBLIC int ui_widget_add_class(ui_widget_t *w, const char *class_name);
LIBUI_PUBLIC bool ui_widget_has_class(ui_widget_t *w, const char *class_name);
LIBUI_PUBLIC int ui_widget_remove_class(ui_widget_t *w, const char *class_name);

// Status

LIBUI_PUBLIC int ui_widget_add_status(ui_widget_t *w, const char *status_name);
LIBUI_PUBLIC bool ui_widget_has_status(ui_widget_t *w, const char *status_name);
LIBUI_PUBLIC int ui_widget_remove_status(ui_widget_t *w,
					 const char *status_name);
LIBUI_PUBLIC void ui_widget_update_status(ui_widget_t *widget);
LIBUI_PUBLIC void ui_widget_set_disabled(ui_widget_t *w, bool disabled);

// Tree

LIBUI_PUBLIC void ui_widget_remove(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_empty(ui_widget_t *w);
LIBUI_PUBLIC int ui_widget_append(ui_widget_t *parent, ui_widget_t *widget);
LIBUI_PUBLIC int ui_widget_prepend(ui_widget_t *parent, ui_widget_t *widget);
LIBUI_PUBLIC int ui_widget_unwrap(ui_widget_t *widget);
LIBUI_PUBLIC int ui_widget_unlink(ui_widget_t *w);
LIBUI_PUBLIC ui_widget_t *ui_widget_prev(ui_widget_t *w);
LIBUI_PUBLIC ui_widget_t *ui_widget_next(ui_widget_t *w);
LIBUI_PUBLIC ui_widget_t *ui_widget_get_child(ui_widget_t *w, size_t index);
LIBUI_PUBLIC size_t ui_widget_each(ui_widget_t *w,
				   void (*callback)(ui_widget_t *, void *),
				   void *arg);
LIBUI_PUBLIC ui_widget_t *ui_widget_at(ui_widget_t *widget, int ix, int iy);
LIBUI_PUBLIC void ui_print_tree(ui_widget_t *w);

// Helper

LIBUI_PUBLIC bool ui_widget_is_visible(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_move(ui_widget_t *w, float left, float top);
LIBUI_PUBLIC void ui_widget_resize(ui_widget_t *w, float width, float height);
LIBUI_PUBLIC void ui_widget_show(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_hide(ui_widget_t *w);
LIBUI_PUBLIC ui_widget_t *ui_widget_get_closest(ui_widget_t *w,
						const char *type);
LIBUI_PUBLIC dict_t *ui_widget_collect_references(ui_widget_t *w);

// Layout

/**
 * 执行布局前的准备操作
 * 重置布局相关属性，以让它们在布局时被重新计算
 * @param rule 父级组件所使用的布局规则
 */
LIBUI_PUBLIC void ui_widget_prepare_reflow(ui_widget_t *w,
					   ui_layout_rule_t rule);

LIBUI_PUBLIC void ui_widget_reset_size(ui_widget_t *w);

LIBUI_PUBLIC void ui_widget_reflow(ui_widget_t *w);

// Renderer

LIBUI_PUBLIC bool ui_widget_mark_dirty_rect(ui_widget_t *w, ui_rect_t *in_rect,
					    ui_box_type_t box_type);
LIBUI_PUBLIC size_t ui_widget_get_dirty_rects(ui_widget_t *w, list_t *rects);
LIBUI_PUBLIC size_t ui_widget_render(ui_widget_t *w, pd_context_t *paint);

// Updater

LIBUI_PUBLIC size_t ui_widget_update(ui_widget_t *w);
LIBUI_PUBLIC size_t ui_update(void);
LIBUI_PUBLIC void ui_refresh_style(void);

LIBUI_PUBLIC void ui_widget_set_rules(ui_widget_t *w,
				      const ui_widget_rules_t *rules);
LIBUI_PUBLIC void ui_widget_add_task_for_children(ui_widget_t *widget,
						  ui_task_type_t task);
LIBUI_PUBLIC void ui_widget_add_task(ui_widget_t *widget, int task);

LIBUI_INLINE void ui_widget_refresh_style(ui_widget_t *w)
{
	ui_widget_add_task(w, UI_TASK_REFRESH_STYLE);
}

LIBUI_INLINE void ui_widget_update_style(ui_widget_t *w)
{
	ui_widget_add_task(w, UI_TASK_UPDATE_STYLE);
}

LIBUI_END_DECLS

#endif

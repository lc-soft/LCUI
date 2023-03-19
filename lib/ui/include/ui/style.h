#ifndef LIB_UI_INCLUDE_UI_STYLE_H
#define LIB_UI_INCLUDE_UI_STYLE_H

#include "common.h"
#include "types.h"

#include <css/selector.h>

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC void ui_widget_get_style(ui_widget_t *w, int key,
				      css_style_value_t *value);
LIBUI_PUBLIC void ui_widget_set_style(ui_widget_t *w, int key,
				      const css_style_value_t *value);
LIBUI_PUBLIC int ui_widget_unset_style(ui_widget_t *w, int key);
LIBUI_PUBLIC css_selector_node_t *ui_widget_create_selector_node(
    ui_widget_t *w);
LIBUI_PUBLIC css_selector_t *ui_widget_create_selector(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_print_stylesheet(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_update_children_style(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_refresh_children_style(ui_widget_t *w);
LIBUI_PUBLIC int ui_widget_set_style_string(ui_widget_t *w,
					    const char *property,
					    const char *css_text);
LIBUI_PUBLIC void ui_widget_set_style_unit_value(ui_widget_t *w, int key,
						 css_numeric_value_t value,
						 css_unit_t unit);
LIBUI_PUBLIC void ui_widget_set_style_keyword_value(ui_widget_t *w, int key,
						    css_keyword_value_t value);
LIBUI_PUBLIC void ui_widget_set_style_color_value(ui_widget_t *w, int key,
						  css_color_value_t value);
LIBUI_PUBLIC void ui_widget_set_style_numeric_value(ui_widget_t *w, int key,
						    css_numeric_value_t value);
LIBUI_PUBLIC void ui_widget_compute_style(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_force_update_style(ui_widget_t *w);
LIBUI_PUBLIC void ui_widget_force_refresh_style(ui_widget_t *w);

LIBUI_END_DECLS

#endif

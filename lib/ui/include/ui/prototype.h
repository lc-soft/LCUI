#ifndef LIB_UI_INCLUDE_UI_PROTOTYPE_H
#define LIB_UI_INCLUDE_UI_PROTOTYPE_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC ui_widget_prototype_t *ui_create_widget_prototype(
    const char *name, const char *parent_name);
LIBUI_PUBLIC ui_widget_prototype_t *ui_get_widget_prototype(const char *name);
LIBUI_PUBLIC bool ui_check_widget_type(ui_widget_t *w, const char *type);
LIBUI_PUBLIC bool ui_check_widget_prototype(ui_widget_t *w,
					    const ui_widget_prototype_t *proto);
LIBUI_PUBLIC void *ui_widget_get_data(ui_widget_t *widget,
				      ui_widget_prototype_t *proto);
LIBUI_PUBLIC void *ui_widget_add_data(ui_widget_t *widget,
				      ui_widget_prototype_t *proto,
				      size_t data_size);

LIBUI_END_DECLS

#endif

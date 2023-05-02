#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_VIEW_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_VIEW_H

#include <ui.h>
#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC ui_widget_t *ui_router_view_get_matched_widget(
    ui_widget_t *w);

LIBUI_ROUTER_PUBLIC void ui_register_router_view(void);

LIBUI_ROUTER_END_DECLS

#endif

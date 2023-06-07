#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTE_RECORD_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_ROUTE_RECORD_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_route_record_t *router_route_record_create(void);

LIBUI_ROUTER_PUBLIC void router_route_record_destroy(
    router_route_record_t *record);

LIBUI_ROUTER_PUBLIC void router_route_record_set_path(
    router_route_record_t *record, const char *path);

LIBUI_ROUTER_PUBLIC const char *router_route_record_get_component(
    const router_route_record_t *record, const char *key);

LIBUI_ROUTER_END_DECLS

#endif

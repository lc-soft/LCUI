#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_MATCHER_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_MATCHER_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_matcher_t *router_matcher_create(void);

LIBUI_ROUTER_PUBLIC void router_matcher_destroy(router_matcher_t *matcher);

LIBUI_ROUTER_PUBLIC router_route_t *router_matcher_match(
    router_matcher_t *matcher, const router_location_t *raw_location,
    const router_route_t *current_route);

LIBUI_ROUTER_PUBLIC router_route_record_t *router_matcher_add_route_record(
    router_matcher_t *matcher, const router_config_t *config,
    const router_route_record_t *parent);

LIBUI_ROUTER_END_DECLS

#endif

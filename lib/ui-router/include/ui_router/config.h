#ifndef LIB_UI_ROUTER_INCLUDE_UI_ROUTER_CONFIG_H
#define LIB_UI_ROUTER_INCLUDE_UI_ROUTER_CONFIG_H

#include <ui_router/common.h>
#include <ui_router/types.h>

LIBUI_ROUTER_BEGIN_DECLS

LIBUI_ROUTER_PUBLIC router_config_t *router_config_create(void);

LIBUI_ROUTER_PUBLIC void router_config_destroy(router_config_t *config);

LIBUI_ROUTER_PUBLIC void router_config_set_name(router_config_t *config,
                                                const char *name);

LIBUI_ROUTER_PUBLIC void router_config_set_path(router_config_t *config,
                                                const char *path);

LIBUI_ROUTER_PUBLIC void router_config_set_component(router_config_t *config,
                                                     const char *name,
                                                     const char *component);

LIBUI_ROUTER_END_DECLS

#endif

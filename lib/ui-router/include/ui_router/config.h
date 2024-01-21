/*
 * lib/ui-router/include/ui_router/config.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

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

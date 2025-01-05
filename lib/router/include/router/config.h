/*
 * lib/router/include/router/config.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIB_ROUTER_INCLUDE_ROUTER_CONFIG_H
#define LIB_ROUTER_INCLUDE_ROUTER_CONFIG_H

#include <router/common.h>
#include <router/types.h>

LIBROUTER_BEGIN_DECLS

LIBROUTER_PUBLIC router_config_t *router_config_create(void);

LIBROUTER_PUBLIC void router_config_destroy(router_config_t *config);

LIBROUTER_PUBLIC void router_config_set_name(router_config_t *config,
                                                const char *name);

LIBROUTER_PUBLIC void router_config_set_path(router_config_t *config,
                                                const char *path);

LIBROUTER_PUBLIC void router_config_set_component(router_config_t *config,
                                                     const char *name,
                                                     const char *component);

LIBROUTER_END_DECLS

#endif

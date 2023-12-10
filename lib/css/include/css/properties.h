/*
 * lib/css/include/css/properties.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_PROPERTIES_H
#define LIBCSS_INCLUDE_CSS_PROPERTIES_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC int css_register_property(const char *name, const char *syntax,
				   const char *initial_value,
				   int (*cascade)(const css_style_array_value_t,
						  css_computed_style_t *));

LIBCSS_PUBLIC int css_register_shorthand_property(
    const char *name, const char *syntax,
    int (*parse)(css_propdef_t *, const char *, css_style_decl_t *));

LIBCSS_PUBLIC void css_init_properties(void);

LIBCSS_PUBLIC void css_destroy_properties(void);

LIBCSS_PUBLIC css_propdef_t *css_get_propdef_by_name(const char *name);

LIBCSS_PUBLIC css_propdef_t *css_get_propdef(int key);

LIBCSS_PUBLIC unsigned css_get_prop_count(void);

LIBCSS_END_DECLS

#endif

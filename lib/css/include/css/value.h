/*
 * lib/css/include/css/value.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_VALUE_H
#define LIBCSS_INCLUDE_CSS_VALUE_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC void css_valdef_destroy(css_valdef_t *valdef);

LIBCSS_PUBLIC void css_init_value_definitons(void);

LIBCSS_PUBLIC void css_destroy_value_definitons(void);

LIBCSS_PUBLIC size_t css_valdef_to_string(const css_valdef_t *valdef, char *str,
					  size_t max_len);

LIBCSS_PUBLIC const css_value_type_record_t *css_register_value_type(
    const char *type_name, css_value_parse_cb parse);

LIBCSS_PUBLIC const css_value_type_record_t *css_get_value_type(
    const char *type_name);

LIBCSS_PUBLIC int css_register_valdef_alias(const char *alias,
					    const char *definitons);

LIBCSS_PUBLIC const css_valdef_t *css_resolve_valdef_alias(const char *alias);

LIBCSS_PUBLIC css_valdef_t *css_compile_valdef(const char *definition_str);

/**
 * 根据 CSS 值的定义，解析字符串中与之匹配的值
 * @return 成功返回已解析的字符串长度，否则返回 -1
 */
LIBCSS_PUBLIC int css_parse_value(const css_valdef_t *valdef, const char *str,
				  css_style_value_t *val);

LIBCSS_PUBLIC size_t css_print_valdef(const css_valdef_t *s);

LIBCSS_PUBLIC size_t css_valdef_to_string(const css_valdef_t *s, char *str,
					  size_t max_len);

LIBCSS_END_DECLS

#endif

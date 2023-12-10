/*
 * lib/css/include/css/style_value.h
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_STYLE_VALUE_H
#define LIBCSS_INCLUDE_CSS_STYLE_VALUE_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

#define CSS_COLOR_TRANSPARENT css_color(0, 0xff, 0xff, 0xff)

LIBCSS_INLINE css_color_value_t css_color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	return b | g << 8 | r << 16 | a << 24;
}

LIBCSS_INLINE uint8_t css_color_alpha(css_color_value_t value)
{
	return value >> 24;
}

LIBCSS_PUBLIC unsigned css_array_value_get_length(const css_style_array_value_t val);

LIBCSS_PUBLIC int css_array_value_set_length(css_style_array_value_t *arr,
					unsigned new_len);

LIBCSS_PUBLIC unsigned css_array_value_concat(css_style_array_value_t *arr1,
					 css_style_array_value_t *arr2);

LIBCSS_PUBLIC css_style_array_value_t
css_array_value_duplicate(const css_style_array_value_t arr);

LIBCSS_PUBLIC void css_array_value_destroy(css_style_array_value_t val);

LIBCSS_PUBLIC void css_style_value_destroy(css_style_value_t *val);

LIBCSS_PUBLIC unsigned css_style_value_get_array_length(
    const css_style_value_t *val);

LIBCSS_PUBLIC int css_style_value_set_array_length(css_style_value_t *val,
					      unsigned new_len);

LIBCSS_PUBLIC void css_style_value_copy(css_style_value_t *dst,
				   const css_style_value_t *src);

LIBCSS_PUBLIC size_t css_style_value_concat_array(css_style_value_t *val1,
					     css_style_value_t *val2);

// https://developer.mozilla.org/en-US/docs/Web/API/CSSStyleValue/parse

LIBCSS_PUBLIC css_style_value_t *css_style_value_parse(const char *property,
						  const char *css_text);

LIBCSS_PUBLIC size_t css_style_value_to_string(const css_style_value_t *s, char *str,
					  size_t max_len);

LIBCSS_PUBLIC size_t css_print_style_value(const css_style_value_t *s);

LIBCSS_END_DECLS

#endif

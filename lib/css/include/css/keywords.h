﻿/*
 * lib/css/include/css/keywords.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_KEYWORDS_H
#define LIBCSS_INCLUDE_CSS_KEYWORDS_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_INLINE bool css_check_keyword(css_style_value_t *sv, css_keyword_value_t kv)
{
	return sv->type == CSS_KEYWORD_VALUE && sv->keyword_value == kv;
}

LIBCSS_PUBLIC int css_register_keyword(const char *name);
LIBCSS_PUBLIC int css_get_keyword_key(const char *name);
LIBCSS_PUBLIC const char *css_get_keyword_name(int val);
LIBCSS_PUBLIC void css_init_keywords(void);
LIBCSS_PUBLIC void css_destroy_keywords(void);

LIBCSS_END_DECLS

#endif

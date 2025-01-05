/*
 * lib/css/include/css/utils.h
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBCSS_INCLUDE_CSS_UTILS_H
#define LIBCSS_INCLUDE_CSS_UTILS_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS


LIBCSS_INLINE float css_metrics_actual_scale(css_metrics_t *metrics)
{
        return metrics->dpi / 96.0f * metrics->scale;
}

LIBCSS_PUBLIC bool css_parse_font_weight(const char *str, int *weight);

LIBCSS_PUBLIC bool css_parse_font_style(const char *str, int *style);

LIBCSS_END_DECLS

#endif

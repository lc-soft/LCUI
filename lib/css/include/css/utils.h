#ifndef LIBCSS_INCLUDE_CSS_UTILS_H
#define LIBCSS_INCLUDE_CSS_UTILS_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC libcss_bool_t css_parse_font_weight(const char *str, int *weight);

LIBCSS_PUBLIC libcss_bool_t css_parse_font_style(const char *str, int *style);

LIBCSS_END_DECLS

#endif

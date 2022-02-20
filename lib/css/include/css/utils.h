#ifndef LIBCSS_INCLUDE_CSS_UTILS_H
#define LIBCSS_INCLUDE_CSS_UTILS_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API LCUI_BOOL css_parse_font_weight(const char *str, int *weight);

LCUI_API LCUI_BOOL css_parse_font_style(const char *str, int *style);

LCUI_END_HEADER

#endif

#ifndef CSS_INCLUDE_CSS_UTILS_H
#define CSS_INCLUDE_CSS_UTILS_H

#include <LCUI/header.h>
#include "def.h"

LCUI_BEGIN_HEADER

/** 从字符串中解析出数字，支持的单位：点(pt)、像素(px)、百分比(%) */
LCUI_API LCUI_BOOL css_parse_number(css_unit_value_t *var, const char *str);

LCUI_API LCUI_BOOL css_parse_rgb(css_unit_value_t *var, const char *str);

LCUI_API LCUI_BOOL css_parse_rgba(css_unit_value_t *var, const char *str);

/** 从字符串中解析出色彩值，支持格式：#fff、#ffffff, rgba(R,G,B,A)、rgb(R,G,B) */
LCUI_API LCUI_BOOL css_parse_color(css_unit_value_t *var, const char *str);

/** 解析资源路径 */
LCUI_API LCUI_BOOL css_parse_url(css_unit_value_t *s, const char *str, const char *dirname);

LCUI_API LCUI_BOOL css_parse_font_weight(const char *str, int *weight);

LCUI_API LCUI_BOOL css_parse_font_style(const char *str, int *style);

LCUI_END_HEADER

#endif

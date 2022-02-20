
#ifndef LIBCSS_INCLUDE_CSS_DATA_TYPES_H
#define LIBCSS_INCLUDE_CSS_DATA_TYPES_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API LCUI_BOOL css_parse_numeric_value(css_style_value_t *s,
					   const char *str);
LCUI_API LCUI_BOOL css_parse_unit_value(css_style_value_t *s, const char *str);
LCUI_API LCUI_BOOL css_parse_string_value(css_style_value_t *val,
					  const char *str);
LCUI_API LCUI_BOOL css_parse_url_value(css_style_value_t *s, const char *str);
LCUI_API LCUI_BOOL css_parse_color_value(css_style_value_t *val,
					 const char *str);
LCUI_API void css_init_data_types(void);

LCUI_END_HEADER

#endif

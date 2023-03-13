
#ifndef LIBCSS_INCLUDE_CSS_DATA_TYPES_H
#define LIBCSS_INCLUDE_CSS_DATA_TYPES_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_PUBLIC libcss_bool_t css_parse_numeric_value(css_style_value_t *s,
					   const char *str);
LIBCSS_PUBLIC libcss_bool_t css_parse_unit_value(css_style_value_t *s, const char *str);
LIBCSS_PUBLIC libcss_bool_t css_parse_string_value(css_style_value_t *val,
					  const char *str);
LIBCSS_PUBLIC libcss_bool_t css_parse_url_value(css_style_value_t *s, const char *str);
LIBCSS_PUBLIC libcss_bool_t css_parse_color_value(css_style_value_t *val,
					 const char *str);
LIBCSS_PUBLIC void css_init_data_types(void);

LIBCSS_END_DECLS

#endif

#ifndef LIBCSS_INCLUDE_CSS_VALUE_H
#define LIBCSS_INCLUDE_CSS_VALUE_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

void css_valdef_destroy(css_valdef_t *valdef);

LCUI_API void css_init_value_definitons(void);

LCUI_API void css_destroy_value_definitons(void);

LCUI_API size_t css_valdef_to_string(const css_valdef_t *valdef, char *str,
				     size_t max_len);

LCUI_API const css_value_type_record_t *css_register_value_type(
    const char *type_name, css_value_parse_func_t parse);

LCUI_API const css_value_type_record_t *css_get_value_type(
    const char *type_name);

LCUI_API int css_register_valdef_alias(const char *alias,
				       const char *definitons);

LCUI_API const css_valdef_t *css_resolve_valdef_alias(const char *alias);

LCUI_API css_valdef_t *css_compile_valdef(const char *definition_str);

/**
 * 根据 CSS 值的定义，解析字符串中与之匹配的值
 * @return 成功返回已解析的字符串长度，否则返回 -1
 */
LCUI_API int css_parse_value(const css_valdef_t *valdef, const char *str,
			     css_style_value_t *val);

LCUI_API size_t css_print_valdef(const css_valdef_t *s);

LCUI_API size_t css_valdef_to_string(const css_valdef_t *s, char *str,
				     size_t max_len);

LCUI_END_HEADER

#endif

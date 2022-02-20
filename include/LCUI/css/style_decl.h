#ifndef LIBCSS_INCLUDE_CSS_STYLE_DECL_H
#define LIBCSS_INCLUDE_CSS_STYLE_DECL_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API css_style_decl_t *css_style_decl_create(void);

LCUI_API void css_style_decl_destroy(css_style_decl_t *list);

LCUI_API css_prop_t *css_style_decl_alloc(css_style_decl_t *list, int key);

LCUI_API void css_style_decl_add(css_style_decl_t *list, int key,
				 const css_style_value_t *value);

LCUI_API void css_style_decl_set(css_style_decl_t *list, int key,
				 const css_style_value_t *value);

LCUI_API int css_style_decl_remove(css_style_decl_t *list, int key);

LCUI_API void css_style_decl_merge(css_style_decl_t *dst,
				   const css_style_decl_t *src);

LCUI_API css_prop_t *css_style_decl_find(css_style_decl_t *list, int key);

LCUI_API size_t css_print_style_decl(const css_style_decl_t *s);

LCUI_API size_t css_style_decl_to_string(const css_style_decl_t *list,
					 char *str, size_t max_len);
LCUI_END_HEADER

#endif

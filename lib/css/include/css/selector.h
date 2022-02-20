
#ifndef LIBCSS_INCLUDE_CSS_SELECTOR_H
#define LIBCSS_INCLUDE_CSS_SELECTOR_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API css_selector_t *css_selector_create(const char *selector);

LCUI_API css_selector_t *css_selector_duplicate(css_selector_t *selector);

LCUI_API int css_selector_append(css_selector_t *selector,
				 css_selector_node_t *node);

LCUI_API void css_selector_update(css_selector_t *s);

LCUI_API void css_selector_destroy(css_selector_t *s);

LCUI_API int css_selector_node_get_name_list(css_selector_node_t *sn,
					     list_t *names);

LCUI_API int css_selector_node_update(css_selector_node_t *node);

LCUI_API css_selector_node_t * css_selector_node_duplicate(const css_selector_node_t *src);

LCUI_API void css_selector_node_destroy(css_selector_node_t *node);

/**
 * 匹配选择器节点
 * 左边的选择器必须包含右边的选择器的所有属性。
 */
LCUI_API LCUI_BOOL css_selector_node_match(css_selector_node_t *sn1,
					   css_selector_node_t *sn2);

LCUI_END_HEADER

#endif

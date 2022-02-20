
#ifndef LIBCSS_INCLUDE_CSS_KEYWORDS_H
#define LIBCSS_INCLUDE_CSS_KEYWORDS_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

INLINE LCUI_BOOL css_check_keyword(css_style_value_t *sv, css_keyword_value_t kv)
{
	return sv->type == CSS_KEYWORD_VALUE && sv->keyword_value == kv;
}

LCUI_API int css_register_keyword(const char *name);
LCUI_API int css_get_keyword_key(const char *name);
LCUI_API const char *css_get_keyword_name(int val);
LCUI_API void css_init_keywords(void);
LCUI_API void css_destroy_keywords(void);

LCUI_END_HEADER

#endif

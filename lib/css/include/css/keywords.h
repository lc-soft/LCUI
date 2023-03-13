
#ifndef LIBCSS_INCLUDE_CSS_KEYWORDS_H
#define LIBCSS_INCLUDE_CSS_KEYWORDS_H

#include "common.h"
#include "types.h"

LIBCSS_BEGIN_DECLS

LIBCSS_INLINE libcss_bool_t css_check_keyword(css_style_value_t *sv, css_keyword_value_t kv)
{
	return sv->type == CSS_KEYWORD_VALUE && sv->keyword_value == kv;
}

LIBCSS_PUBLIC int css_register_keyword(const char *name);
LIBCSS_PUBLIC int css_get_keyword_key(const char *name);
LIBCSS_PUBLIC const char *css_get_keyword_name(int val);
LIBCSS_PUBLIC void css_init_keywords(void);
LIBCSS_PUBLIC void css_destroy_keywords(void);

LIBCSS_END_DECLS

#endif

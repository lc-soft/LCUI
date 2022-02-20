
#ifndef LIBCSS_INCLUDE_CSS_PROPERTIES_H
#define LIBCSS_INCLUDE_CSS_PROPERTIES_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API int css_register_property(const char *name, const char *syntax,
				   const char *initial_value,
				   int (*cascade)(const css_style_array_value_t,
						  css_computed_style_t *));

LCUI_API int css_register_shorthand_property(
    const char *name, const char *syntax,
    int (*parse)(css_propdef_t *, const char *, css_style_decl_t *));

LCUI_API void css_init_properties(void);

LCUI_API void css_destroy_properties(void);

LCUI_API css_propdef_t *css_get_propdef_by_name(const char *name);

LCUI_API css_propdef_t *css_get_propdef(int key);

LCUI_API unsigned css_get_prop_count(void);

LCUI_END_HEADER

#endif

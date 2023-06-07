#ifndef LIB_UI_INCLUDE_UI_CSS_H
#define LIB_UI_INCLUDE_UI_CSS_H

#include "common.h"

LIBUI_END_DECLS

LIBUI_PUBLIC int ui_load_css_file(const char *filepath);
LIBUI_PUBLIC size_t ui_load_css_string(const char *str, const char *space);

LIBUI_END_DECLS
#endif


#ifndef LCUI_INCLUDE_LCUI_FONTS_H
#define LCUI_INCLUDE_LCUI_FONTS_H

#include "common.h"

LCUI_BEGIN_HEADER

LCUI_API bool lcui_fonts_set_default(const char *family_name);

#define lcui_set_default_font lcui_fonts_set_default

#endif

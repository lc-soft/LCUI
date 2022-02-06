#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_PIXEL_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_PIXEL_H

#include <LCUI/header.h>
#include <LCUI/types.h>
#include "def.h"

LCUI_API unsigned pd_get_pixel_size(pd_color_type_t color_type);

LCUI_API unsigned pd_get_pixel_row_size(pd_color_type_t color_type, size_t len);

LCUI_API int pd_format_pixels(const uchar_t *in_pixels,
				   pd_color_type_t in_color_type,
				   uchar_t *out_pixels,
				   pd_color_type_t out_color_type,
				   size_t count);

#endif

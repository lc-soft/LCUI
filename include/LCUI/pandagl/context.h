#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H

#include <LCUI/def.h>
#include "def.h"

LCUI_BEGIN_HEADER

LCUI_API pd_context_t* pd_context_create(pd_canvas_t *canvas, pd_rect_t *rect);

LCUI_API void pd_context_destroy(pd_context_t* paint);

LCUI_END_HEADER

#endif

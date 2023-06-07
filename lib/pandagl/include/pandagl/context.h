#ifndef LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H
#define LIB_PANDAGL_INCLUDE_PANDAGL_CONTEXT_H

#include "common.h"
#include "types.h"

PD_BEGIN_DECLS

PD_PUBLIC pd_context_t* pd_context_create(pd_canvas_t *canvas, pd_rect_t *rect);

PD_PUBLIC void pd_context_destroy(pd_context_t* paint);

PD_END_DECLS

#endif

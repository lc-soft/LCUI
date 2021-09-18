// Copyright to be defined

#ifndef PANDAGL_INCLUDE_PANDAGL_DRAW_BORDER_H_
#define PANDAGL_INCLUDE_PANDAGL_DRAW_BORDER_H_

#include <PandaGL/types.h>
#include <PandaGL/build.h>

PD_API int border_crop_content(const pd_border_t *border, const pd_rect_t *box,
                pd_context_t* context_ptr);

PD_API int pd_border_paint(const pd_border_t *border,
              const pd_rect_t *box,
              pd_context_t* context_ptr);


#endif  // !PANDAGL_INCLUDE_PANDAGL_DRAW_BORDER_H_


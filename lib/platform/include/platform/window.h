#ifndef LIBPLAT_INCLUDE_PLATFORM_WINDOW_H
#define LIBPLAT_INCLUDE_PLATFORM_WINDOW_H

#include "types.h"
#include "common.h"

LIBPLAT_BEGIN_DECLS

LIBPLAT_PUBLIC int app_get_screen_width(void);
LIBPLAT_PUBLIC int app_get_screen_height(void);
LIBPLAT_PUBLIC void *app_window_get_handle(app_window_t *wnd);

LIBPLAT_PUBLIC app_window_t *app_get_window_by_handle(void *handle);
LIBPLAT_PUBLIC app_window_t *app_window_create(const wchar_t *title, int x, int y,
					 int width, int height,
					 app_window_t *parent);

LIBPLAT_PUBLIC void app_window_close(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_destroy(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_set_position(app_window_t *wnd, int x, int y);
LIBPLAT_PUBLIC void app_window_set_framebuffer_size(app_window_t *wnd, int width,
					      int height);
LIBPLAT_PUBLIC void app_window_set_size(app_window_t *wnd, int width, int height);
LIBPLAT_PUBLIC void app_window_show(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_hide(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_activate(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_set_title(app_window_t *wnd, const wchar_t *title);
LIBPLAT_PUBLIC int app_window_get_width(app_window_t *wnd);
LIBPLAT_PUBLIC int app_window_get_height(app_window_t *wnd);
LIBPLAT_PUBLIC void app_window_set_min_width(app_window_t *wnd, int min_width);
LIBPLAT_PUBLIC void app_window_set_min_height(app_window_t *wnd, int min_height);
LIBPLAT_PUBLIC void app_window_set_max_width(app_window_t *wnd, int max_width);
LIBPLAT_PUBLIC void app_window_set_max_height(app_window_t *wnd, int max_height);
LIBPLAT_PUBLIC app_window_paint_t *app_window_begin_paint(app_window_t *wnd,
						    pd_rect_t *rect);
LIBPLAT_PUBLIC void app_window_end_paint(app_window_t *wnd,
				   app_window_paint_t *paint);
LIBPLAT_PUBLIC void app_window_present(app_window_t *wnd);

LIBPLAT_END_DECLS

#endif

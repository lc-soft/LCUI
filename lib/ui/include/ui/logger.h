#ifndef LIB_UI_INCLUDE_UI_LOGGER_H
#define LIB_UI_INCLUDE_UI_LOGGER_H

#include "common.h"
#include "types.h"

LIBUI_BEGIN_DECLS

LIBUI_PUBLIC int ui_logger_log(logger_level_e level, ui_widget_t *w,
			       const char *fmt, ...);

#define ui_debug(W, ...) ui_logger_log(LOGGER_LEVEL_DEBUG, W, ##__VA_ARGS__)

LIBUI_END_DECLS

#endif

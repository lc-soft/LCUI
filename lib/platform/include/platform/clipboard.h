#ifndef LIBPLAT_INCLUDE_PLATFORM_CLIPBOARD_H
#define LIBPLAT_INCLUDE_PLATFORM_CLIPBOARD_H

#include "types.h"
#include "common.h"

LIBPLAT_BEGIN_DECLS

/**
 * Clipboard
 * References: https://docs.gtk.org/gtk3/class.Clipboard.html
 */

typedef struct clipboard_t {
	pd_canvas_t *image;
	wchar_t *text;
	size_t len;
} clipboard_t;

typedef void (*clipboard_callback_t)(clipboard_t *, void *);

LIBPLAT_PUBLIC int clipboard_request_text(clipboard_callback_t action, void *arg);
LIBPLAT_PUBLIC int clipboard_set_text(const wchar_t *text, size_t len);
LIBPLAT_PUBLIC void clipboard_init(void);
LIBPLAT_PUBLIC void clipboard_destroy(void);

LIBPLAT_END_DECLS

#endif
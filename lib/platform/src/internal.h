#include "config.h"
#include <LCUI/util.h>
#include "../include/platform.h"

#ifdef LCUI_PLATFORM_WIN_DESKTOP

int ime_add_win32(void);

#endif

#ifdef LCUI_PLATFORM_LINUX

#ifdef USE_LIBX11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>

Display *x11_app_get_display(void);
Window x11_app_get_main_window(void);

void x11_app_driver_init(app_driver_t *driver);
void x11_app_window_driver_init(app_window_driver_t *driver);

int x11_clipboard_request_text(clipboard_callback_t callback, void *arg);
int x11_clipboard_set_text(const wchar_t *text, size_t len);
void x11_clipboard_init(void);
void x11_clipboard_destroy(void);

#endif

void fb_app_driver_init(app_driver_t *driver);
void fb_app_window_driver_init(app_window_driver_t *driver);

int linux_mouse_init(void);
int linux_mouse_destroy(void);
int linux_keyboard_init(void);
int linux_keyboard_destroy(void);
int ime_add_linux(void);

#endif

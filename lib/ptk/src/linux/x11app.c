/*
 * lib/ptk/src/linux/x11app.c
 *
 * Copyright (c) 2023-2025, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include "ptk.h"
#include "x11app.h"

#if defined(PTK_LINUX) && defined(PTK_HAS_LIBX11)

#define MOUSE_WHEEL_DELTA 20
#define MIN_WINDOW_WIDTH 100
#define MIN_WINDOW_HEIGHT 40

#include <pandagl.h>

struct ptk_window {
	Window handle;
	int x, y;
	int width, height;
	int min_width, min_height;
	int max_width, max_height;

	GC gc;
	XImage *ximage;
	pd_canvas_t fb;

	/** list_t<pd_rect_t> */
	list_t rects;
	list_node_t node;
};

static struct ptk_x11app {
	int screen;
	Display *display;
	Window win_root;
	Window win_events;
	Atom wm_delete;
	Atom wm_exit;
	Colormap cmap;

	int exit_code;

	/** list_t<ptk_native_event_listener_t> */
	list_t native_listeners;

	/** list_t<ptk_window_t> */
	list_t windows;
} x11_app;

static void ptk_x11window__show(ptk_window_t *wnd);
static void ptk_x11window__set_size(ptk_window_t *wnd, int width, int height);

static int convert_keycode(KeySym keysym)
{
	switch (keysym) {
	case XK_Tab:
		return KEY_TAB;
	case XK_Escape:
		return KEY_ESCAPE;
	case XK_Return:
		return KEY_ENTER;
	case XK_Delete:
		return KEY_DELETE;
	case XK_BackSpace:
		return KEY_BACKSPACE;
	case XK_Home:
		return KEY_HOME;
	case XK_Left:
		return KEY_LEFT;
	case XK_Up:
		return KEY_UP;
	case XK_Right:
		return KEY_RIGHT;
	case XK_Down:
		return KEY_DOWN;
	case XK_Page_Up:
		return KEY_PAGEUP;
	case XK_Page_Down:
		return KEY_PAGEDOWN;
	case XK_End:
		return KEY_END;
	case XK_Control_R:
	case XK_Control_L:
		return KEY_CONTROL;
	case XK_Shift_R:
	case XK_Shift_L:
		return KEY_SHIFT;
	case XK_Alt_L:
	case XK_Alt_R:
		return KEY_ALT;
	case XK_Caps_Lock:
		return KEY_CAPITAL;
	case XK_comma:
		return KEY_COMMA;
	case XK_period:
		return KEY_PERIOD;
	case XK_minus:
		return KEY_MINUS;
	case XK_slash:
		return KEY_SLASH;
	case XK_semicolon:
		return KEY_SEMICOLON;
	case XK_equal:
		return KEY_EQUAL;
	case XK_bracketleft:
		return KEY_BRACKETLEFT;
	case XK_bracketright:
		return KEY_BRACKETRIGHT;
	case XK_backslash:
		return KEY_BACKSLASH;
	case XK_apostrophe:
		return KEY_APOSTROPHE;
	case XK_grave:
		return KEY_GRAVE;
	default:
		break;
	}
	return keysym;
}

static ptk_window_t *ptk_x11window__create(const wchar_t *title, int x, int y,
					   int width, int height,
					   ptk_window_t *parent)
{
	ptk_window_t *wnd;
	unsigned long bdcolor = BlackPixel(x11_app.display, x11_app.screen);
	unsigned long bgcolor = WhitePixel(x11_app.display, x11_app.screen);

	wnd = calloc(sizeof(ptk_window_t), 1);
	wnd->gc = NULL;
	wnd->ximage = NULL;
	wnd->node.data = wnd;
	wnd->width = width;
	wnd->height = height;
	wnd->x = x;
	wnd->y = y;
	pd_canvas_init(&wnd->fb);
	list_create(&wnd->rects);
	wnd->fb.color_type = PD_COLOR_TYPE_ARGB;
	wnd->handle = XCreateSimpleWindow(x11_app.display, x11_app.win_root, x,
					  y, y_max(width, MIN_WINDOW_WIDTH),
					  y_max(height, MIN_WINDOW_HEIGHT), 1,
					  bdcolor, bgcolor);
	list_append_node(&x11_app.windows, &wnd->node);
	return wnd;
}

static void ptk_x11window__on_destroy(ptk_window_t *wnd)
{
	list_unlink(&x11_app.windows, &wnd->node);
	list_destroy(&wnd->rects, free);
	if (wnd->ximage) {
		XDestroyImage(wnd->ximage);
		wnd->ximage = NULL;
	}
	if (wnd->gc) {
		XFreeGC(x11_app.display, wnd->gc);
		wnd->gc = NULL;
	}
	free(wnd);
}

static ptk_window_t *ptk_x11app_get_window_by_handle(void *handle)
{
	list_node_t *node;

	for (list_each(node, &x11_app.windows)) {
		if (((ptk_window_t *)node->data)->handle == *(Window *)handle) {
			return node->data;
		}
	}
	return NULL;
}

static void ptk_x11window__on_size(ptk_window_t *wnd, int width, int height)
{
	int depth;
	XGCValues gcv;
	Visual *visual;

	width = y_max(MIN_WINDOW_WIDTH, width);
	height = y_max(MIN_WINDOW_HEIGHT, height);
	if (width == wnd->width && height == wnd->height && wnd->ximage &&
	    wnd->gc) {
		return;
	}
	if (wnd->ximage) {
		XDestroyImage(wnd->ximage);
		wnd->ximage = NULL;
	}
	if (wnd->gc) {
		XFreeGC(x11_app.display, wnd->gc);
		wnd->gc = NULL;
	}
	pd_canvas_init(&wnd->fb);
	wnd->width = width;
	wnd->height = height;
	depth = DefaultDepth(x11_app.display, x11_app.screen);
	switch (depth) {
	case 32:
	case 24:
		wnd->fb.color_type = PD_COLOR_TYPE_ARGB;
		break;
	default:
		logger_error("[x11_app] unsupport depth: %d.\n", depth);
		break;
	}
	pd_canvas_create(&wnd->fb, width, height);
	visual = DefaultVisual(x11_app.display, x11_app.screen);
	wnd->ximage =
	    XCreateImage(x11_app.display, visual, depth, ZPixmap, 0,
			 (char *)(wnd->fb.bytes), width, height, 32, 0);
	if (!wnd->ximage) {
		pd_canvas_destroy(&wnd->fb);
		logger_error("[x11_app] create XImage faild.\n");
		return;
	}
	gcv.graphics_exposures = False;
	wnd->gc =
	    XCreateGC(x11_app.display, wnd->handle, GCGraphicsExposures, &gcv);
	if (!wnd->gc) {
		logger_error("[x11_app] create graphics context faild.\n");
	}
}

static int x11_ptk_process_native_event(void)
{
	KeySym keysym;
	Display *dsp = x11_app.display;
	XEvent xe = { 0 };

	list_node_t *node;
	ptk_native_event_listener_t *listener;
	ptk_event_t e = { 0 };

	XNextEvent(x11_app.display, &xe);
	e.window = ptk_x11app_get_window_by_handle(&xe.xany.window);
	switch (xe.type) {
	case ConfigureNotify:
		e.type = PTK_EVENT_SIZE;
		e.size.width = xe.xconfigure.width;
		e.size.height = xe.xconfigure.height;
		ptk_x11window__on_size(e.window, xe.xconfigure.width,
				       xe.xconfigure.height);
		ptk_process_event(&e);
		// TODO: add MOVE event?
		break;
	case Expose:
		e.type = PTK_EVENT_PAINT;
		e.paint.rect.x = xe.xexpose.x;
		e.paint.rect.y = xe.xexpose.y;
		e.paint.rect.width = xe.xexpose.width;
		e.paint.rect.height = xe.xexpose.height;
		ptk_process_event(&e);
		break;
	case KeyPress:
	case KeyRelease: {
		int min_keycode, max_keycode;

		e.type =
		    xe.type == KeyPress ? PTK_EVENT_KEYDOWN : PTK_EVENT_KEYUP;
		XAutoRepeatOn(dsp);
		keysym = XkbKeycodeToKeysym(dsp, xe.xkey.keycode, 0, 1);
		e.key.code = convert_keycode(keysym);
		e.key.shift_key = xe.xkey.state & ShiftMask ? true : false;
		e.key.ctrl_key = xe.xkey.state & ControlMask ? true : false;
		// @WhoAteDaCake
		// TODO: this triggers and line 155 causes event to trigger
		// twice? I tried to remove the one in if statement and put it
		// to the end of the function but it doesn't seem to work
		// properly then? Needs investigation
		ptk_process_event(&e);
		XDisplayKeycodes(dsp, &min_keycode, &max_keycode);
		if (keysym >= min_keycode && keysym <= max_keycode &&
		    e.type == PTK_EVENT_KEYDOWN && !e.key.ctrl_key) {
			e.type = PTK_EVENT_KEYPRESS;
			e.key.code = XkbKeycodeToKeysym(
			    dsp, xe.xkey.keycode, 0,
			    xe.xkey.state & ShiftMask ? 1 : 0);
			ptk_process_event(&e);
		}
		break;
	}
	case MotionNotify:
		e.type = PTK_EVENT_MOUSEMOVE;
		e.mouse.x = xe.xmotion.x;
		e.mouse.y = xe.xmotion.y;
		ptk_process_event(&e);
		break;
	case ButtonPress:
		if (xe.xbutton.button == Button4) {
			e.type = PTK_EVENT_WHEEL;
			e.wheel.delta_y = MOUSE_WHEEL_DELTA;
		} else if (xe.xbutton.button == Button5) {
			e.type = PTK_EVENT_WHEEL;
			e.wheel.delta_y = -MOUSE_WHEEL_DELTA;
		} else {
			e.type = PTK_EVENT_MOUSEDOWN;
			e.mouse.x = xe.xbutton.x;
			e.mouse.y = xe.xbutton.y;
			e.mouse.button = xe.xbutton.button;
		}
		ptk_process_event(&e);
		break;
	case ButtonRelease:
		e.type = PTK_EVENT_MOUSEUP;
		e.mouse.x = xe.xbutton.x;
		e.mouse.y = xe.xbutton.y;
		e.mouse.button = xe.xbutton.button;
		ptk_process_event(&e);
		break;
	/**
	 * FIXME: 调用 XDestroyWindow() 后未接收到 DestroyNotify 事件
	 */
	// case DestroyNotify:
	case ClientMessage:
		if (xe.xclient.data.l[0] == x11_app.wm_delete) {
			logger_debug("WM_DELETE_WINNDOW\n");
			e.type = PTK_EVENT_CLOSE;
			ptk_process_event(&e);
			ptk_x11window__on_destroy(e.window);
		} else if (xe.xclient.data.l[0] == x11_app.wm_exit) {
			logger_debug("WM_EXIT\n");
			e.type = PTK_EVENT_QUIT;
			ptk_process_event(&e);
			return 0;
		}
		break;
	default:
		break;
	}
	ptk_event_destroy(&e);
	for (list_each(node, &x11_app.native_listeners)) {
		listener = node->data;
		if (listener->type == xe.type) {
			listener->handler(&xe, listener->data);
		}
	}
	return 1;
}

static int ptk_x11app_pending(void)
{
	XFlush(x11_app.display);
	return XEventsQueued(x11_app.display, QueuedAlready);
}

static int x11_ptk_process_native_events(ptk_process_events_option_t option)
{
	int ret = 0;

	x11_app.exit_code = 0;
	if (option == PTK_PROCESS_EVENTS_ONE_IF_PRESENT ||
	    option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT) {
		do {
			ptk_tick();
			if (!ptk_x11app_pending()) {
				break;
			}
			x11_ptk_process_native_event();
			ptk_process_events();
		} while (option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
		return x11_app.exit_code;
	}
	do {
		for (ptk_tick(); !ptk_x11app_pending(); ptk_tick()) {
			ptk_process_events();
			sleep_ms(1);
		}
		ret = x11_ptk_process_native_event();
		ptk_process_events();
	} while (option == PTK_PROCESS_EVENTS_UNTIL_QUIT && ret != 0);
	return x11_app.exit_code;
}

static int x11_ptk_add_native_event_listener(int type,
					     ptk_native_event_handler_t handler,
					     void *data)
{
	ptk_native_event_listener_t *listener;

	listener = malloc(sizeof(ptk_native_event_listener_t));
	if (!listener) {
		return -1;
	}
	listener->handler = handler;
	listener->data = data;
	listener->type = type;
	list_append(&x11_app.native_listeners, listener);
	return 0;
}

static int x11_ptk_remove_native_event_listener(
    int type, ptk_native_event_handler_t handler)
{
	list_node_t *node, *prev;
	ptk_native_event_listener_t *listener;

	for (list_each(node, &x11_app.native_listeners)) {
		prev = node->prev;
		listener = node->data;
		if (listener->handler == handler && listener->type == type) {
			list_delete_node(&x11_app.native_listeners, node);
			free(listener);
			node = prev;
			return 0;
		}
	}
	return -1;
}

static int x11_ptk_screen_get_width(void)
{
	Screen *s = DefaultScreenOfDisplay(x11_app.display);
	return XWidthOfScreen(s);
}

static int x11_ptk_screen_get_height(void)
{
	Screen *s = DefaultScreenOfDisplay(x11_app.display);
	return XHeightOfScreen(s);
}

static void ptk_x11window__activate(ptk_window_t *wnd)
{
	XSetWMProtocols(x11_app.display, wnd->handle, &x11_app.wm_delete, 1);
	XSelectInput(x11_app.display, wnd->handle,
		     ExposureMask | KeyPressMask | ButtonPress |
			 StructureNotifyMask | ButtonReleaseMask |
			 KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
			 PointerMotionMask | Button1MotionMask |
			 VisibilityChangeMask);
	XFlush(x11_app.display);
	ptk_x11window__show(wnd);
}

static void ptk_x11window__close(ptk_window_t *wnd)
{
	XEvent xe = { 0 };

	xe.xclient.type = ClientMessage;
	xe.xclient.data.l[0] = x11_app.wm_delete;
	xe.xclient.serial = 0;
	xe.xclient.send_event = 1;
	xe.xclient.format = 32;
	xe.xclient.window = wnd->handle;
	XSendEvent(x11_app.display, x11_app.win_events, 0, NoEventMask,
		       &xe);
}

static void ptk_x11window__destroy(ptk_window_t *wnd)
{
	XDestroyWindow(x11_app.display, wnd->handle);
}

static void ptk_x11window__set_title(ptk_window_t *wnd, const wchar_t *title)
{
	size_t len;
	char *utf8_title;
	XTextProperty name;

	len = encode_utf8(NULL, title, 0) + 1;
	utf8_title = malloc(sizeof(char) * len);
	if (!title) {
		return;
	}
	len = encode_utf8(utf8_title, title, len);
	name.value = (unsigned char *)utf8_title;
	name.encoding = XA_STRING;
	name.format = 8 * sizeof(char);
	name.nitems = len;
	XSetWMName(x11_app.display, wnd->handle, &name);
	free(utf8_title);
}

static void ptk_x11window__set_size(ptk_window_t *wnd, int width, int height)
{
	// Ignore useless events and avoid infinite window size changes
	if (wnd->width == width && wnd->height == height) {
		return;
	}
	XResizeWindow(x11_app.display, wnd->handle, width, height);
	// When XResizeWindow() is called, the x and y values in the
	// ConfigureNotify event sent by XResizeWindow() are old because the
	// position set by the previous call to XMoveWindow() has not been
	// updated. Therefore, we need to set the latest position again.
	XMoveWindow(x11_app.display, wnd->handle, wnd->x, wnd->y);
}

static void ptk_x11window__set_position(ptk_window_t *wnd, int x, int y)
{
	if (wnd->x == x && wnd->y == y) {
		return;
	}
	wnd->x = x;
	wnd->y = y;
	XMoveWindow(x11_app.display, wnd->handle, x, y);
}

static void *ptk_x11window__get_handle(ptk_window_t *wnd)
{
	return &wnd->handle;
}

static int ptk_x11window__get_width(ptk_window_t *wnd)
{
	return wnd->width;
}

static int ptk_x11window__get_height(ptk_window_t *wnd)
{
	return wnd->height;
}

static void ptk_x11window__set_min_width(ptk_window_t *wnd, int min_width)
{
	wnd->min_width = min_width;
}

static void ptk_x11window__set_min_height(ptk_window_t *wnd, int min_height)
{
	wnd->min_height = min_height;
}

static void ptk_x11window__set_max_width(ptk_window_t *wnd, int max_width)
{
	wnd->max_width = max_width;
}

static void ptk_x11window__set_max_height(ptk_window_t *wnd, int max_height)
{
	wnd->max_height = max_height;
}

static void ptk_x11window__show(ptk_window_t *wnd)
{
	XMapWindow(x11_app.display, wnd->handle);
}

static void ptk_x11window__hide(ptk_window_t *wnd)
{
	XUnmapWindow(x11_app.display, wnd->handle);
}

static ptk_window_paint_t *ptk_x11window__begin_paint(ptk_window_t *wnd,
						      pd_rect_t *rect)
{
	pd_context_t *paint;

	paint = malloc(sizeof(pd_context_t));
	paint->rect = *rect;
	paint->with_alpha = false;
	pd_canvas_init(&paint->canvas);
	pd_rect_correct(&paint->rect, wnd->width, wnd->height);
	pd_canvas_quote(&paint->canvas, &wnd->fb, &paint->rect);
	pd_canvas_fill(&paint->canvas, pd_rgb(255, 255, 255));
	return paint;
}

static void ptk_x11window__end_paint(ptk_window_t *wnd,
				     ptk_window_paint_t *paint)
{
	pd_rect_t *rect;

	rect = malloc(sizeof(pd_rect_t));
	*rect = paint->rect;
	list_append(&wnd->rects, rect);
	pd_canvas_init(&paint->canvas);
	free(paint);
}

static void ptk_x11window__present(ptk_window_t *wnd)
{
	list_node_t *node;
	list_t rects;
	pd_rect_t *rect;

	if (!wnd->gc || !wnd->ximage) {
		return;
	}
	list_create(&rects);
	list_concat(&rects, &wnd->rects);
	for (list_each(node, &rects)) {
		rect = node->data;
		XPutImage(x11_app.display, wnd->handle, wnd->gc, wnd->ximage,
			  rect->x, rect->y, rect->x, rect->y, rect->width,
			  rect->height);
	}
	list_destroy(&rects, free);
}

static void ptk_x11app_present(void)
{
	list_node_t *node;

	for (list_each(node, &x11_app.windows)) {
		ptk_x11window__present(node->data);
	}
}

static int ptk_x11app_init(const wchar_t *name)
{
	x11_app.display = XOpenDisplay(NULL);
	if (!x11_app.display) {
		return -1;
	}
	x11_app.screen = DefaultScreen(x11_app.display);
	x11_app.win_root = RootWindow(x11_app.display, x11_app.screen);
	x11_app.win_events = XCreateSimpleWindow(
	    x11_app.display, x11_app.win_root, 0, 0, 10, 10, 0, 0, 0);
	x11_app.cmap = DefaultColormap(x11_app.display, x11_app.screen);
	x11_app.wm_delete =
	    XInternAtom(x11_app.display, "WM_DELETE_WINDOW", false);
	x11_app.wm_exit = XInternAtom(x11_app.display, "WM_EXIT", false);
	XSelectInput(x11_app.display, x11_app.win_events, StructureNotifyMask);
	return 0;
}

static int ptk_x11app_destroy(void)
{
	list_destroy_without_node(
	    &x11_app.windows,
	    (list_item_destructor_t)ptk_x11window__on_destroy);
	XCloseDisplay(x11_app.display);
	return 0;
}

static void ptk_x11app_exit(int exit_code)
{
	XEvent xe = { 0 };

	xe.xclient.type = ClientMessage;
	xe.xclient.data.l[0] = x11_app.wm_exit;
	xe.xclient.serial = 0;
	xe.xclient.send_event = 1;
	xe.xclient.format = 32;
	xe.xclient.window = x11_app.win_events;
	XSendEvent(x11_app.display, x11_app.win_events, 0, NoEventMask, &xe);
}

Display *ptk_x11_get_display(void)
{
	return x11_app.display;
}

Window ptk_x11_get_main_window(void)
{
	return x11_app.win_events;
}

void ptk_x11app_driver_init(ptk_app_driver_t *driver)
{
	driver->init = ptk_x11app_init;
	driver->destroy = ptk_x11app_destroy;
	driver->process_events = x11_ptk_process_native_events;
	driver->on_event = x11_ptk_add_native_event_listener;
	driver->off_event = x11_ptk_remove_native_event_listener;
	driver->create_window = ptk_x11window__create;
	driver->get_window = ptk_x11app_get_window_by_handle;
	driver->get_screen_width = x11_ptk_screen_get_width;
	driver->get_screen_height = x11_ptk_screen_get_height;
	driver->present = ptk_x11app_present;
	driver->exit = ptk_x11app_exit;
}

void ptk_x11window_driver_init(ptk_window_driver_t *driver)
{
	driver->show = ptk_x11window__show;
	driver->hide = ptk_x11window__hide;
	driver->close = ptk_x11window__close;
	driver->destroy = ptk_x11window__destroy;
	driver->activate = ptk_x11window__activate;
	driver->set_title = ptk_x11window__set_title;
	driver->set_position = ptk_x11window__set_position;
	driver->set_size = ptk_x11window__set_size;
	driver->get_width = ptk_x11window__get_width;
	driver->get_height = ptk_x11window__get_height;
	driver->get_handle = ptk_x11window__get_handle;
	driver->set_max_width = ptk_x11window__set_max_width;
	driver->set_max_height = ptk_x11window__set_max_height;
	driver->set_min_width = ptk_x11window__set_min_width;
	driver->set_min_height = ptk_x11window__set_min_height;
	driver->begin_paint = ptk_x11window__begin_paint;
	driver->end_paint = ptk_x11window__end_paint;
	driver->present = ptk_x11window__present;
}

#endif

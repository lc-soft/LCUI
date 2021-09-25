#include "../private.h"

#if defined(APP_PLATFORM_LINUX) && defined(USE_LIBX11)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

struct app_window_t {
	Window handle;
	int width, height;
	int min_width, min_height;
	int max_width, max_height;

	GC gc;
	XImage *ximage;

	/** LinkedList<LCUI_Rect> */
	LinkedList rects;
	LinkedListNode node;
};

static struct x11_app_t {
	int screen;
	Display *display;
	Window win_root;
	Window win_main;
	Atom wm_delete;
	Colormap cmap;

	/** LinkedList<app_native_event_listener_t> */
	LinkedList native_listeners;

	/** LinkedList<app_window_t> */
	LinkedList windows;
} x11_app;

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

static LCUI_BOOL x11_app_process_event(XEvent *xe)
{
	KeySym keysym;
	XDisplay *dsp = x11_app.display;

	app_event_t e = { 0 };
	app_window_t *wnd;

	switch (xe.type) {
	case ConfigureNotify:
		e.type = APP_EVENT_SIZE;
		e.window = x11_app_get_window_by_handle(xe.xconfigure.window);
		x11_app_window_resize(e.window, xe.xconfigure.width,
				      xe.xconfigure.height);
		break;
	case Expose:
		e.window = x11_app_get_window_by_handle(xe.xexpose.window);
		e.type = APP_EVENT_PAINT;
		e.paint.rect.x = xe.xexpose.x;
		e.paint.rect.y = xe.xexpose.y;
		e.paint.rect.width = xe.xexpose.width;
		e.paint.rect.height = xe.xexpose.height;
		app_process_event(&e);
		break;
	case KeyPress:
	case KeyRelease:
		e.type =
		    xe.type == KeyPress ? APP_EVENT_KEYDOWN : APP_EVENT_KEYUP;
		XAutoRepeatOn(dsp);
		keysym = XkbKeycodeToKeysym(dsp, xe.xkey.keycode, 0, 1);
		e.key.code = convert_keycode(keysym);
		e.key.shift_key = x_xe.xkey.state & ShiftMask ? TRUE : FALSE;
		e.key.ctrl_key = x_xe.xkey.state & ControlMask ? TRUE : FALSE;
		app_process_event(&e);
		if (keysym >= XK_space && keysym <= XK_asciitilde &&
		    e.type == APP_EVENT_KEYDOWN) {
			e.type = APP_EVENT_KEYPRESS;
			e.key.code = XkbKeycodeToKeysym(
			    dsp, xe.xkey.keycode, 0,
			    xe.xkey.state & ShiftMask ? 1 : 0)
			    app_process_event(&e);
		}
		break;
	case MotionNotify:
		e.type = APP_EVENT_MOUSEMOVE;
		e.mouse.x = xe.xmotion.x;
		e.mouse.y = xe.xmotion.y;
		app_process_event(&e);
		break;
	case ButtonPress:
		if (xe.xbutton.button == Button4) {
			e.type = APP_EVENT_WHEEL;
			e.wheel.delta_y = MOUSE_WHEEL_DELTA;
		} else if (xe.xbutton.button == Button5) {
			e.type = APP_EVENT_WHEEL;
			e.wheel.delta_y = -MOUSE_WHEEL_DELTA;
		} else {
			e.type = APP_EVENT_MOUSEDOWN;
			e.mouse.x = xe.xbutton.x;
			e.mouse.y = xe.xbutton.y;
			e.mouse.button = xe.xbutton.button;
		}
		app_process_event(&e);
		break;
	case ButtonRelease:
		e.type = APP_EVENT_MOUSEUP;
		e.mouse.x = xe.xbutton.x;
		e.mouse.y = xe.xbutton.y;
		e.mouse.button = xe.xbutton.button;
		app_process_event(&e);
		break;
	case ClientMessage:
		if (e.xclient.data.l[0] == x11_app.wm_delete) {
			// TODO
		}
		break;
	default:
		break;
	}
	app_event_destroy(&e);
	return TRUE;
}

static void x11_app_wait_event(void)
{
	int fd;
	fd_set fdset;
	struct timeval tv;
	XFlush(x11_app.display);
	fd = ConnectionNumber(x11_app.display);
	if (XEventsQueued(x11_app.display, QueuedAlready)) {
		return TRUE;
	}
	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	tv.tv_sec = 0;
	tv.tv_usec = 10000;
	if (select(fd + 1, &fdset, NULL, NULL, &tv) == 1) {
		return XPending(x11_app.display);
	}
	return FALSE;
}

static void x11_app_process_native_events(void)
{
	MSG msg;
	LinkedListNode *node;
	app_native_event_listener_t *listener;

	while (XEventsQueued(x11_app.display, QueuedAlready)) {
		XNextEvent(x11_app.display, &xe);
		x11_app_process_event(&xe);
		for (LinkedList_Each(node, &x11_app.native_listeners)) {
			listener = node->data;
			if (listener->type == msg.message) {
				listener->handler(&ex, listener->data);
			}
		}
	}
}

static int app_add_native_event_listener(int event_type,
					 app_event_handler_t handler,
					 void *data)
{
	app_native_event_listener_t *listener;

	listener = malloc(sizeof(app_native_event_listener_t));
	if (!listener) {
		return -1;
	}
	listener->handler = handler;
	listener->data = data;
	listener->type = event_type;
	LinkedList_Append(&x11_app.native_listeners, listener);
	return 0;
}

static int app_remove_native_event_listener(int event_type,
					    app_event_handler_t handler)
{
	LinkedListNode *node, *prev;
	app_native_event_listener_t *listener;

	for (LinkedList_Each(node, &x11_app.native_listeners)) {
		prev = node->prev;
		listener = node->data;
		if (listener->handler == handler &&
		    listener->type == event_type) {
			LinkedList_DeleteNode(&x11_app.native_listeners,
					      node);
			free(listener);
			node = prev;
			return 0;
		}
	}
	return -1;
}

static void x11_app_get_screen_width(void)
{
	Screen *s = DefaultScreenOfDisplay(x11_app.display);
	return XWidthOfScreen(s);
}

static int x11_app_get_screen_height(void)
{
	Screen *s = DefaultScreenOfDisplay(x11_app.display);
	return XHeightOfScreen(s);
}

static void x11_app_window_activate(app_window_t *wnd)
{
	XSetWMProtocols(x11_app.display, w->handle, &x11_app.wm_delete, 1);
	XSelectInput(x11_app.display, w->handle,
		     ExposureMask | KeyPressMask | ButtonPress |
			 StructureNotifyMask | ButtonReleaseMask |
			 KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
			 PointerMotionMask | Button1MotionMask |
			 VisibilityChangeMask);
	XFlush(x11_app.display);
	x11_app_window_show(wnd);
}

static app_window_t *x11_app_create_window(const wchar_t *title, int x, int y,
					   int width, int height,
					   app_window_t *parent)
{
	app_window_t *wnd;
	unsigned long bdcolor = BlackPixel(x11_app.display, x11_app.screen);
	unsigned long bgcolor = WhitePixel(x11_app.display, x11_app.screen);

	wnd = malloc(sizeof(app_window_t));
	wnd->gc = NULL;
	wnd->ximage = NULL;
	wnd->is_ready = FALSE;
	wnd->node.data = wnd;
	wnd->width = MIN_WIDTH;
	wnd->height = MIN_HEIGHT;
	Graph_Init(&wnd->fb);
	LCUIMutex_Init(&wnd->mutex);
	LinkedList_Init(&wnd->rects);
	wnd->fb.color_type = LCUI_COLOR_TYPE_ARGB;
	wnd->window =
	    XCreateSimpleWindow(x11_app.display, x11_app.win_root, x, y, width,
				height, 1, bdcolor, bgcolor);
	LinkedList_AppendNode(&x11_app.windows, &wnd->node);
	return wnd;
}

static void x11_app_window_destroy(app_window_t *wnd)
{
	LinkedList_Clear(&wnd->rects, free);
	if (wnd->ximage) {
		XDestroyImage(wnd->ximage);
	}
	if (wnd->gc) {
		XFreeGC(x11_app.display, wnd->gc);
	}
	free(wnd);
}

static void x11_app_window_set_title(app_window_t *wnd, const wchar_t *title)
{
	size_t len;
	char *utf8_title;
	XTextProperty name;

	len = LCUI_EncodeString(NULL, title, 0, ENCODING_UTF8) + 1;
	utf8_title = malloc(sizeof(char) * len);
	if (!title) {
		return;
	}
	len = LCUI_EncodeString(utf8_title, title, len, ENCODING_UTF8);
	name.value = (unsigned char *)utf8_title;
	name.encoding = XA_STRING;
	name.format = 8 * sizeof(char);
	name.nitems = len;
	XSetWMName(x11_app.display, wnd->handle, &name);
	free(utf8_title);
}

static void x11_app_window_set_size(app_window_t *wnd, int width, int height)
{
	int depth;
	XGCValues gcv;
	Visual *visual;

	if (width == wnd->width && height == wnd->height && wnd->ximage &&
	    wnd->gc) {
		return;
	}
	if (wnd->ximage) {
		XDestroyImage(wnd->ximage);
		wnd->ximage = NULL;
	}
	if (wnd->gc) {
		XFreeGC(x11.app->display, wnd->gc);
		wnd->gc = NULL;
	}
	Graph_Init(&wnd->fb);
	wnd->width = width;
	wnd->height = height;
	depth = DefaultDepth(x11.app->display, x11.app->screen);
	switch (depth) {
	case 32:
	case 24:
		wnd->fb.color_type = LCUI_COLOR_TYPE_ARGB;
		break;
	default:
		Logger_Error("[x11_app] unsupport depth: %d.\n", depth);
		break;
	}
	Graph_Create(&wnd->fb, width, height);
	visual = DefaultVisual(x11.app->display, x11.app->screen);
	wnd->ximage =
	    XCreateImage(x11.app->display, visual, depth, ZPixmap, 0,
			 (char *)(wnd->fb.bytes), width, height, 32, 0);
	if (!wnd->ximage) {
		Graph_Free(&wnd->fb);
		Logger_Error("[x11_app] create XImage faild.\n");
		return;
	}
	gcv.graphics_exposures = False;
	wnd->gc =
	    XCreateGC(x11.app->display, wnd->window, GCGraphicsExposures, &gcv);
	if (!wnd->gc) {
		Logger_Error("[x11_app] create graphics context faild.\n");
		return;
	}
	XResizeWindow(x11_app.display, wnd->window, width, height);
}

static void x11_app_window_set_position(app_window_t *wnd, int x, int y)
{
	XMoveWindow(x11_app.display, wnd->window, x, y);
}

static void *x11_app_window_get_handle(app_window_t *wnd)
{
	return wnd->window;
}

static int x11_app_window_get_width(app_window_t *wnd)
{
	return wnd->width;
}

static int x11_app_window_get_height(app_window_t *wnd)
{
	return wnd->height;
}

static void x11_app_window_set_min_width(app_window_t *wnd, int min_width)
{
	wnd->min_width = min_width;
}

static void x11_app_window_set_min_height(app_window_t *wnd, int min_height)
{
	wnd->min_height = min_height;
}

static void x11_app_window_set_max_width(app_window_t *wnd, int max_width)
{
	wnd->max_width = max_width;
}

static void x11_app_window_set_max_height(app_window_t *wnd, int max_height)
{
	wnd->max_height = max_height;
}

static void x11_app_window_show(app_window_t *wnd)
{
	XMapWindow(x11_app.display, wnd->window);
}

static void x11_app_window_hide(app_window_t *wnd)
{
	XUnmapWindow(x11_app.display, wnd->window);
}

static app_window_paint_t *x11_app_window_begin_paint(app_window_t *wnd,
					       LCUI_Rect *rect)
{
	LCUI_PaintContext paint;

	paint = malloc(sizeof(LCUI_PaintContextRec));
	paint->rect = *rect;
	paint->with_alpha = FALSE;
	Graph_Init(&paint->canvas);
	LCUIRect_ValidateArea(&paint->rect, wnd->width, wnd->height);
	Graph_Quote(&paint->canvas, &wnd->fb, &paint->rect);
	Graph_FillRect(&paint->canvas, RGB(255, 255, 255), NULL, TRUE);
	return paint;
}

static void x11_app_window_end_paint(app_window_t *wnd,
				     app_window_paint_t *paint)
{
	LCUI_Rect *rect;

	rect = malloc(sizeof(LCUI_Rect));
	*rect = paint->rect;
	LinkedList_Append(&wnd->rects, rect);
	Graph_Init(paint->canvas);
	free(paint);
}

static void x11_app_window_present(app_window_t *wnd)
{
	LinkedListNode *node;
	LinkedList rects;
	LCUI_Rect *rect;

	LinkedList_Init(&rects);
	LinkedList_Concat(&rects, &wnd->rects);
	for (LinkedList_Each(node, &rects)) {
		rect = node->data;
		XPutImage(x11_app.display, wnd->window, wnd->gc, wnd->ximage,
			  rect->x, rect->y, rect->x, rect->y, rect->width,
			  rect->height);
	}
	LinkedList_Clear(&rects, free);
}

static void x11_app_present(void)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &rects)) {
		x11_app_window_present(node->data);
	}
}

static void x11_app_init(void)
{
	x11_app.display = XOpenDisplay(NULL);
	if (!x11_app.display) {
		free(app);
		return NULL;
	}
	x11_app.screen = DefaultScreen(x11_app.display);
	x11_app.win_root = RootWindow(x11_app.display, x11_app.screen);
	x11_app.cmap = DefaultColormap(x11_app.display, x11_app.screen);
	x11_app.wm_delete =
	    XInternAtom(x11_app.display, "WM_DELETE_WINDOW", FALSE);
}

static void x11_app_destroy(void)
{
	XCloseDisplay(x11_app.display);
}

void x11_app_driver_init(app_driver_t *dirver)
{
	driver->init = x11_app_init;
	driver->destroy = x11_app_destroy;
	driver->wait_event = x11_app_wait_event;
	driver->process_events = x11_app_process_events;
	driver->present = x11_app_present;
}

void x11_app_window_driver_init(app_window_driver_t *dirver)
{
	driver->show = x11_app_window_show;
	driver->hide = x11_app_window_hide;
	driver->activate = x11_app_window_activate;
	driver->set_title = x11_app_window_set_title;
	driver->set_size = x11_app_window_set_size;
	driver->get_width = x11_app_window_get_width;
	driver->get_height = x11_app_window_get_height;
	driver->get_handle = x11_app_window_get_handle;
	driver->set_max_width = x11_app_window_set_max_width;
	driver->set_max_height = x11_app_window_set_max_height;
	driver->set_min_width = x11_app_window_set_min_width;
	driver->set_min_height = x11_app_window_set_min_height;
	driver->begin_paint = x11_app_window_begin_paint;
	driver->end_paint = x11_app_window_end_paint;
	driver->present = x11_app_window_present;
}

#endif

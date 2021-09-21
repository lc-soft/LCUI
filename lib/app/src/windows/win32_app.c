#include <app.h>
#ifdef APP_PLATFORM_WIN_DESKTUP
#pragma comment(lib, "User32")
#pragma comment(lib, "Gdi32")
#include <windows.h>
#include "resource.h"
#include "../config.h"

#define MIN_WIDTH 320
#define MIN_HEIGHT 240
#define WIN32_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX)
#define WM_APP_EVENT (WM_USER + 1000)

typedef enum app_window_render_mode_t {
	RENDER_MODE_STRETCH_BLT,
	RENDER_MODE_BIT_BLT
} app_window_render_mode_t;

struct app_window_t {
	HWND hwnd;
	int width, height;
	int min_width, min_height;
	int max_width, max_height;

	app_window_render_mode_t mode;
	HDC hdc_fb;
	HDC hdc_client;
	HBITMAP fb_bmp;
	LCUI_BOOL is_ready;
	LCUI_Graph fb;
	LinkedListNode node;
};

static struct app_t {
	const wchar_t *class_name;
	HINSTANCE main_instance;
	HINSTANCE dll_instance;
	LinkedList windows;

	/** LinkedList<app_native_event_listener_t> */
	LinkedList native_listeners;
} app_win32;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID unused)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	app_win32.dll_instance = hModule;
	return TRUE;
}

void app_process_native_events(void)
{
	MSG msg;
	LinkedListNode *node;
	app_native_event_listener_t *listener;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		for (LinkedList_Each(node, &app_win32.native_listeners)) {
			listener = node->data;
			if (listener->type == msg.message) {
				listener->handler(&msg, listener->data);
			}
		}
	}
}

int app_add_native_event_listener(int event_type, app_event_handler_t handler,
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
	LinkedList_Append(&app_win32.native_listeners, listener);
	return 0;
}

int app_remove_native_event_listener(int event_type, app_event_handler_t handler)
{
	LinkedListNode *node, *prev;
	app_native_event_listener_t *listener;

	for (LinkedList_Each(node, &app_win32.native_listeners)) {
		prev = node->prev;
		listener = node->data;
		if (listener->handler == handler && listener->type == event_type) {
			LinkedList_DeleteNode(&app_win32.native_listeners, node);
			free(listener);
			node = prev;
			return 0;
		}
	}
	return -1;
}

int app_get_screen_width(void)
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int app_get_screen_height(void)
{
	return GetSystemMetrics(SM_CYSCREEN);
}

void *app_window_get_handle(app_window_t *wnd)
{
	return wnd->hwnd;
}

app_window_t *app_get_window_by_handle(void *handle)
{
	LinkedListNode *node;
	for (LinkedList_Each(node, &app_win32.windows)) {
		if (((app_window_t *)node->data)->hwnd == handle) {
			return node->data;
		}
	}
	return NULL;
}

static LRESULT CALLBACK app_window_process(HWND hwnd, UINT msg, WPARAM arg1,
					   LPARAM arg2)
{
	static POINT mouse_pos = { 0, 0 };

	app_event_t e = { 0 };
	app_window_t *wnd;

	wnd = app_get_window_by_handle(hwnd);
	e.window = wnd;
	switch (msg) {
	case WM_APP_EVENT:
		return 0;
	case WM_SIZE: {
		e.type = APP_EVENT_SIZE;
		e.size.width = LOWORD(arg2);
		e.size.height = HIWORD(arg2);
		break;
	}
	case WM_GETMINMAXINFO: {
		MINMAXINFO *mminfo = arg2;
		app_minmaxinfo_event_t *info = &e.minmaxinfo;
		int style = GetWindowLong(hwnd, GWL_STYLE);

		e.type = APP_EVENT_MINMAXINFO;
		info->min_width = MIN_WIDTH;
		info->min_height = MIN_HEIGHT;
		info->max_width = GetSystemMetrics(SM_CXMAXTRACK);
		info->max_height = GetSystemMetrics(SM_CYMAXTRACK);
		convert_client_size_to_window_size(wnd, &info->min_width,
						   &info->min_height);
		convert_client_size_to_window_size(wnd, &info->max_width,
						   &info->max_height);
		mminfo->ptMinTrackSize.x = info->min_width;
		mminfo->ptMinTrackSize.y = info->min_height;
		mminfo->ptMaxTrackSize.x = info->max_width;
		mminfo->ptMaxTrackSize.y = info->max_height;
		if (info->min_width == info->max_width &&
		    info->min_height == info->max_height) {
			style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
		} else if (info->min_width == info->max_width ||
			   info->min_height == info->max_height) {
			style &= ~WS_MAXIMIZEBOX;
		}
		SetWindowLong(hwnd, GWL_STYLE, style);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;

		e.type = APP_EVENT_PAINT;
		BeginPaint(hwnd, &ps);
		e.paint.rect.x = ps.rcPaint.left;
		e.paint.rect.y = ps.rcPaint.top;
		e.paint.rect.width = ps.rcPaint.right - e.paint.rect.x;
		e.paint.rect.height = ps.rcPaint.bottom - e.paint.rect.y;
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		LinkedList_Unlink(&app_win32.windows, &wnd->node);
		app_window_destroy(wnd);
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
		e.key.code = arg1;
		e.key.code = arg1;
		e.key.shift_key =
		    (GetKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE;
		e.key.ctrl_key =
		    (GetKeyState(VK_CONTROL) & 0x8000) ? TRUE : FALSE;
		e.type =
		    msg == WM_KEYDOWN ? APP_EVENT_KEYDOWN : APP_EVENT_KEYUP;
		break;
	case WM_MOUSEMOVE: {
		POINT pos;

		GetCursorPos(&pos);
		ScreenToClient(hwnd, &pos);
		e.mouse.x = pos.x;
		e.mouse.y = pos.y;
		e.type = APP_EVENT_MOUSEMOVE;
		break;
	}
	case WM_LBUTTONDOWN:
		e.type = APP_EVENT_MOUSEDOWN;
		e.mouse.button = MOUSE_BUTTON_LEFT;
		e.mouse.x = mouse_pos.x;
		e.mouse.y = mouse_pos.y;
		SetCapture(hwnd);
		break;
	case WM_LBUTTONUP:
		e.type = APP_EVENT_MOUSEUP;
		e.mouse.button = MOUSE_BUTTON_LEFT;
		e.mouse.x = mouse_pos.x;
		e.mouse.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		e.type = APP_EVENT_MOUSEDOWN;
		e.mouse.button = MOUSE_BUTTON_RIGHT;
		e.mouse.x = mouse_pos.x;
		e.mouse.y = mouse_pos.y;
		SetCapture(hwnd);
		break;
	case WM_RBUTTONUP:
		e.type = APP_EVENT_MOUSEUP;
		e.mouse.button = MOUSE_BUTTON_RIGHT;
		e.mouse.x = mouse_pos.x;
		e.mouse.y = mouse_pos.y;
		ReleaseCapture();
		break;
	case WM_MOUSEWHEEL:
		e.type = APP_EVENT_MOUSEWHEEL;
		e.wheel.delta_y = GET_WHEEL_DELTA_WPARAM(arg1);
		e.wheel.delta_mode = APP_WHEEL_DELTA_LINE;
		break;
#ifdef ENABLE_TOUCH
	case WM_TOUCH: {
		POINT pos;
		UINT i, n = LOWORD(arg1);
		PTOUCHINPUT inputs = malloc(sizeof(TOUCHINPUT) * n);
		HTOUCHINPUT handle = (HTOUCHINPUT)arg2;

		if (inputs == NULL) {
			break;
		}
		e.type = APP_EVENT_TOUCH;
		e.touch.n_points = n;
		e.touch.points = malloc(sizeof(touch_point_t) * n);
		if (e.touch.points == NULL) {
			free(inputs);
			break;
		}
		if (!GetTouchInputInfo(handle, n, inputs, sizeof(TOUCHINPUT))) {
			free(inputs);
			break;
		}
		for (i = 0; i < n; ++i) {
			pos.x = inputs[i].x / 100;
			pos.y = inputs[i].y / 100;
			ScreenToClient(hwnd, &pos);
			e.touch.points[i].x = pos.x;
			e.touch.points[i].y = pos.y;
			e.touch.points[i].id = inputs[i].dwID;
			if (inputs[i].dwFlags & TOUCHEVENTF_PRIMARY) {
				e.touch.points[i].is_primary = TRUE;
			} else {
				e.touch.points[i].is_primary = FALSE;
			}
			if (inputs[i].dwFlags & TOUCHEVENTF_DOWN) {
				e.touch.points[i].state = APP_EVENT_TOUCHDOWN;
			} else if (inputs[i].dwFlags & TOUCHEVENTF_UP) {
				e.touch.points[i].state = APP_EVENT_TOUCHUP;
			} else if (inputs[i].dwFlags & TOUCHEVENTF_MOVE) {
				e.touch.points[i].state = APP_EVENT_TOUCHMOVE;
			}
		}
		free(inputs);
		if (!CloseTouchInputHandle(handle)) {
			break;
		}
		break;
	}
#endif
	default:
		return DefWindowProc(hwnd, msg, arg1, arg2);
	}
	app_post_event(&e);
}

app_window_t *app_window_create(const wchar_t *title, int x, int y, int width,
				int height, app_window_t *parent)
{
	app_window_t *wnd;

	wnd = malloc(sizeof(app_window_t));
	if (!wnd) {
		return NULL;
	}
	if (x == APP_WINDOW_DEFAULT_X) {
		x = CW_USEDEFAULT;
	} else if (x == APP_WINDOW_CENTER_X) {
		x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	}
	if (y == APP_WINDOW_DEFAULT_Y) {
		y = CW_USEDEFAULT;
	} else if (y == APP_WINDOW_CENTER_Y) {
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}
	wnd->mode = RENDER_MODE_BIT_BLT;
	wnd->hwnd = NULL;
	wnd->hdc_fb = NULL;
	wnd->hdc_client = NULL;
	wnd->fb_bmp = NULL;
	wnd->is_ready = FALSE;
	wnd->node.data = wnd;
	wnd->min_width = MIN_WIDTH;
	wnd->min_height = MIN_HEIGHT;
	wnd->max_width = GetSystemMetrics(SM_CXMAXTRACK);
	wnd->max_height = GetSystemMetrics(SM_CYMAXTRACK);
	Graph_Init(&wnd->fb);
	wnd->fb.color_type = LCUI_COLOR_TYPE_ARGB;
	wnd->hwnd =
	    CreateWindowW(app_win32.class_name, title, WIN32_WINDOW_STYLE, x, y,
			  width, height, parent ? parent->hwnd : NULL, NULL,
			  app_win32.main_instance, NULL);
#ifdef ENABLE_TOUCH
	RegisterTouchWindow(wnd->hwnd, 0);
#endif
	wnd->hdc_client = GetDC(wnd->hwnd);
	wnd->hdc_fb = CreateCompatibleDC(wnd->hdc_client);
	LinkedList_AppendNode(&app_win32.windows, &wnd->node);
	return wnd;
}

void app_window_close(app_window_t *wnd)
{
	PostMessage(wnd->hwnd, WM_CLOSE, 0, 0);
}

void app_window_destroy(app_window_t *wnd)
{
	wnd->width = 0;
	wnd->height = 0;
	if (wnd->fb_bmp) {
		DeleteObject(wnd->fb_bmp);
	}
	if (wnd->hwnd) {
		if (wnd->hdc_fb) {
			/* DeleteDC only for CreateDC */
			DeleteDC(wnd->hdc_fb);
		}
		if (wnd->hdc_client) {
			/* ReleaseDC only for GetDC */
			ReleaseDC(wnd->hwnd, wnd->hdc_client);
		}
	}
	wnd->hwnd = NULL;
	wnd->fb_bmp = NULL;
	wnd->hdc_fb = NULL;
	wnd->hdc_client = NULL;
	Graph_Free(&wnd->fb);
	free(wnd);
}

void app_window_set_position(app_window_t *wnd, int x, int y)
{
	x += GetSystemMetrics(SM_CXFIXEDFRAME);
	y += GetSystemMetrics(SM_CYFIXEDFRAME);
	SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, x, y, 0, 0,
		     SWP_NOSIZE | SWP_NOZORDER);
}

void app_window_set_framebuffer_size(app_window_t *wnd, int width, int height)
{
	HBITMAP old_bmp;

	if (wnd->width == width && wnd->height == height) {
		return;
	}
	Graph_Create(&wnd->fb, width, height);
	wnd->fb_bmp = CreateCompatibleBitmap(wnd->hdc_client, width, height);
	old_bmp = (HBITMAP)SelectObject(wnd->hdc_fb, wnd->fb_bmp);
	if (old_bmp) {
		DeleteObject(old_bmp);
	}
	wnd->width = width;
	wnd->height = height;
}

static void convert_client_size_to_window_size(app_window_t *wnd, int *width,
					       int *height)
{
	RECT rect_client, rect_window;
	GetClientRect(wnd->hwnd, &rect_client);
	GetWindowRect(wnd->hwnd, &rect_window);
	*width += rect_window.right - rect_window.left;
	*width -= rect_client.right - rect_client.left;
	*height += rect_window.bottom - rect_window.top;
	*height -= rect_client.bottom - rect_client.top;
}

void app_window_set_size(app_window_t *wnd, int width, int height)
{
	if (wnd->width == width && wnd->height == height) {
		return;
	}
	app_window_set_framebuffer_size(wnd, width, height);
	convert_client_size_to_window_size(wnd, &width, &height);
	SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, 0, 0, width, height,
		     SWP_NOMOVE | SWP_NOZORDER);
}

void app_window_show(app_window_t *wnd)
{
	ShowWindow(wnd->hwnd, SW_SHOWNORMAL);
}

void app_window_hide(app_window_t *wnd)
{
	ShowWindow(wnd->hwnd, SW_HIDE);
}

void app_window_set_title(app_window_t *wnd, const wchar_t *title)
{
	SetWindowTextW(wnd->hwnd, title);
}

int app_window_get_width(app_window_t *wnd)
{
	return wnd->width;
}

int app_window_get_height(app_window_t *wnd)
{
	return wnd->height;
}

void app_window_set_min_width(app_window_t *wnd, int min_width)
{
	wnd->min_width = min_width;
}

void app_window_set_min_height(app_window_t *wnd, int min_height)
{
	wnd->min_height = min_height;
}

void app_window_set_max_width(app_window_t *wnd, int max_width)
{
	wnd->max_width = max_width;
}

void app_window_set_max_height(app_window_t *wnd, int max_height)
{
	wnd->max_height = max_height;
}

app_window_paint_t *app_window_begin_paint(app_window_t *wnd, LCUI_Rect *rect)
{
	app_window_paint_t *paint = LCUIPainter_Begin(&wnd->fb, rect);
	Graph_FillRect(&paint->canvas, RGB(255, 255, 255), NULL, TRUE);
	return paint;
}

void app_window_end_paint(app_window_t *wnd, app_window_paint_t *paint)
{
	LCUIPainter_End(paint);
}

void app_window_present(app_window_t *wnd)
{
	RECT client_rect;

	SetBitmapBits(wnd->fb_bmp, (DWORD)wnd->fb.mem_size, wnd->fb.bytes);
	switch (wnd->mode) {
	case RENDER_MODE_STRETCH_BLT:
		GetClientRect(wnd->hwnd, &client_rect);
		StretchBlt(wnd->hdc_client, 0, 0, client_rect.right,
			   client_rect.bottom, wnd->hdc_fb, 0, 0, wnd->width,
			   wnd->height, SRCCOPY);
		break;
	case RENDER_MODE_BIT_BLT:
	default:
		BitBlt(wnd->hdc_client, 0, 0, wnd->width, wnd->height,
		       wnd->hdc_fb, 0, 0, SRCCOPY);
		break;
	}
	ValidateRect(wnd->hwnd, NULL);
}

void app_set_instance(void *instance)
{
	app_win32.main_instance = instance;
}

int app_init(const wchar_t *name)
{
	WNDCLASSW wndclass;

	app_win32.class_name = name;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = app_window_process;
	wndclass.lpszClassName = app_win32.class_name;
	wndclass.hInstance = app_win32.main_instance;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon =
	    LoadIcon(app_win32.dll_instance, MAKEINTRESOURCE(IDI_LCUI_ICON));
	if (!RegisterClassW(&wndclass)) {
		wchar_t str[256];
		swprintf(str, 255, __FUNCTIONW__ L": error code: %d\n",
			 GetLastError());
		MessageBoxW(NULL, str, app_win32.class_name, MB_ICONERROR);
		return -1;
	}
	LinkedList_Init(&app_win32.windows);
	LinkedList_Init(&app_win32.native_listeners);
	return 0;
}

void app_quit(void)
{
	LinkedListNode *node;

	for (LinkedList_Each(node, &app_win32.windows)) {
		app_window_close(node->data);
	}
}

void app_destroy(void)
{
	UnregisterClassW(app_win32.class_name, app_win32.main_instance);
	LinkedList_ClearData(&app_win32.windows, app_window_destroy);
}

#endif

/*
 * lib/platform/src/windows/win32_app.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "../app.h"

#ifdef LIBPLAT_WIN_DESKTOP
#pragma comment(lib, "User32")
#pragma comment(lib, "Gdi32")
#include <windows.h>
#include <yutil.h>
#include <pandagl.h>
#include <shellscalingapi.h>

#include "resource.h"

#define MIN_WIDTH 320
#define MIN_HEIGHT 240
#define WIN32_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX)

#define WM_APP_TICK (WM_USER + 'T' + 'I' + 'C' + 'K')

typedef enum app_window_render_mode_t {
        RENDER_MODE_STRETCH_BLT,
        RENDER_MODE_BIT_BLT
} app_window_render_mode_t;

struct app_window_t {
        HWND hwnd;
        DWORD style;
        DWORD ex_style;
        WINDOWPLACEMENT placement;

        int width, height;
        int min_width, min_height;
        int max_width, max_height;
        bool visible;

        app_window_render_mode_t mode;
        HDC hdc_fb;
        HDC hdc_client;
        HBITMAP fb_bmp;
        bool is_ready;
        pd_canvas_t fb;
        list_node_t node;
};

static struct app_t {
        const wchar_t *class_name;
        HINSTANCE main_instance;
        HINSTANCE dll_instance;
        DWORD ui_thread_id;

        int exit_code;

        /** list_t<app_window_t*> */
        list_t windows;

        /** list_t<app_native_event_listener_t*> */
        list_t native_listeners;
} win32_app;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID unused)
{
        switch (reason) {
        case DLL_PROCESS_ATTACH:
                win32_app.dll_instance = hModule;
                break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
                break;
        }
        return true;
}

static DWORD format_error_message(DWORD err, wchar_t *buf, int len)
{
        return FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
                             LANG_NEUTRAL, buf, len, NULL);
}

static void app_set_dpi_awareness(void)
{
        HMODULE shcore = LoadLibrary(TEXT("Shcore.dll"));
        if (shcore) {
                typedef HRESULT(WINAPI * SetProcessDpiAwarenessType)(
                    PROCESS_DPI_AWARENESS);
                SetProcessDpiAwarenessType SetProcessDpiAwareness =
                    (SetProcessDpiAwarenessType)GetProcAddress(
                        shcore, "SetProcessDpiAwareness");
                if (SetProcessDpiAwareness) {
                        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
                }
                FreeLibrary(shcore);
        }
}

static int app_process_native_event(void)
{
        MSG msg;
        BOOL ret;
        DWORD err;
        wchar_t errmsg[256];
        app_event_t e = { 0 };
        list_node_t *node;
        app_native_event_listener_t *listener;

        ret = GetMessage(&msg, NULL, 0, 0);
        app_process_events();
        if (ret == 0) {
                logger_debug("[win32-app] WM_QUIT\n");
                win32_app.exit_code = (int)msg.wParam;
                e.type = APP_EVENT_QUIT;
                app_process_event(&e);
                return 0;
        }
        if (ret == -1) {
                err = GetLastError();
                format_error_message(err, errmsg, 256);
                logger_error("[win32-app] error %d: %ls\n", err, errmsg);
                return -1;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_APP_TICK) {
                e.type = APP_EVENT_TICK;
                app_process_event(&e);
        }
        for (list_each(node, &win32_app.native_listeners)) {
                listener = node->data;
                if (listener->type == msg.message) {
                        listener->handler(&msg, listener->data);
                }
        }
        return ret;
}

static void app_tick(void)
{
        app_event_t e = { 0 };

        e.type = APP_EVENT_TICK;
        app_process_event(&e);
}

static int app_pending(void)
{
        MSG msg;
        return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

int app_process_native_events(app_process_events_option_t option)
{
        int ret = 0;

        win32_app.exit_code = 0;
        if (option == APP_PROCESS_EVENTS_ONE_IF_PRESENT ||
            option == APP_PROCESS_EVENTS_ALL_IF_PRESENT) {
                do {
                        app_tick();
                        if (!app_pending()) {
                                break;
                        }
                        app_process_native_event();
                        app_process_events();
                } while (option == APP_PROCESS_EVENTS_ALL_IF_PRESENT);
                return win32_app.exit_code;
        }
        do {
                for (app_tick(); !app_pending(); app_tick()) {
                        app_process_events();
                        sleep_ms(1);
                }
                ret = app_process_native_event();
                app_process_events();
        } while (option == APP_PROCESS_EVENTS_UNTIL_QUIT && ret != 0);
        return win32_app.exit_code;
}

int app_add_native_event_listener(int event_type,
                                  app_native_event_handler_t handler,
                                  void *data)
{
        app_native_event_listener_t *listener;

        listener = malloc(sizeof(app_native_event_listener_t));
        if (!listener) {
                return -ENOMEM;
        }
        listener->handler = handler;
        listener->data = data;
        listener->type = event_type;
        list_append(&win32_app.native_listeners, listener);
        return 0;
}

int app_remove_native_event_listener(int event_type,
                                     app_native_event_handler_t handler)
{
        list_node_t *node, *prev;
        app_native_event_listener_t *listener;

        for (list_each(node, &win32_app.native_listeners)) {
                prev = node->prev;
                listener = node->data;
                if (listener->handler == handler &&
                    listener->type == event_type) {
                        list_delete_node(&win32_app.native_listeners, node);
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

unsigned app_window_get_dpi(app_window_t *wnd)
{
        return GetDpiForWindow(wnd->hwnd);
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

app_window_t *app_get_window_by_handle(void *handle)
{
        list_node_t *node;

        for (list_each(node, &win32_app.windows)) {
                if (((app_window_t *)node->data)->hwnd == handle) {
                        return node->data;
                }
        }
        return NULL;
}

static void app_window_on_size(app_window_t *wnd, int width, int height)
{
        HBITMAP old_bmp;

        if (wnd->width == width && wnd->height == height) {
                return;
        }
        pd_canvas_create(&wnd->fb, width, height);
        wnd->fb_bmp = CreateCompatibleBitmap(wnd->hdc_client, width, height);
        old_bmp = (HBITMAP)SelectObject(wnd->hdc_fb, wnd->fb_bmp);
        if (old_bmp) {
                DeleteObject(old_bmp);
        }
        wnd->width = width;
        wnd->height = height;
}

static LRESULT CALLBACK app_window_process(HWND hwnd, UINT msg, WPARAM arg1,
                                           LPARAM arg2)
{
        POINT mouse_pos;
        app_event_t e = { 0 };
        app_window_t *wnd;

        wnd = app_get_window_by_handle(hwnd);
        win32_app.ui_thread_id = GetCurrentThreadId();
        e.window = wnd;
        switch (msg) {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
                if (hwnd) {
                        GetCursorPos(&mouse_pos);
                        ScreenToClient(hwnd, &mouse_pos);
                        e.mouse.x = mouse_pos.x;
                        e.mouse.y = mouse_pos.y;
                }
        default:
                break;
        }
        switch (msg) {
        case WM_SHOWWINDOW:
                e.type = APP_EVENT_VISIBILITY_CHANGE;
                e.window->visible = arg1 ? true : false;
                e.visibility_change.visible = e.window->visible;
                break;
        case WM_SIZE: {
                e.type = APP_EVENT_SIZE;
                e.size.width = LOWORD(arg2);
                e.size.height = HIWORD(arg2);
                app_window_on_size(wnd, e.size.width, e.size.height);
                logger_debug(
                    "[win32-app] [window %p] on WM_SIZE, size: (%d, %d)\n", wnd,
                    e.size.width, e.size.height);
                break;
        }
        case WM_GETMINMAXINFO: {
                int style;
                MINMAXINFO *mminfo = (MINMAXINFO *)arg2;
                app_minmaxinfo_event_t *info = &e.minmaxinfo;

                if (!wnd) {
                        break;
                }
                e.type = APP_EVENT_MINMAXINFO;
                style = GetWindowLong(hwnd, GWL_STYLE);
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
        case WM_DPICHANGED: {
                RECT *const new_rect = (RECT *)arg2;

                e.type = APP_EVENT_DPICHANGED;
                e.dpi_change.dpi = HIWORD(arg1);
                SetWindowPos(hwnd, NULL, new_rect->left, new_rect->top,
                             new_rect->right - new_rect->left,
                             new_rect->bottom - new_rect->top,
                             SWP_NOZORDER | SWP_NOACTIVATE);
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
        case WM_CLOSE:
                e.type = APP_EVENT_CLOSE;
                break;
        case WM_DESTROY:
                if (wnd) {
                        list_unlink(&win32_app.windows, &wnd->node);
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
                        pd_canvas_destroy(&wnd->fb);
                        free(wnd);
                }
                return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
                e.key.code = (int)arg1;
                e.key.shift_key =
                    (GetKeyState(VK_SHIFT) & 0x8000) ? true : false;
                e.key.ctrl_key =
                    (GetKeyState(VK_CONTROL) & 0x8000) ? true : false;
                e.type =
                    msg == WM_KEYDOWN ? APP_EVENT_KEYDOWN : APP_EVENT_KEYUP;
                break;
        case WM_MOUSEMOVE:
                e.type = APP_EVENT_MOUSEMOVE;
                break;
        case WM_LBUTTONDOWN:
                e.type = APP_EVENT_MOUSEDOWN;
                e.mouse.button = MOUSE_BUTTON_LEFT;
                SetCapture(hwnd);
                break;
        case WM_LBUTTONUP:
                e.type = APP_EVENT_MOUSEUP;
                e.mouse.button = MOUSE_BUTTON_LEFT;
                ReleaseCapture();
                break;
        case WM_RBUTTONDOWN:
                e.type = APP_EVENT_MOUSEDOWN;
                e.mouse.button = MOUSE_BUTTON_RIGHT;
                SetCapture(hwnd);
                break;
        case WM_RBUTTONUP:
                e.type = APP_EVENT_MOUSEUP;
                e.mouse.button = MOUSE_BUTTON_RIGHT;
                ReleaseCapture();
                break;
        case WM_MOUSEWHEEL:
                e.type = APP_EVENT_WHEEL;
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
                                e.touch.points[i].is_primary = true;
                        } else {
                                e.touch.points[i].is_primary = false;
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
        app_process_event(&e);
        app_event_destroy(&e);
        return 0;
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
        wnd->is_ready = false;
        wnd->node.data = wnd;
        wnd->min_width = MIN_WIDTH;
        wnd->min_height = MIN_HEIGHT;
        wnd->width = width;
        wnd->height = height;
        wnd->visible = false;
        wnd->max_width = GetSystemMetrics(SM_CXMAXTRACK);
        wnd->max_height = GetSystemMetrics(SM_CYMAXTRACK);
        pd_canvas_init(&wnd->fb);
        wnd->fb.color_type = PD_COLOR_TYPE_ARGB;
        list_append_node(&win32_app.windows, &wnd->node);
        wnd->hwnd =
            CreateWindowW(win32_app.class_name, title, WIN32_WINDOW_STYLE, x, y,
                          width, height, parent ? parent->hwnd : NULL, NULL,
                          win32_app.main_instance, NULL);
#ifdef ENABLE_TOUCH
        RegisterTouchWindow(wnd->hwnd, 0);
#endif
        wnd->hdc_client = GetDC(wnd->hwnd);
        wnd->hdc_fb = CreateCompatibleDC(wnd->hdc_client);
        return wnd;
}

void app_window_set_fullscreen(app_window_t *wnd, bool fullscreen)
{
        if (fullscreen) {
                MONITORINFO mi = { sizeof(mi) };
                wnd->style = GetWindowLong(wnd->hwnd, GWL_STYLE);
                wnd->ex_style = GetWindowLong(wnd->hwnd, GWL_EXSTYLE);
                GetWindowPlacement(wnd->hwnd, &wnd->placement);
                SetWindowLong(wnd->hwnd, GWL_STYLE,
                              wnd->style & ~(WS_CAPTION | WS_THICKFRAME));
                SetWindowLong(
                    wnd->hwnd, GWL_EXSTYLE,
                    wnd->ex_style & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE |
                                      WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
                if (GetMonitorInfo(
                        MonitorFromWindow(wnd->hwnd, MONITOR_DEFAULTTOPRIMARY),
                        &mi)) {
                        SetWindowPos(wnd->hwnd, HWND_TOP, mi.rcMonitor.left,
                                     mi.rcMonitor.top,
                                     mi.rcMonitor.right - mi.rcMonitor.left,
                                     mi.rcMonitor.bottom - mi.rcMonitor.top,
                                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
                }
        } else {
                SetWindowLong(wnd->hwnd, GWL_STYLE, wnd->style);
                SetWindowLong(wnd->hwnd, GWL_EXSTYLE, wnd->ex_style);
                SetWindowPlacement(wnd->hwnd, &wnd->placement);
                SetWindowPos(
                    wnd->hwnd, NULL, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }
}

void app_window_activate(app_window_t *wnd)
{
        SetActiveWindow(wnd->hwnd);
}

void app_window_close(app_window_t *wnd)
{
        SendMessage(wnd->hwnd, WM_CLOSE, 0, 0);
}

void app_window_destroy(app_window_t *wnd)
{
        DestroyWindow(wnd->hwnd);
}

void app_window_set_position(app_window_t *wnd, int x, int y)
{
        x += GetSystemMetrics(SM_CXFIXEDFRAME);
        y += GetSystemMetrics(SM_CYFIXEDFRAME);
        SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, x, y, 0, 0,
                     SWP_NOSIZE | SWP_NOZORDER);
}

void app_window_set_size(app_window_t *wnd, int width, int height)
{
        if (wnd->width == width && wnd->height == height) {
                return;
        }
        convert_client_size_to_window_size(wnd, &width, &height);
        SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, 0, 0, width, height,
                     SWP_NOMOVE | SWP_NOZORDER);
}

void app_window_show(app_window_t *wnd)
{
        if (!wnd->visible) {
                ShowWindow(wnd->hwnd, SW_SHOWNORMAL);
        }
}

void app_window_hide(app_window_t *wnd)
{
        if (wnd->visible) {
                ShowWindow(wnd->hwnd, SW_HIDE);
        }
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

app_window_paint_t *app_window_begin_paint(app_window_t *wnd, pd_rect_t *rect)
{
        app_window_paint_t *paint = pd_context_create(&wnd->fb, rect);
        pd_canvas_fill(&paint->canvas, pd_rgb(255, 255, 255));
        return paint;
}

void app_window_end_paint(app_window_t *wnd, app_window_paint_t *paint)
{
        pd_context_destroy(paint);
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
        win32_app.main_instance = instance;
}

app_id_t app_get_id(void)
{
        return APP_ID_WIN32;
}

void app_present(void)
{
        list_node_t *node;

        for (list_each(node, &win32_app.windows)) {
                app_window_present(node->data);
        }
}

int app_init_engine(const wchar_t *name)
{
        WNDCLASSW wndclass = { 0 };

        win32_app.class_name = name;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hbrBackground = NULL;
        wndclass.lpszMenuName = NULL;
        wndclass.lpfnWndProc = app_window_process;
        wndclass.lpszClassName = win32_app.class_name;
        wndclass.hInstance = win32_app.main_instance;
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hIcon =
            LoadIcon(win32_app.dll_instance, MAKEINTRESOURCE(IDI_LCUI_ICON));
        if (!RegisterClassW(&wndclass)) {
                wchar_t str[256];
                wchar_t msg[256];
                DWORD err;

                err = GetLastError();
                format_error_message(err, msg, 256);
                swprintf(str, 255, __FUNCTIONW__ L": error %d: %ls\n", err,
                         msg);
                logger_error("[win32-app] %ls\n", str);
                MessageBoxW(NULL, str, win32_app.class_name, MB_ICONERROR);
                return -1;
        }
        list_create(&win32_app.windows);
        list_create(&win32_app.native_listeners);
        app_set_dpi_awareness();
        return 0;
}

void app_exit(int exit_code)
{
        PostQuitMessage(exit_code);
}

int app_destroy_engine(void)
{
        MSG msg;

        // 销毁未关闭的窗口
        if (win32_app.windows.length > 0) {
                list_destroy_without_node(&win32_app.windows,
                                          app_window_destroy);
                PostQuitMessage(0);
                while (GetMessage(&msg, NULL, 0, 0) != 0) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                }
        }
        assert(win32_app.windows.length == 0);
        assert(FindWindowW(win32_app.class_name, NULL) == NULL);
        if (!UnregisterClassW(win32_app.class_name, win32_app.main_instance)) {
                wchar_t str[256];
                wchar_t msg[256];
                DWORD err;

                err = GetLastError();
                format_error_message(err, msg, 256);
                swprintf(str, 255, __FUNCTIONW__ L": error %d: %ls\n", err,
                         msg);
                logger_error("[win32-app] %ls\n", str);
        }
        return 0;
}

#endif

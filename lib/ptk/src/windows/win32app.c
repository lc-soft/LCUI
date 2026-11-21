/*
 * lib/ptk/src/windows/win32app.c
 *
 * Copyright (c) 2023-2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include "ptk/app.h"
#include "ptk/events.h"
#include "ptk/window.h"

#ifdef PTK_WIN_DESKTOP
#pragma comment(lib, "User32")
#pragma comment(lib, "Gdi32")
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <yutil.h>
#include <pandagl.h>
#include <shellscalingapi.h>

#define MIN_WIDTH 320
#define MIN_HEIGHT 240
#define WIN32_WINDOW_STYLE (WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX)

typedef enum {
        PTK_RENDER_MODE_STRETCH_BLT,
        PTK_RENDER_MODE_BIT_BLT
} ptk_window_render_mode_t;

struct ptk_window {
        HWND hwnd;
        DWORD style;
        DWORD ex_style;
        WINDOWPLACEMENT placement;

        int x, y;
        int width, height;
        int min_width, min_height;
        int max_width, max_height;
        bool visible;
        bool maximized;

        ptk_cursor_type_t cursor;
        HCURSOR hcursor;

        HICON hicon_small;
        HICON hicon_big;

        ptk_window_render_mode_t mode;
        HDC hdc_fb;
        HDC hdc_client;
        HBITMAP fb_bmp;
        bool is_ready;
        pd_canvas_t fb;
        list_node_t node;
};

static struct app {
        const wchar_t *class_name;
        HINSTANCE main_instance;
        HINSTANCE dll_instance;
        DWORD ui_thread_id;

        int exit_code;

        /** list_t<ptk_window_t*> */
        list_t windows;

        /** list_t<ptk_native_event_listener_t*> */
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

static int ptk_process_native_event(void)
{
        MSG msg;
        BOOL ret;
        DWORD err;
        wchar_t errmsg[256];
        ptk_event_t e = { 0 };
        list_node_t *node;
        ptk_native_event_listener_t *listener;

        ret = GetMessage(&msg, NULL, 0, 0);
        ptk_process_events();
        if (ret == 0) {
                win32_app.exit_code = (int)msg.wParam;
                e.type = PTK_EVENT_QUIT;
                ptk_process_event(&e);
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
        for (list_each(node, &win32_app.native_listeners)) {
                listener = node->data;
                if (listener->type == msg.message) {
                        listener->handler(&msg, listener->data);
                }
        }
        return ret;
}

static int ptk_win32_pending(void)
{
        MSG msg;
        return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

int ptk_process_native_events(ptk_process_events_option_t option)
{
        int ret = 0;

        win32_app.exit_code = 0;
        if (option == PTK_PROCESS_EVENTS_ONE_IF_PRESENT ||
            option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT) {
                do {
                        ptk_tick();
                        if (!ptk_win32_pending()) {
                                break;
                        }
                        ptk_process_native_event();
                        ptk_process_events();
                } while (option == PTK_PROCESS_EVENTS_ALL_IF_PRESENT);
                return win32_app.exit_code;
        }
        do {
                for (ptk_tick(); !ptk_win32_pending(); ptk_tick()) {
                        ptk_process_events();
                        y_sleep(1);
                }
                ret = ptk_process_native_event();
                ptk_process_events();
        } while (option == PTK_PROCESS_EVENTS_UNTIL_QUIT && ret != 0);
        return win32_app.exit_code;
}

int ptk_add_native_event_listener(int event_type,
                                  ptk_native_event_handler_t handler,
                                  void *data)
{
        ptk_native_event_listener_t *listener;

        listener = malloc(sizeof(ptk_native_event_listener_t));
        if (!listener) {
                return -ENOMEM;
        }
        listener->handler = handler;
        listener->data = data;
        listener->type = event_type;
        list_append(&win32_app.native_listeners, listener);
        return 0;
}

int ptk_remove_native_event_listener(int event_type,
                                     ptk_native_event_handler_t handler)
{
        list_node_t *node, *prev;
        ptk_native_event_listener_t *listener;

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

int ptk_screen_get_width(void)
{
        return GetSystemMetrics(SM_CXSCREEN);
}

int ptk_screen_get_height(void)
{
        return GetSystemMetrics(SM_CYSCREEN);
}

void *ptk_window_get_handle(ptk_window_t *wnd)
{
        return wnd->hwnd;
}

unsigned ptk_window_get_dpi(ptk_window_t *wnd)
{
        UINT dpi = GetDpiForWindow(wnd->hwnd);
        if (dpi == 0) {
                logger_error("[win32-app] GetDpiForWindow failed, hwnd: %p\n",
                             wnd->hwnd);
                dpi = GetDpiForSystem();
        }
        return dpi;
}

static void convert_client_size_to_window_size(ptk_window_t *wnd, int *width,
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

ptk_window_t *ptk_find_window(void *handle)
{
        list_node_t *node;

        for (list_each(node, &win32_app.windows)) {
                if (((ptk_window_t *)node->data)->hwnd == handle) {
                        return node->data;
                }
        }
        return NULL;
}

static void ptk_window_on_size(ptk_window_t *wnd, int width, int height)
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

static LRESULT CALLBACK ptk_window_process(HWND hwnd, UINT msg, WPARAM arg1,
                                           LPARAM arg2)
{
        POINT mouse_pos;
        ptk_event_t e = { 0 };
        ptk_window_t *wnd;

        wnd = ptk_find_window(hwnd);
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
        case WM_SETCURSOR:
                if (wnd && LOWORD(arg2) == HTCLIENT) {
                        SetCursor(wnd->hcursor); /* NONE 时为 NULL，隐藏指针 */
                        return TRUE;
                }
                /* 非客户区：交给默认处理（系统箭头） */
                return DefWindowProc(hwnd, msg, arg1, arg2);
        case WM_SHOWWINDOW:
                e.type = PTK_EVENT_VISIBILITY_CHANGE;
                e.window->visible = arg1 ? true : false;
                e.visibility_change.visible = e.window->visible;
                break;
        case WM_MOVE:
                if (!wnd) {
                        break;
                }
                e.type = PTK_EVENT_MOVE;
                wnd->x = (int)(short)LOWORD(arg2);
                wnd->y = (int)(short)HIWORD(arg2);
                break;
        case WM_SIZE: {
                e.type = PTK_EVENT_SIZE;
                e.size.width = LOWORD(arg2);
                e.size.height = HIWORD(arg2);
                ptk_window_on_size(wnd, e.size.width, e.size.height);
                break;
        }
        case WM_GETMINMAXINFO: {
                int style;
                MINMAXINFO *mminfo = (MINMAXINFO *)arg2;
                app_minmaxinfo_event_t *info = &e.minmaxinfo;

                if (!wnd) {
                        break;
                }
                e.type = PTK_EVENT_MINMAXINFO;
                style = GetWindowLong(hwnd, GWL_STYLE);
                info->min_width = MIN_WIDTH;
                info->min_height = MIN_HEIGHT;
                info->max_width = GetSystemMetrics(SM_CXMAXTRACK);
                info->max_height = GetSystemMetrics(SM_CYMAXTRACK);
                ptk_process_event(&e);
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
                return 0;
        }
        case WM_DPICHANGED: {
                RECT *const new_rect = (RECT *)arg2;

                e.type = PTK_EVENT_DPICHANGED;
                e.dpi_change.dpi = HIWORD(arg1);
                SetWindowPos(hwnd, NULL, new_rect->left, new_rect->top,
                             new_rect->right - new_rect->left,
                             new_rect->bottom - new_rect->top,
                             SWP_NOZORDER | SWP_NOACTIVATE);
                break;
        }
        case WM_PAINT: {
                PAINTSTRUCT ps;

                e.type = PTK_EVENT_PAINT;
                BeginPaint(hwnd, &ps);
                e.paint.rect.x = ps.rcPaint.left;
                e.paint.rect.y = ps.rcPaint.top;
                e.paint.rect.width = ps.rcPaint.right - e.paint.rect.x;
                e.paint.rect.height = ps.rcPaint.bottom - e.paint.rect.y;
                EndPaint(hwnd, &ps);
                break;
        }
        case WM_CLOSE:
                e.type = PTK_EVENT_CLOSE;
                ptk_process_event(&e);
                ptk_event_destroy(&e);
                return DefWindowProc(hwnd, msg, arg1, arg2);
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
                        if (wnd->hicon_small) {
                                DestroyIcon(wnd->hicon_small);
                                wnd->hicon_small = NULL;
                        }
                        if (wnd->hicon_big) {
                                DestroyIcon(wnd->hicon_big);
                                wnd->hicon_big = NULL;
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
                    msg == WM_KEYDOWN ? PTK_EVENT_KEYDOWN : PTK_EVENT_KEYUP;
                break;
        case WM_MOUSEMOVE:
                e.type = PTK_EVENT_MOUSEMOVE;
                break;
        case WM_LBUTTONDOWN:
                e.type = PTK_EVENT_MOUSEDOWN;
                e.mouse.button = MOUSE_BUTTON_LEFT;
                SetCapture(hwnd);
                break;
        case WM_LBUTTONUP:
                e.type = PTK_EVENT_MOUSEUP;
                e.mouse.button = MOUSE_BUTTON_LEFT;
                ReleaseCapture();
                break;
        case WM_RBUTTONDOWN:
                e.type = PTK_EVENT_MOUSEDOWN;
                e.mouse.button = MOUSE_BUTTON_RIGHT;
                SetCapture(hwnd);
                break;
        case WM_RBUTTONUP:
                e.type = PTK_EVENT_MOUSEUP;
                e.mouse.button = MOUSE_BUTTON_RIGHT;
                ReleaseCapture();
                break;
        case WM_MOUSEWHEEL:
                e.type = PTK_EVENT_WHEEL;
                e.wheel.delta_y = GET_WHEEL_DELTA_WPARAM(arg1);
                e.wheel.delta_mode = APP_WHEEL_DELTA_LINE;
                break;
#ifdef PTK_TOUCH_ENABLED
        case WM_TOUCH: {
                POINT pos;
                UINT i, n = LOWORD(arg1);
                PTOUCHINPUT inputs = malloc(sizeof(TOUCHINPUT) * n);
                HTOUCHINPUT handle = (HTOUCHINPUT)arg2;

                if (inputs == NULL) {
                        break;
                }
                e.type = PTK_EVENT_TOUCH;
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
                                e.touch.points[i].state = PTK_EVENT_TOUCHDOWN;
                        } else if (inputs[i].dwFlags & TOUCHEVENTF_UP) {
                                e.touch.points[i].state = PTK_EVENT_TOUCHUP;
                        } else if (inputs[i].dwFlags & TOUCHEVENTF_MOVE) {
                                e.touch.points[i].state = PTK_EVENT_TOUCHMOVE;
                        }
                }
                free(inputs);
                if (!CloseTouchInputHandle(handle)) {
                        break;
                }
                break;
        }
#endif
        case WM_SETFOCUS:
                e.type = PTK_EVENT_FOCUS;
                break;
        case WM_KILLFOCUS:
                e.type = PTK_EVENT_UNFOCUS;
                break;
        default:
                return DefWindowProc(hwnd, msg, arg1, arg2);
        }
        ptk_process_event(&e);
        ptk_event_destroy(&e);
        return 0;
}

ptk_window_t *ptk_window_create(const wchar_t *title, int x, int y, int width,
                                int height, ptk_window_t *parent)
{
        ptk_window_t *wnd;
        DWORD err;
        wchar_t errmsg[256];

        wnd = malloc(sizeof(ptk_window_t));
        if (x == PTK_WINDOW_DEFAULT_X) {
                x = CW_USEDEFAULT;
        } else if (x == PTK_WINDOW_CENTER_X) {
                x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        }
        if (y == PTK_WINDOW_DEFAULT_Y) {
                y = CW_USEDEFAULT;
        } else if (y == PTK_WINDOW_CENTER_Y) {
                y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
        }
        wnd->mode = PTK_RENDER_MODE_BIT_BLT;
        wnd->hwnd = NULL;
        wnd->hdc_fb = NULL;
        wnd->hdc_client = NULL;
        wnd->fb_bmp = NULL;
        wnd->is_ready = false;
        wnd->cursor = PTK_CURSOR_AUTO;
        wnd->hcursor = LoadCursorW(NULL, IDC_ARROW);
        wnd->hicon_small = NULL;
        wnd->hicon_big = NULL;
        wnd->node.data = wnd;
        wnd->min_width = MIN_WIDTH;
        wnd->min_height = MIN_HEIGHT;
        wnd->x = x;
        wnd->y = y;
        wnd->width = width;
        wnd->height = height;
        wnd->visible = false;
        wnd->maximized = false;
        wnd->max_width = GetSystemMetrics(SM_CXMAXTRACK);
        wnd->max_height = GetSystemMetrics(SM_CYMAXTRACK);
        pd_canvas_init(&wnd->fb);
        wnd->fb.color_type = PD_COLOR_TYPE_ARGB;
        list_append_node(&win32_app.windows, &wnd->node);
        wnd->hwnd =
            CreateWindowW(win32_app.class_name, title, WIN32_WINDOW_STYLE, x, y,
                          width, height, parent ? parent->hwnd : NULL, NULL,
                          win32_app.main_instance, NULL);
        if (!wnd->hwnd) {
                err = GetLastError();
                format_error_message(err, errmsg, 256);
                logger_error(
                    "[win32-app] CreateWindowW failed, error %d: %ls\n", err,
                    errmsg);
                list_unlink(&win32_app.windows, &wnd->node);
                free(wnd);
                return NULL;
        }
#ifdef PTK_TOUCH_ENABLED
        RegisterTouchWindow(wnd->hwnd, 0);
#endif
        wnd->hdc_client = GetDC(wnd->hwnd);
        if (!wnd->hdc_client) {
                err = GetLastError();
                format_error_message(err, errmsg, 256);
                logger_error("[win32-app] GetDC failed, error %d: %ls\n", err,
                             errmsg);
                DestroyWindow(wnd->hwnd);
                list_unlink(&win32_app.windows, &wnd->node);
                free(wnd);
                return NULL;
        }
        wnd->hdc_fb = CreateCompatibleDC(wnd->hdc_client);
        if (!wnd->hdc_fb) {
                err = GetLastError();
                format_error_message(err, errmsg, 256);
                logger_error(
                    "[win32-app] CreateCompatibleDC failed, error %d: %ls\n",
                    err, errmsg);
                ReleaseDC(wnd->hwnd, wnd->hdc_client);
                DestroyWindow(wnd->hwnd);
                list_unlink(&win32_app.windows, &wnd->node);
                free(wnd);
                return NULL;
        }
        return wnd;
}

void ptk_window_set_fullscreen(ptk_window_t *wnd, bool fullscreen)
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

void ptk_window_activate(ptk_window_t *wnd)
{
        SetActiveWindow(wnd->hwnd);
}

void ptk_window_close(ptk_window_t *wnd)
{
        SendMessage(wnd->hwnd, WM_CLOSE, 0, 0);
}

void ptk_window_destroy(ptk_window_t *wnd)
{
        DestroyWindow(wnd->hwnd);
}

void ptk_window_set_position(ptk_window_t *wnd, int x, int y)
{
        POINT p = { 0, 0 };
        RECT rect;

        ClientToScreen(wnd->hwnd, &p);
        GetWindowRect(wnd->hwnd, &rect);
        SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, rect.left + (x - p.x),
                     rect.top + (y - p.y), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void ptk_window_get_position(ptk_window_t *wnd, int *x, int *y)
{
        POINT point = { 0, 0 };

        if (!wnd || !wnd->hwnd || !ClientToScreen(wnd->hwnd, &point)) {
                if (x) {
                        *x = 0;
                }
                if (y) {
                        *y = 0;
                }
                return;
        }
        if (x) {
                *x = point.x;
        }
        if (y) {
                *y = point.y;
        }
}

void ptk_window_set_maximized(ptk_window_t *wnd, bool maximized)
{
        if (!wnd || !wnd->hwnd) {
                return;
        }
        wnd->maximized = maximized;
        if (!wnd->visible) {
                return;
        }
        ShowWindow(wnd->hwnd, maximized ? SW_MAXIMIZE : SW_RESTORE);
}

void ptk_window_set_size(ptk_window_t *wnd, int width, int height)
{
        int client_width = width;
        int client_height = height;
        ptk_event_t e = { 0 };

        if (wnd->width == width && wnd->height == height) {
                return;
        }
        convert_client_size_to_window_size(wnd, &width, &height);
        SetWindowPos(wnd->hwnd, HWND_NOTOPMOST, 0, 0, width, height,
                     SWP_NOMOVE | SWP_NOZORDER);
        /* Keep the cached client size in sync immediately. WM_SIZE is
         * asynchronous, so callers that query the size right after set_size()
         * must not observe the previous value. */
        wnd->width = client_width;
        wnd->height = client_height;
        /* Hidden windows may not receive WM_SIZE, so notify listeners now. */
        e.type = PTK_EVENT_SIZE;
        e.window = wnd;
        e.size.width = client_width;
        e.size.height = client_height;
        ptk_process_event(&e);
}

void ptk_window_show(ptk_window_t *wnd)
{
        if (!wnd->visible) {
                ShowWindow(wnd->hwnd, SW_SHOWNORMAL);
        }
        if (wnd->maximized) {
                ShowWindow(wnd->hwnd, SW_MAXIMIZE);
        }
}

void ptk_window_hide(ptk_window_t *wnd)
{
        if (wnd->visible) {
                ShowWindow(wnd->hwnd, SW_HIDE);
        }
}

void ptk_window_set_title(ptk_window_t *wnd, const wchar_t *title)
{
        SetWindowTextW(wnd->hwnd, title);
}

static HICON ptk_win32_create_icon(const ptk_icon_image_t *image)
{
        BITMAPINFOHEADER bi = { 0 };
        ICONINFO info = { 0 };
        HBITMAP color;
        HBITMAP mask;
        HICON icon;
        void *bits;
        unsigned char *mask_bits;
        int mask_stride;
        size_t size;

        size = (size_t)image->width * image->height * sizeof(uint32_t);
        bi.biSize = sizeof(bi);
        bi.biWidth = image->width;
        bi.biHeight = -image->height;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        color = CreateDIBSection(NULL, (BITMAPINFO *)&bi, DIB_RGB_COLORS, &bits,
                                 NULL, 0);
        if (!color) {
                return NULL;
        }
        memcpy(bits, image->argb, size);

        mask_stride = ((image->width + 15) / 16) * 2;
        mask_bits = calloc(1, (size_t)mask_stride * image->height);
        if (!mask_bits) {
                DeleteObject(color);
                return NULL;
        }
        mask = CreateBitmap(image->width, image->height, 1, 1, mask_bits);
        free(mask_bits);
        if (!mask) {
                DeleteObject(color);
                return NULL;
        }

        info.fIcon = TRUE;
        info.hbmColor = color;
        info.hbmMask = mask;
        icon = CreateIconIndirect(&info);
        DeleteObject(color);
        DeleteObject(mask);
        return icon;
}

static int ptk_win32_icon_size_for_dpi(unsigned dpi, bool big)
{
        unsigned scale = dpi * 100 / 96;

        if (scale >= 175) {
                return big ? 64 : 32;
        }
        if (scale >= 125) {
                return big ? 48 : 24;
        }
        return big ? 32 : 16;
}

static const ptk_icon_image_t *ptk_win32_pick_icon(
    const ptk_icon_image_t *images, int count, int size)
{
        int i, diff;
        int best_diff = INT_MAX;
        const ptk_icon_image_t *best = NULL;

        for (i = 0; i < count; ++i) {
                if (images[i].width <= 0 || images[i].height <= 0 ||
                    !images[i].argb) {
                        continue;
                }
                diff =
                    abs(images[i].width - size) + abs(images[i].height - size);
                if (diff < best_diff) {
                        best = &images[i];
                        best_diff = diff;
                }
        }
        return best;
}

void ptk_window_set_icons(ptk_window_t *wnd, const ptk_icon_image_t *images,
                          int count)
{
        HICON small_icon = NULL;
        HICON big_icon = NULL;
        const ptk_icon_image_t *image;
        unsigned dpi;
        int small_size;
        int big_size;

        if (!wnd || !wnd->hwnd) {
                return;
        }
        if (images && count > 0) {
                dpi = ptk_window_get_dpi(wnd);
                small_size = ptk_win32_icon_size_for_dpi(dpi, false);
                big_size = ptk_win32_icon_size_for_dpi(dpi, true);
                image = ptk_win32_pick_icon(images, count, small_size);
                if (image) {
                        small_icon = ptk_win32_create_icon(image);
                }
                image = ptk_win32_pick_icon(images, count, big_size);
                if (image) {
                        big_icon = ptk_win32_create_icon(image);
                }
                if (!small_icon || !big_icon) {
                        if (small_icon) {
                                DestroyIcon(small_icon);
                        }
                        if (big_icon) {
                                DestroyIcon(big_icon);
                        }
                        logger_error(
                            "[win32-app] failed to create window icon\n");
                        return;
                }
        }
        if (wnd->hicon_small) {
                DestroyIcon(wnd->hicon_small);
        }
        if (wnd->hicon_big) {
                DestroyIcon(wnd->hicon_big);
        }
        wnd->hicon_small = small_icon;
        wnd->hicon_big = big_icon;
        SendMessageW(wnd->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)small_icon);
        SendMessageW(wnd->hwnd, WM_SETICON, ICON_BIG, (LPARAM)big_icon);
}

/* 将光标类型映射为系统光标句柄；PTK_CURSOR_NONE 返回 NULL 以隐藏指针。
 * 无法精确对应的值取最接近的系统光标（见 spec 平台近似映射表）。 */
static HCURSOR ptk_load_system_cursor_by_type(ptk_cursor_type_t type)
{
        switch (type) {
        case PTK_CURSOR_HELP:
                return LoadCursor(NULL, IDC_HELP);
        case PTK_CURSOR_POINTER:
        case PTK_CURSOR_GRAB:
        case PTK_CURSOR_GRABBING:
                return LoadCursor(NULL, IDC_HAND);
        case PTK_CURSOR_PROGRESS:
                return LoadCursor(NULL, IDC_APPSTARTING);
        case PTK_CURSOR_WAIT:
                return LoadCursor(NULL, IDC_WAIT);
        case PTK_CURSOR_CELL:
        case PTK_CURSOR_CROSSHAIR:
                return LoadCursor(NULL, IDC_CROSS);
        case PTK_CURSOR_TEXT:
        case PTK_CURSOR_VERTICAL_TEXT:
                return LoadCursor(NULL, IDC_IBEAM);
        case PTK_CURSOR_MOVE:
        case PTK_CURSOR_ALL_SCROLL:
                return LoadCursor(NULL, IDC_SIZEALL);
        case PTK_CURSOR_NO_DROP:
        case PTK_CURSOR_NOT_ALLOWED:
                return LoadCursor(NULL, IDC_NO);
        case PTK_CURSOR_COL_RESIZE:
        case PTK_CURSOR_EW_RESIZE:
        case PTK_CURSOR_E_RESIZE:
        case PTK_CURSOR_W_RESIZE:
                return LoadCursor(NULL, IDC_SIZEWE);
        case PTK_CURSOR_ROW_RESIZE:
        case PTK_CURSOR_NS_RESIZE:
        case PTK_CURSOR_N_RESIZE:
        case PTK_CURSOR_S_RESIZE:
                return LoadCursor(NULL, IDC_SIZENS);
        case PTK_CURSOR_NW_RESIZE:
        case PTK_CURSOR_SE_RESIZE:
        case PTK_CURSOR_NWSE_RESIZE:
                return LoadCursor(NULL, IDC_SIZENWSE);
        case PTK_CURSOR_NE_RESIZE:
        case PTK_CURSOR_SW_RESIZE:
        case PTK_CURSOR_NESW_RESIZE:
                return LoadCursor(NULL, IDC_SIZENESW);
        case PTK_CURSOR_NONE:
                return NULL;
        default:
                return LoadCursor(NULL, IDC_ARROW);
        }
}

void ptk_window_set_cursor(ptk_window_t *wnd, ptk_cursor_type_t cursor)
{
        if (wnd->cursor == cursor) {
                return;
        }
        wnd->cursor = cursor;
        wnd->hcursor = ptk_load_system_cursor_by_type(cursor);
        SetCursor(wnd->hcursor);
}

ptk_cursor_type_t ptk_window_get_cursor(ptk_window_t *wnd)
{
        return wnd->cursor;
}

int ptk_window_get_width(ptk_window_t *wnd)
{
        return wnd->width;
}

int ptk_window_get_height(ptk_window_t *wnd)
{
        return wnd->height;
}

bool ptk_window_is_maximized(ptk_window_t *wnd)
{
        return wnd && wnd->hwnd && IsZoomed(wnd->hwnd) ? true : false;
}

bool ptk_window_is_visible(ptk_window_t *wnd)
{
        return wnd && wnd->visible;
}

void ptk_window_set_min_width(ptk_window_t *wnd, int min_width)
{
        wnd->min_width = min_width;
}

void ptk_window_set_min_height(ptk_window_t *wnd, int min_height)
{
        wnd->min_height = min_height;
}

void ptk_window_set_max_width(ptk_window_t *wnd, int max_width)
{
        wnd->max_width = max_width;
}

void ptk_window_set_max_height(ptk_window_t *wnd, int max_height)
{
        wnd->max_height = max_height;
}

ptk_window_paint_t *ptk_window_begin_paint(ptk_window_t *wnd, pd_rect_t *rect)
{
        ptk_window_paint_t *paint = pd_context_create(&wnd->fb, rect);
        pd_canvas_fill(&paint->canvas, pd_rgb(255, 255, 255));
        return paint;
}

void ptk_window_end_paint(ptk_window_t *wnd, ptk_window_paint_t *paint)
{
        pd_context_destroy(paint);
}

void ptk_window_present(ptk_window_t *wnd)
{
        RECT client_rect;

        SetBitmapBits(wnd->fb_bmp, (DWORD)wnd->fb.mem_size, wnd->fb.bytes);
        switch (wnd->mode) {
        case PTK_RENDER_MODE_STRETCH_BLT:
                GetClientRect(wnd->hwnd, &client_rect);
                StretchBlt(wnd->hdc_client, 0, 0, client_rect.right,
                           client_rect.bottom, wnd->hdc_fb, 0, 0, wnd->width,
                           wnd->height, SRCCOPY);
                break;
        case PTK_RENDER_MODE_BIT_BLT:
        default:
                BitBlt(wnd->hdc_client, 0, 0, wnd->width, wnd->height,
                       wnd->hdc_fb, 0, 0, SRCCOPY);
                break;
        }
        ValidateRect(wnd->hwnd, NULL);
}

void ptk_set_instance(void *instance)
{
        win32_app.main_instance = instance;
}

ptk_app_id_t ptk_get_app_id(void)
{
        return PTK_APP_ID_WIN32;
}

void ptk_app_present(void)
{
        list_node_t *node;

        for (list_each(node, &win32_app.windows)) {
                ptk_window_present(node->data);
        }
}

int ptk_app_init(const wchar_t *name)
{
        WNDCLASSW wndclass = { 0 };

        win32_app.class_name = name;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hbrBackground = NULL;
        wndclass.lpszMenuName = NULL;
        wndclass.lpfnWndProc = ptk_window_process;
        wndclass.lpszClassName = win32_app.class_name;
        wndclass.hInstance = win32_app.main_instance;
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
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

void ptk_app_exit(int exit_code)
{
        PostQuitMessage(exit_code);
}

int ptk_app_destroy(void)
{
        MSG msg;

        // 销毁未关闭的窗口
        if (win32_app.windows.length > 0) {
                list_destroy_without_node(&win32_app.windows,
                                          ptk_window_destroy);
                PostQuitMessage(0);
                while (GetMessage(&msg, NULL, 0, 0) != 0) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                }
        }
        assert(win32_app.windows.length == 0);
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

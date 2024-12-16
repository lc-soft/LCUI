/*
 * lib/ptk/include/ptk/types.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_TYPES_H
#define PTK_INCLUDE_PLATFORM_TYPES_H

#include <stdbool.h>
#include <pandagl/types.h>

typedef pd_context_t ptk_window_paint_t;
typedef void (*ptk_timer_cb)(void *);

typedef enum {
	PTK_APP_ID_UNKNOWN,
	PTK_APP_ID_LINUX,
	PTK_APP_ID_LINUX_X11,
	PTK_APP_ID_WIN_DESKTOP,
	PTK_APP_ID_UWP
} ptk_app_id_t;

/** @see
 * https://docs.microsoft.com/en-us/uwp/api/Windows.UI.Core.CoreProcessEventsOption?view=winrt-22000
 */
typedef enum {
	PTK_PROCESS_EVENTS_ONE_AND_ALL_PENDING,
	PTK_PROCESS_EVENTS_ONE_IF_PRESENT,
	PTK_PROCESS_EVENTS_UNTIL_QUIT,
	PTK_PROCESS_EVENTS_ALL_IF_PRESENT,
} ptk_process_events_option_t;

#define PTK_APP_ID_WIN32 PTK_APP_ID_WIN_DESKTOP

typedef enum {
	PTK_EVENT_NONE,
	PTK_EVENT_TICK,
	PTK_EVENT_QUIT,
	PTK_EVENT_CLOSE,
	PTK_EVENT_PAINT,
	PTK_EVENT_SIZE,
	PTK_EVENT_MINMAXINFO,
	PTK_EVENT_VISIBILITY_CHANGE,
	PTK_EVENT_KEYDOWN,
	PTK_EVENT_KEYPRESS,
	PTK_EVENT_KEYUP,
	PTK_EVENT_MOUSE,
	PTK_EVENT_MOUSEMOVE,
	PTK_EVENT_MOUSEDOWN,
	PTK_EVENT_MOUSEUP,
	PTK_EVENT_WHEEL,
	PTK_EVENT_COMPOSITION,
	PTK_EVENT_TOUCH,
	PTK_EVENT_TOUCHMOVE,
	PTK_EVENT_TOUCHDOWN,
	PTK_EVENT_TOUCHUP,
	PTK_EVENT_DPICHANGED,
	PTK_EVENT_USER = 100
} ptk_event_type_t;

#define PTK_EVENT_TEXTINPUT PTK_EVENT_COMPOSITION

typedef struct ptk_event ptk_event_t;
typedef struct ptk_window ptk_window_t;

typedef void (*ptk_event_handler_t)(ptk_event_t *, void *);
typedef int (*ptk_event_dispatcher_t)(ptk_event_t *);

typedef struct ptk_event_listener {
	ptk_event_type_t type;
	ptk_event_handler_t handler;
	void *data;
} ptk_event_listener_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/keyCode
 */
typedef enum {
	KEY_BACKSPACE = 8,
	KEY_TAB = 9,
	KEY_ENTER = 13,
	KEY_SHIFT = 16,
	KEY_CONTROL = 17,
	KEY_ALT = 18,
	KEY_CAPITAL = 20,
	KEY_ESCAPE = 27,
	KEY_SPACE = ' ',
	KEY_PAGEUP = 33,
	KEY_PAGEDOWN = 34,
	KEY_END = 35,
	KEY_HOME = 36,
	KEY_LEFT = 37,
	KEY_UP = 38,
	KEY_RIGHT = 39,
	KEY_DOWN = 40,
	KEY_INSERT = 45,
	KEY_DELETE = 46,
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_A = 'A',
	KEY_B = 'B',
	KEY_C = 'C',
	KEY_D = 'D',
	KEY_E = 'E',
	KEY_F = 'F',
	KEY_G = 'G',
	KEY_H = 'H',
	KEY_I = 'I',
	KEY_J = 'J',
	KEY_K = 'K',
	KEY_L = 'L',
	KEY_M = 'M',
	KEY_N = 'N',
	KEY_O = 'O',
	KEY_P = 'P',
	KEY_Q = 'Q',
	KEY_R = 'R',
	KEY_S = 'S',
	KEY_T = 'T',
	KEY_U = 'U',
	KEY_V = 'V',
	KEY_W = 'W',
	KEY_X = 'X',
	KEY_Y = 'Y',
	KEY_Z = 'Z',
	KEY_SEMICOLON = 186,
	KEY_EQUAL = 187,
	KEY_COMMA = 188,
	KEY_MINUS = 189,
	KEY_PERIOD = 190,
	KEY_SLASH = 191,
	KEY_GRAVE = 192,
	KEY_BRACKETLEFT = 219,
	KEY_BACKSLASH = 220,
	KEY_BRACKETRIGHT = 221,
	KEY_APOSTROPHE = 222
} ptk_key_t;

/**
 * @see
 * https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent/button#return_value
 */
typedef enum mouse_button_code_t {
	MOUSE_BUTTON_MAIN,
	MOUSE_BUTTON_AUXILIARY,
	MOUSE_BUTTON_SECNODARY,
	MOUSE_BUTTON_FORTH_BUTTON,
	MOUSE_BUTTON_FIFTH_BUTTON
} mouse_button_code_t;

#define MOUSE_BUTTON_LEFT MOUSE_BUTTON_MAIN
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_AUXILIARY
#define MOUSE_BUTTON_WHEEL MOUSE_BUTTON_AUXILIARY
#define MOUSE_BUTTON_RIGHT MOUSE_BUTTON_SECNODARY

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent
 */
typedef struct app_keyboard_event {
	int code;
	bool alt_key;
	bool ctrl_key;
	bool shift_key;
	bool meta_key;
	bool is_composing;
} app_keyboard_event_t;

typedef struct touch_point {
	int x;
	int y;
	int id;
	int state;
	bool is_primary;
} touch_point_t;

typedef struct app_touch_event {
	unsigned n_points;
	touch_point_t *points;
} app_touch_event_t;

typedef struct app_paint_event {
	pd_rect_t rect;
} app_paint_event_t;

typedef struct app_textinput_event {
	wchar_t *text;
	size_t length;
} app_textinput_event_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent
 */
typedef struct app_mouse_event {
	int x;
	int y;
	int button;
} app_mouse_event_t;

typedef enum app_wheel_delta_mode_t {
	APP_WHEEL_DELTA_PIXEL = 0,
	APP_WHEEL_DELTA_LINE,
	APP_WHEEL_DELTA_PAGE
} app_wheel_delta_mode_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/WheelEvent
 */
typedef struct app_wheel_event {
	int delta_x;
	int delta_y;
	int delta_z;
	int delta_mode;
} app_wheel_event_t;

typedef struct app_size_event {
	int width;
	int height;
} app_size_event_t;

typedef struct app_minmaxinfo_event {
	int min_width;
	int min_height;
	int max_width;
	int max_height;
} app_minmaxinfo_event_t;

typedef struct app_visibility_change_event {
	bool visible;
} app_visibility_change_event_t;

typedef struct app_dpi_change_event {
	int dpi;
} app_dpi_change_event_t;

struct ptk_event {
	ptk_event_type_t type;
	ptk_window_t *window;
	void *data;

	union {
		app_mouse_event_t mouse;
		app_wheel_event_t wheel;
		app_textinput_event_t composition;
		app_textinput_event_t text;
		app_keyboard_event_t key;
		app_touch_event_t touch;
		app_paint_event_t paint;
		app_size_event_t size;
		app_minmaxinfo_event_t minmaxinfo;
		app_visibility_change_event_t visibility_change;
		app_dpi_change_event_t dpi_change;
	};
};

typedef void ptk_native_event_t;
typedef void (*ptk_native_event_handler_t)(ptk_native_event_t *, void *);

typedef struct ptk_native_event_listener {
	int type;
	ptk_native_event_handler_t handler;
	void *data;
} ptk_native_event_listener_t;

typedef struct ptk_window_driver {
	void (*hide)(ptk_window_t *);
	void (*show)(ptk_window_t *);
	void (*activate)(ptk_window_t *);
	void (*close)(ptk_window_t *);
	void (*destroy)(ptk_window_t *);
	void (*set_title)(ptk_window_t *, const wchar_t *);
	void (*set_size)(ptk_window_t *, int, int);
	void (*set_position)(ptk_window_t *, int, int);
	void *(*get_handle)(ptk_window_t *);
	int (*get_width)(ptk_window_t *);
	int (*get_height)(ptk_window_t *);
	void (*set_min_width)(ptk_window_t *, int);
	void (*set_min_height)(ptk_window_t *, int);
	void (*set_max_width)(ptk_window_t *, int);
	void (*set_max_height)(ptk_window_t *, int);
	ptk_window_paint_t *(*begin_paint)(ptk_window_t *, pd_rect_t *);
	void (*end_paint)(ptk_window_t *, ptk_window_paint_t *);
	void (*present)(ptk_window_t *);
} ptk_window_driver_t;

typedef struct ptk_app_driver {
	int (*init)(const wchar_t *);
	int (*destroy)(void);
	int (*process_events)(ptk_process_events_option_t option);
	int (*on_event)(int type, ptk_native_event_handler_t handler,
			void *data);
	int (*off_event)(int type, ptk_native_event_handler_t handler);
	int (*get_screen_width)(void);
	int (*get_screen_height)(void);
	ptk_window_t *(*create_window)(const wchar_t *title, int x, int y,
				       int width, int height,
				       ptk_window_t *parent);
	ptk_window_t *(*get_window)(void *handle);
	void (*present)(void);
	void (*exit)(int exit_code);
} ptk_app_driver_t;

// Window

#define PTK_WINDOW_CENTER_X ((int)0xe0000000)
#define PTK_WINDOW_CENTER_Y ((int)0xe0000000)
#define PTK_WINDOW_DEFAULT_X ((int)0xf0000000)
#define PTK_WINDOW_DEFAULT_Y ((int)0xf0000000)
#define PTK_WINDOW_DEFAULT_WIDTH 800
#define PTK_WINDOW_DEFAULT_HEIGHT 600

#endif

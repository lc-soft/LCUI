/*
 * lib/platform/include/platform/types.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LIBPLAT_INCLUDE_PLATFORM_TYPES_H
#define LIBPLAT_INCLUDE_PLATFORM_TYPES_H

#include <stdbool.h>
#include <pandagl/types.h>

typedef pd_context_t app_window_paint_t;

typedef enum app_id_t {
	APP_ID_UNKNOWN,
	APP_ID_LINUX,
	APP_ID_LINUX_X11,
	APP_ID_WIN_DESKTOP,
	APP_ID_UWP
} app_id_t;

/** @see
 * https://docs.microsoft.com/en-us/uwp/api/Windows.UI.Core.CoreProcessEventsOption?view=winrt-22000
 */
typedef enum app_process_events_option_t {
	APP_PROCESS_EVENTS_ONE_AND_ALL_PENDING,
	APP_PROCESS_EVENTS_ONE_IF_PRESENT,
	APP_PROCESS_EVENTS_UNTIL_QUIT,
	APP_PROCESS_EVENTS_ALL_IF_PRESENT,
} app_process_events_option_t;

#define APP_ID_WIN32 APP_ID_WIN_DESKTOP

typedef enum app_event_type_t {
	APP_EVENT_NONE,
	APP_EVENT_TICK,
	APP_EVENT_QUIT,
	APP_EVENT_CLOSE,
	APP_EVENT_PAINT,
	APP_EVENT_SIZE,
	APP_EVENT_MINMAXINFO,
	APP_EVENT_VISIBILITY_CHANGE,
	APP_EVENT_KEYDOWN,
	APP_EVENT_KEYPRESS,
	APP_EVENT_KEYUP,
	APP_EVENT_MOUSE,
	APP_EVENT_MOUSEMOVE,
	APP_EVENT_MOUSEDOWN,
	APP_EVENT_MOUSEUP,
	APP_EVENT_WHEEL,
	APP_EVENT_COMPOSITION,
	APP_EVENT_TOUCH,
	APP_EVENT_TOUCHMOVE,
	APP_EVENT_TOUCHDOWN,
	APP_EVENT_TOUCHUP,
	APP_EVENT_USER = 100
} app_event_type_t;

#define APP_EVENT_TEXTINPUT APP_EVENT_COMPOSITION

typedef struct app_event_t app_event_t;
typedef struct app_window_t app_window_t;

typedef void (*app_event_handler_t)(app_event_t *, void *);
typedef int (*app_event_dispatcher_t)(app_event_t *);

typedef struct app_event_listener_t {
	app_event_type_t type;
	app_event_handler_t handler;
	void *data;
} app_event_listener_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/keyCode
 */
enum keycode {
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
};

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
typedef struct app_keyboard_event_t {
	int code;
	bool alt_key;
	bool ctrl_key;
	bool shift_key;
	bool meta_key;
	bool is_composing;
} app_keyboard_event_t;

typedef struct touch_point_t {
	int x;
	int y;
	int id;
	int state;
	bool is_primary;
} touch_point_t;

typedef struct app_touch_event_t {
	unsigned n_points;
	touch_point_t *points;
} app_touch_event_t;

typedef struct app_paint_event_t {
	pd_rect_t rect;
} app_paint_event_t;

typedef struct app_textinput_event_t {
	wchar_t *text;
	size_t length;
} app_textinput_event_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent
 */
typedef struct app_mouse_event_t {
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
typedef struct app_wheel_event_t {
	int delta_x;
	int delta_y;
	int delta_z;
	int delta_mode;
} app_wheel_event_t;

typedef struct app_size_event_t {
	int width;
	int height;
} app_size_event_t;

typedef struct app_minmaxinfo_event_t {
	int min_width;
	int min_height;
	int max_width;
	int max_height;
} app_minmaxinfo_event_t;

typedef struct app_visibility_change_event_t {
	bool visible;
} app_visibility_change_event_t;

struct app_event_t {
	app_event_type_t type;
	app_window_t *window;
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
	};
};

typedef void app_native_event_t;
typedef void (*app_native_event_handler_t)(app_native_event_t *, void *);

typedef struct app_native_event_listener_t {
	int type;
	app_native_event_handler_t handler;
	void *data;
} app_native_event_listener_t;

typedef struct app_window_driver_t {
	void (*hide)(app_window_t *);
	void (*show)(app_window_t *);
	void (*activate)(app_window_t *);
	void (*close)(app_window_t *);
	void (*destroy)(app_window_t *);
	void (*set_title)(app_window_t *, const wchar_t *);
	void (*set_size)(app_window_t *, int, int);
	void (*set_position)(app_window_t *, int, int);
	void *(*get_handle)(app_window_t *);
	int (*get_width)(app_window_t *);
	int (*get_height)(app_window_t *);
	void (*set_min_width)(app_window_t *, int);
	void (*set_min_height)(app_window_t *, int);
	void (*set_max_width)(app_window_t *, int);
	void (*set_max_height)(app_window_t *, int);
	app_window_paint_t *(*begin_paint)(app_window_t *, pd_rect_t *);
	void (*end_paint)(app_window_t *, app_window_paint_t *);
	void (*present)(app_window_t *);
} app_window_driver_t;

typedef struct app_driver_t {
	int (*init)(const wchar_t *);
	int (*destroy)(void);
	int (*process_events)(app_process_events_option_t option);
	int (*on_event)(int type, app_native_event_handler_t handler,
			void *data);
	int (*off_event)(int type, app_native_event_handler_t handler);
	int (*get_screen_width)(void);
	int (*get_screen_height)(void);
	app_window_t *(*create_window)(const wchar_t *title, int x, int y,
				       int width, int height,
				       app_window_t *parent);
	app_window_t *(*get_window)(void *handle);
	void (*present)(void);
	void (*exit)(int exit_code);
} app_driver_t;

// Window

#define APP_WINDOW_CENTER_X ((int)0xe0000000)
#define APP_WINDOW_CENTER_Y ((int)0xe0000000)
#define APP_WINDOW_DEFAULT_X ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_Y ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_WIDTH 800
#define APP_WINDOW_DEFAULT_HEIGHT 600

#endif

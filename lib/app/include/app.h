#ifndef LIBAPP_INCLUDE_APP_H
#define LIBAPP_INCLUDE_APP_H

#include <LCUI.h>

typedef enum app_event_type_t {
	APP_EVENT_NONE,
	APP_EVENT_IDLE,
	APP_EVENT_KEYDOWN,
	APP_EVENT_KEYPRESS,
	APP_EVENT_KEYUP,
	APP_EVENT_MOUSE,
	APP_EVENT_MOUSEMOVE,
	APP_EVENT_MOUSEDOWN,
	APP_EVENT_MOUSEUP,
	APP_EVENT_MOUSEWHEEL,
	APP_EVENT_COMPOSITION,
	APP_EVENT_TOUCH,
	APP_EVENT_TOUCHMOVE,
	APP_EVENT_TOUCHDOWN,
	APP_EVENT_TOUCHUP,
	APP_EVENT_PAINT,
	APP_EVENT_QUIT,
	LCUI_USER = 100
} app_event_type_t;

typedef struct app_event_t app_event_t;

typedef void(*app_event_handler_t)(app_event_t*, void*);

typedef struct app_event_listener_t {
	app_event_type_t type;
	app_event_handler_t handler;
	void *data;
	void (*destroy_data)(void*);
};


/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/keyCode
 */
typedef enum keycode_t {
	KEY_BACKSPACE = 8,
	KEY_TAB = 9,
	KEY_ENTER = 13,
	KEY_SHIFT = 16,
	KEY_CONTROL = 17,
	KEY_ALT	= 18,
	KEY_CAPITAL = 20,
	KEY_ESCAPE = 27,
	KEY_SPACE = ' ',
	KEY_PAGEUP = 33,
	KEY_PAGEDOWN = 34,
	KEY_END	 = 35,
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
} app_keycode_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent
 */
typedef struct app_keyboard_event_t {
	keycode_t code;
	LCUI_BOOL alt_key;
	LCUI_BOOL ctrl_key;
	LCUI_BOOL shift_key;
	LCUI_BOOL meta_key;
	LCUI_BOOL is_composing;
} app_keyboard_event_t;

typedef struct touch_point_t {
	int x;
	int y;
	int id;
	int state;
	LCUI_BOOL is_primary;
} touch_point_t;

typedef struct app_touch_event_t {
	unsigned n_points;
	touch_point_t *points;
} app_touch_event_t;

typedef struct app_paint_event_t {
	LCUI_Rect rect;
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

struct app_event_t {
	app_event_type_t type;
	app_event_handler_t handler;
	void *data;

	union {
		app_mouse_event_t mouse;
		app_wheel_event_t wheel;
		app_textinput_event_t composition;
		app_keyboard_event_t key;
		app_touch_event_t touch;
		app_paint_event_t paint;
	};
	LinkedListNode node;
};

#endif

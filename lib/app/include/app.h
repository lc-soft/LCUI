#ifndef LIBAPP_INCLUDE_APP_H
#define LIBAPP_INCLUDE_APP_H

#include <LCUI.h>

#ifdef _WIN32
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define APP_PLATFORM_UWP
#else
#define APP_PLATFORM_WIN_DESKTUP
#endif
#else
#undef linux
#define APP_PLATFORM_LINUX
#endif

typedef LCUI_PaintContextRec app_window_paint_t;

typedef enum app_event_type_t {
	APP_EVENT_NONE,
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
	APP_EVENT_SIZE,
	APP_EVENT_MINMAXINFO,
	APP_EVENT_PAINT,
	APP_EVENT_CLOSE,
	APP_EVENT_QUIT,
	LCUI_USER = 100
} app_event_type_t;

#define APP_EVENT_TEXTINPUT APP_EVENT_COMPOSITION

typedef struct app_event_t app_event_t;
typedef struct app_window_t app_window_t;

typedef void (*app_event_handler_t)(app_event_t *, void *);

typedef struct app_event_listener_t {
	app_event_type_t type;
	app_event_handler_t handler;
	void *data;
} app_event_listener_t;

/**
 * @see https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/keyCode
 */
typedef enum keycode_t {
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
} keycode_t;

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
	};
};

typedef void app_native_event_t;
typedef void (*app_native_event_handler_t)(app_native_event_t*, void *);

typedef struct app_native_event_listener_t {
	int type;
	app_native_event_handler_t handler;
	void *data;
} app_native_event_listener_t;

// App events

void app_init_events(void);
void app_destroy_events(void);
int app_post_event(app_event_t *e);
int app_poll_event(app_event_t *e);

// Step timer

typedef struct step_timer_t {
	// Source timing data.
	uint64_t frequency;
	uint64_t last_time;
	uint64_t max_delta;

	// Derived timing data.
	uint64_t elapsed_time;
	uint64_t total_time;
	uint64_t left_over_time;

	// Members for tracking the framerate.
	uint32_t frame_count;
	uint32_t frame_per_second;
	uint32_t frames_this_second;
	uint64_t second_counter;

	// Members for configuring fixed timestep mode.
	LCUI_BOOL is_fixed_time_step;
	float target_elapsed_time;
} step_timer_t;

typedef void (*step_timer_handler_t)(step_timer_t *timer, void *data);

void step_timer_init(step_timer_t *timer);

// Update timer state, calling the specified Update function the appropriate
// number of times.
void step_timer_tick(step_timer_t *timer, step_timer_handler_t *handler,
		     void *data);

// Window

#define APP_WINDOW_CENTER_X ((int)0xe0000000)
#define APP_WINDOW_CENTER_Y ((int)0xe0000000)
#define APP_WINDOW_DEFAULT_X ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_Y ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_WIDTH 800
#define APP_WINDOW_DEFAULT_HEIGHT 600

int app_get_screen_width(void);
int app_get_screen_height(void);
void *app_window_get_handle(app_window_t *wnd);

app_window_t *app_get_window_by_handle(void *handle);
app_window_t *app_window_create(const wchar_t *title, int x, int y, int width,
				int height, app_window_t *parent);

void app_window_close(app_window_t *wnd);
void app_window_destroy(app_window_t *wnd);
void app_window_set_position(app_window_t *wnd, int x, int y);
void app_window_set_framebuffer_size(app_window_t *wnd, int width, int height);
void app_window_set_size(app_window_t *wnd, int width, int height);
void app_window_show(app_window_t *wnd);
void app_window_hide(app_window_t *wnd);
void app_window_set_title(app_window_t *wnd, const wchar_t *title);
int app_window_get_width(app_window_t *wnd);
int app_window_get_height(app_window_t *wnd);
void app_window_set_min_width(app_window_t *wnd, int min_width);
void app_window_set_min_height(app_window_t *wnd, int min_height);
void app_window_set_max_width(app_window_t *wnd, int max_width);
void app_window_set_max_height(app_window_t *wnd, int max_height);
app_window_paint_t *app_window_begin_paint(app_window_t *wnd, LCUI_Rect *rect);
void app_window_end_paint(app_window_t *wnd, app_window_paint_t *paint);
void app_window_present(app_window_t *wnd);


// Native events

int app_add_native_event_listener(int event_type, app_event_handler_t handler,
				   void *data);
int app_remove_native_event_listener(int event_type,
				      app_event_handler_t handler);

INLINE int app_on_native_event(int event_type, app_event_handler_t handler,
			   void *data)
{
	return app_add_native_event_listener(event_type, handler, data);
}

INLINE int app_off_native_event(int event_type, app_event_handler_t handler)
{
	return app_remove_native_event_listener(event_type, handler);
}


// Events

int app_touch_event_init(app_event_t *e, touch_point_t *points, int n_points);
int app_composition_event_init(app_event_t *e, const wchar_t *text, size_t len);
int app_event_copy(app_event_t *dest, app_event_t *src);
void app_event_destroy(app_event_t *e);
int app_touch_event_init(app_event_t *e, touch_point_t *points, int n_points);
void app_init_events(void);
void app_destroy_events(void);
int app_post_event(app_event_t *e);
int app_process_event(app_event_t *e);
int app_poll_event(app_event_t *e);

int app_init(const wchar_t *name);
void app_quit(void);
void app_destroy(void);

#endif

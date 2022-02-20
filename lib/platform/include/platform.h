#ifndef LIBPLATFORM_INCLUDE_PLATFORM_H
#define LIBPLATFORM_INCLUDE_PLATFORM_H

#include <LCUI/def.h>
#include <LCUI/pandagl/def.h>

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
	LCUI_BOOL visible;
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
	uint32_t frames_per_second;
	uint32_t frames_this_second;
	uint64_t second_counter;

	// Members for configuring fixed timestep mode.
	LCUI_BOOL is_fixed_time_step;
	uint64_t target_elapsed_time;
} step_timer_t;

typedef void (*step_timer_handler_t)(step_timer_t *timer, void *data);

void step_timer_init(step_timer_t *timer);

// Update timer state, calling the specified Update function the appropriate
// number of times.
void step_timer_tick(step_timer_t *timer, step_timer_handler_t handler,
		     void *data);

// Window

#define APP_WINDOW_CENTER_X ((int)0xe0000000)
#define APP_WINDOW_CENTER_Y ((int)0xe0000000)
#define APP_WINDOW_DEFAULT_X ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_Y ((int)0xf0000000)
#define APP_WINDOW_DEFAULT_WIDTH 800
#define APP_WINDOW_DEFAULT_HEIGHT 600

LCUI_API int app_get_screen_width(void);
LCUI_API int app_get_screen_height(void);
LCUI_API void *app_window_get_handle(app_window_t *wnd);

LCUI_API app_window_t *app_get_window_by_handle(void *handle);
LCUI_API app_window_t *app_window_create(const wchar_t *title, int x, int y,
					 int width, int height,
					 app_window_t *parent);

LCUI_API void app_window_close(app_window_t *wnd);
LCUI_API void app_window_destroy(app_window_t *wnd);
LCUI_API void app_window_set_position(app_window_t *wnd, int x, int y);
LCUI_API void app_window_set_framebuffer_size(app_window_t *wnd, int width,
					      int height);
LCUI_API void app_window_set_size(app_window_t *wnd, int width, int height);
LCUI_API void app_window_show(app_window_t *wnd);
LCUI_API void app_window_hide(app_window_t *wnd);
LCUI_API void app_window_activate(app_window_t *wnd);
LCUI_API void app_window_set_title(app_window_t *wnd, const wchar_t *title);
LCUI_API int app_window_get_width(app_window_t *wnd);
LCUI_API int app_window_get_height(app_window_t *wnd);
LCUI_API void app_window_set_min_width(app_window_t *wnd, int min_width);
LCUI_API void app_window_set_min_height(app_window_t *wnd, int min_height);
LCUI_API void app_window_set_max_width(app_window_t *wnd, int max_width);
LCUI_API void app_window_set_max_height(app_window_t *wnd, int max_height);
LCUI_API app_window_paint_t *app_window_begin_paint(app_window_t *wnd,
						    pd_rect_t *rect);
LCUI_API void app_window_end_paint(app_window_t *wnd,
				   app_window_paint_t *paint);
LCUI_API void app_window_present(app_window_t *wnd);

// Native events

LCUI_API int app_process_native_events(app_process_events_option_t option);

LCUI_API int app_add_native_event_listener(int event_type,
					   app_native_event_handler_t handler,
					   void *data);
LCUI_API int app_remove_native_event_listener(
    int event_type, app_native_event_handler_t handler);

INLINE int app_on_native_event(int event_type,
			       app_native_event_handler_t handler, void *data)
{
	return app_add_native_event_listener(event_type, handler, data);
}

INLINE int app_off_native_event(int event_type,
				app_native_event_handler_t handler)
{
	return app_remove_native_event_listener(event_type, handler);
}

// Events

LCUI_API int app_touch_event_init(app_event_t *e, touch_point_t *points,
				  int n_points);
LCUI_API int app_composition_event_init(app_event_t *e, const wchar_t *text,
					size_t len);
LCUI_API int app_event_copy(app_event_t *dest, app_event_t *src);
LCUI_API void app_event_destroy(app_event_t *e);
LCUI_API int app_touch_event_init(app_event_t *e, touch_point_t *points,
				  int n_points);
LCUI_API void app_init_events(void);
LCUI_API void app_destroy_events(void);
LCUI_API int app_post_event(app_event_t *e);
LCUI_API int app_process_event(app_event_t *e);

LCUI_API int app_add_event_listener(int event_type, app_event_handler_t handler,
				    void *data);
LCUI_API int app_remove_event_listener(int event_type,
				       app_event_handler_t handler);

INLINE int app_on_event(int event_type, app_event_handler_t handler, void *data)
{
	return app_add_event_listener(event_type, handler, data);
}

INLINE int app_off_event(int event_type, app_event_handler_t handler)
{
	return app_remove_event_listener(event_type, handler);
}

LCUI_API void app_set_event_dispatcher(app_event_dispatcher_t dispatcher);
LCUI_API void app_init_events(void);
LCUI_API void app_destroy_events(void);
LCUI_API int app_post_event(app_event_t *e);
LCUI_API void app_process_events(void);

// Engine

LCUI_API int app_init_engine(const wchar_t *name);
LCUI_API int app_destroy_engine(void);

// Base

LCUI_API void app_set_instance(void *instance);
LCUI_API app_id_t app_get_id(void);
LCUI_API void app_present(void);
LCUI_API int app_init(const wchar_t *name);
LCUI_API void app_exit(int exit_code);
LCUI_API void app_destroy(void);

// Input method engine

typedef struct ime_handler_t {
	LCUI_BOOL (*prockey)(int, LCUI_BOOL);
	void (*totext)(int);
	LCUI_BOOL (*open)(void);
	LCUI_BOOL (*close)(void);
	void (*setcaret)(int, int);
} ime_handler_t;

LCUI_API int ime_add(const char *ime_name, ime_handler_t *handler);

/** 选定输入法 */
LCUI_API LCUI_BOOL ime_select(int ime_id);

LCUI_API LCUI_BOOL ime_select_by_name(const char *name);

/** 检测键值是否为字符键值 */
LCUI_API LCUI_BOOL ime_check_char_key(int key);

/** 切换至下一个输入法 */
LCUI_API void ime_switch(void);

/** 检测输入法是否要处理按键事件 */
LCUI_API LCUI_BOOL ime_process_key(app_event_t *e);

/** 提交输入法输入的内容至目标 */
LCUI_API int ime_commit(const wchar_t *str, size_t len);

LCUI_API void ime_set_caret(int x, int y);

/* 初始化LCUI输入法模块 */
LCUI_API void app_init_ime(void);

/* 停用LCUI输入法模块 */
LCUI_API void app_destroy_ime(void);

/**
 * Clipboard
 * References: https://docs.gtk.org/gtk3/class.Clipboard.html
 */

typedef struct clipboard_t {
	pd_canvas_t *image;
	wchar_t *text;
	size_t len;
} clipboard_t;

typedef void (*clipboard_callback_t)(clipboard_t *, void *);

LCUI_API int clipboard_request_text(clipboard_callback_t action, void *arg);
LCUI_API int clipboard_set_text(const wchar_t *text, size_t len);
LCUI_API void clipboard_init(void);
LCUI_API void clipboard_destroy(void);

LCUI_API int open_uri(const char *uri);

#endif

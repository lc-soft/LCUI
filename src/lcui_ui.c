#include <math.h>
#include <yutil.h>
#include <pandagl.h>
#include <timer.h>
#include <ui.h>
#include <ui_server.h>
#include <ui_widgets.h>
#include <LCUI/app.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

static struct lcui_ui_t {
	int image_loading_timer;
	lcui_display_mode_t mode;
	ui_mutation_observer_t *observer;

	/** list_t<app_window_t> */
	list_t windows;
} lcui_ui;

static void lcui_dispatch_ui_mouse_event(ui_event_type_t type,
					 app_event_t *app_evt)
{
	ui_event_t e = { 0 };
	float scale = ui_metrics.scale;

	e.type = type;
	e.mouse.y = (float)round(app_evt->mouse.y / scale);
	e.mouse.x = (float)round(app_evt->mouse.x / scale);
	ui_dispatch_event(&e);
}

static void lcui_dispatch_ui_keyboard_event(ui_event_type_t type,
					    app_event_t *app_evt)
{
	ui_event_t e = { 0 };

	e.type = type;
	e.key.code = app_evt->key.code;
	e.key.is_composing = app_evt->key.is_composing;
	e.key.alt_key = app_evt->key.alt_key;
	e.key.shift_key = app_evt->key.shift_key;
	e.key.ctrl_key = app_evt->key.ctrl_key;
	e.key.meta_key = app_evt->key.meta_key;
	ui_dispatch_event(&e);
}

static void lcui_dispatch_ui_touch_event(app_touch_event_t *touch)
{
	size_t i;
	float scale;
	ui_event_t e = { 0 };

	scale = ui_metrics.scale;
	e.type = UI_EVENT_TOUCH;
	e.touch.n_points = touch->n_points;
	e.touch.points = malloc(sizeof(ui_touch_point_t) * e.touch.n_points);
	for (i = 0; i < e.touch.n_points; ++i) {
		switch (touch->points[i].state) {
		case APP_EVENT_TOUCHDOWN:
			e.touch.points[i].state = UI_EVENT_TOUCHDOWN;
			break;
		case APP_EVENT_TOUCHUP:
			e.touch.points[i].state = UI_EVENT_TOUCHUP;
			break;
		case APP_EVENT_TOUCHMOVE:
			e.touch.points[i].state = UI_EVENT_TOUCHMOVE;
			break;
		default:
			break;
		}
		e.touch.points[i].x = (float)round(touch->points[i].x / scale);
		e.touch.points[i].y = (float)round(touch->points[i].y / scale);
	}
	ui_dispatch_event(&e);
	ui_event_destroy(&e);
}

static void lcui_dispatch_ui_textinput_event(app_event_t *app_evt)
{
	ui_event_t e = { 0 };

	e.type = UI_EVENT_TEXTINPUT;
	e.text.length = app_evt->text.length;
	e.text.text = wcsdup2(app_evt->text.text);
	ui_dispatch_event(&e);
	ui_event_destroy(&e);
}

static void lcui_dispatch_ui_wheel_event(app_wheel_event_t *wheel)
{
	ui_event_t e = { 0 };

	// TODO:
	e.type = UI_EVENT_WHEEL;
	e.wheel.delta_mode = UI_WHEEL_DELTA_PIXEL;
	e.wheel.delta_y = wheel->delta_y;
	ui_dispatch_event(&e);
}

void lcui_dispatch_ui_event(app_event_t *app_event)
{
	switch (app_event->type) {
	case APP_EVENT_KEYDOWN:
		lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYDOWN, app_event);
		break;
	case APP_EVENT_KEYUP:
		lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYUP, app_event);
		break;
	case APP_EVENT_KEYPRESS:
		lcui_dispatch_ui_keyboard_event(UI_EVENT_KEYPRESS, app_event);
		break;
	case APP_EVENT_MOUSEDOWN:
		lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEDOWN, app_event);
		break;
	case APP_EVENT_MOUSEUP:
		lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEUP, app_event);
		break;
	case APP_EVENT_MOUSEMOVE:
		lcui_dispatch_ui_mouse_event(UI_EVENT_MOUSEMOVE, app_event);
		break;
	case APP_EVENT_TOUCH:
		lcui_dispatch_ui_touch_event(&app_event->touch);
		break;
	case APP_EVENT_WHEEL:
		lcui_dispatch_ui_wheel_event(&app_event->wheel);
		break;
	case APP_EVENT_COMPOSITION:
		lcui_dispatch_ui_textinput_event(app_event);
		break;
	default:
		break;
	}
}

size_t lcui_render_ui(void)
{
	return ui_server_render();
}

void lcui_update_ui(void)
{
	ui_process_image_events();
	ui_process_events();
	ui_update();
}

static void lcui_process_ui_mutation(ui_mutation_record_t *mutation)
{
	list_node_t *node;

	if (mutation->type != UI_MUTATION_RECORD_TYPE_CHILD_LIST ||
	    lcui_ui.mode != LCUI_DISPLAY_MODE_SEAMLESS) {
		return;
	}
	for (list_each(node, &mutation->removed_widgets)) {
		ui_server_disconnect(node->data, NULL);
	}
	for (list_each(node, &mutation->added_widgets)) {
		ui_server_connect(node->data, NULL);
	}
}

static void lcui_on_ui_mutation(ui_mutation_list_t *mutation_list,
				ui_mutation_observer_t *observer, void *arg)
{
	list_node_t *node;

	for (list_each(node, mutation_list)) {
		lcui_process_ui_mutation(node->data);
	}
}

static void lcui_on_window_destroy(app_event_t *e, void *arg)
{
	list_node_t *node;

	for (list_each(node, &lcui_ui.windows)) {
		if (node->data == e->window) {
			list_delete_node(&lcui_ui.windows, node);
			break;
		}
	}
}

static void lcui_close_window(void *arg)
{
	app_window_close(arg);
}

void lcui_set_ui_display_mode(lcui_display_mode_t mode)
{
	float scale;
	app_window_t *wnd;
	list_node_t *node;
	ui_mutation_observer_init_t options = { 0 };

	if (mode == LCUI_DISPLAY_MODE_DEFAULT) {
		mode = LCUI_DISPLAY_MODE_WINDOWED;
	}
	if (lcui_ui.observer) {
		ui_mutation_observer_disconnect(lcui_ui.observer);
		ui_mutation_observer_destroy(lcui_ui.observer);
		lcui_ui.observer = NULL;
	}
	list_destroy(&lcui_ui.windows, lcui_close_window);
	switch (lcui_ui.mode) {
	case LCUI_DISPLAY_MODE_FULLSCREEN:
	case LCUI_DISPLAY_MODE_WINDOWED:
		ui_server_disconnect(ui_root(), NULL);
		break;
	case LCUI_DISPLAY_MODE_SEAMLESS:
		for (list_each(node, &ui_root()->children)) {
			ui_server_disconnect(node->data, NULL);
		}
	default:
		break;
	}
	switch (mode) {
	case LCUI_DISPLAY_MODE_FULLSCREEN:
		scale = ui_metrics.scale;
		// TODO: set window fullscreen style
		wnd = app_window_create(NULL, 0, 0, 0, 0, NULL);
		ui_widget_resize(ui_root(), app_get_screen_width() / scale,
				 app_get_screen_height() / scale);
		list_append(&lcui_ui.windows, wnd);
		ui_server_connect(ui_root(), wnd);
		break;
	case LCUI_DISPLAY_MODE_SEAMLESS:
		options.child_list = TRUE;
		lcui_ui.observer =
		    ui_mutation_observer_create(lcui_on_ui_mutation, NULL);
		ui_mutation_observer_observe(lcui_ui.observer, ui_root(),
					     options);
		for (list_each(node, &ui_root()->children)) {
			wnd = app_window_create(NULL, 0, 0, 0, 0, NULL);
			ui_server_connect(node->data, wnd);
		}
		break;
	case LCUI_DISPLAY_MODE_WINDOWED:
	default:
		wnd = app_window_create(NULL, 0, 0, 0, 0, NULL);
		ui_server_connect(ui_root(), wnd);
		break;
	}
	lcui_ui.mode = mode;
}

void lcui_init_ui_preset_widgets(void)
{
	ui_register_textview();
	ui_register_canvas();
	ui_register_anchor();
	ui_register_button();
	ui_register_scrollbar();
	ui_register_textcaret();
	ui_register_textedit();
}

void lcui_destroy_ui_preset_widgets(void)
{
	ui_unregister_textview();
	ui_unregister_anchor();
}

#ifdef LIBPLAT_WIN32
static void lcui_load_fonts_for_windows(void)
{
	size_t i;
	int *ids = NULL;
	const char *names[] = { "Consola", "Simsun", "Microsoft YaHei", NULL };
	const char *fonts[] = { "C:/Windows/Fonts/consola.ttf",
				"C:/Windows/Fonts/simsun.ttc",
				"C:/Windows/Fonts/msyh.ttf",
				"C:/Windows/Fonts/msyh.ttc" };

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		pd_font_library_load_file(fonts[i]);
	}
	i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL, names);
	if (i > 0) {
		pd_font_library_set_default_font(ids[i - 1]);
	}
	free(ids);
}

#else

#ifdef HAVE_FONTCONFIG

static void lcui_load_fonts_by_font_config(void)
{
	size_t i;
	char *path;
	int *ids = NULL;
	const char *names[] = { "Noto Sans CJK", "Ubuntu",
				"WenQuanYi Micro Hei", NULL };
	const char *fonts[] = { "Ubuntu", "Noto Sans CJK SC",
				"WenQuanYi Micro Hei" };

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		path = pd_font_library_get_font_path(fonts[i]);
		pd_font_library_load_file(path);
		free(path);
	}
	i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL, names);
	if (i > 0) {
		pd_font_library_set_default_font(ids[i - 1]);
	}
	free(ids);
}

#else

static void lcui_load_fonts_for_linux(void)
{
	size_t i;
	int *ids = NULL;
	const char *names[] = { "Noto Sans CJK SC", "Ubuntu", "Ubuntu Mono",
				"WenQuanYi Micro Hei", NULL };
	const char *fonts[] = {
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-C.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-LI.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-L.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-MI.ttf",
		"/usr/share/fonts/truetype/ubuntu/UbuntuMono-BI.ttf",
		"/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf",
		"/usr/share/fonts/truetype/ubuntu/UbuntuMono-RI.ttf",
		"/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-RI.ttf",
		"/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-RI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-B.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-M.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-MI.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-L.ttf",
		"/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-LI.ttf",
		"/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
		"/usr/share/fonts/opentype/noto/NotoSansCJK.ttc",
		"/usr/share/fonts/truetype/wqy/wqy-microhei.ttc"
	};

	for (i = 0; i < sizeof(fonts) / sizeof(char *); ++i) {
		pd_font_library_load_file(fonts[i]);
	}
	i = pd_font_library_query(&ids, PD_FONT_STYLE_NORMAL, PD_FONT_WEIGHT_NORMAL, names);
	if (i > 0) {
		pd_font_library_set_default_font(ids[i - 1]);
	}
	free(ids);
}
#endif

#endif

static void lcui_load_default_fonts(void)
{
#ifdef LIBPLAT_WIN32
	lcui_load_fonts_for_windows();
#elif defined(HAVE_FONTCONFIG)
	logger_debug("[font] fontconfig enabled\n");
	lcui_load_fonts_by_font_config();
#else
	lcui_load_fonts_for_linux();
#endif
}

static void lcui_ui_image_loading_timer(void *arg)
{
	ui_load_images();
}

void lcui_init_ui(void)
{
	ui_init();
	ui_server_init();
	ui_widget_resize(ui_root(), DEFAULT_WINDOW_WIDTH,
			 DEFAULT_WINDOW_HEIGHT);
	list_create(&lcui_ui.windows);
	lcui_set_ui_display_mode(LCUI_DISPLAY_MODE_DEFAULT);
	lcui_init_ui_preset_widgets();
	lcui_load_default_fonts();
	app_on_event(APP_EVENT_CLOSE, lcui_on_window_destroy, NULL);
	lcui_ui.image_loading_timer = lcui_set_interval(100, lcui_ui_image_loading_timer, NULL);
}

void lcui_destroy_ui(void)
{
	lcui_destroy_timer(lcui_ui.image_loading_timer);
	lcui_destroy_ui_preset_widgets();
	app_off_event(APP_EVENT_CLOSE, lcui_on_window_destroy);
	list_destroy(&lcui_ui.windows, lcui_close_window);
	if (lcui_ui.observer) {
		ui_mutation_observer_disconnect(lcui_ui.observer);
		ui_mutation_observer_destroy(lcui_ui.observer);
		lcui_ui.observer = NULL;
	}
	ui_server_destroy();
	ui_destroy();
}

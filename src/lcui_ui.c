#include <math.h>
#include <LCUI/util.h>
#include <LCUI/ui/server.h>
#include <LCUI/app.h>
#include <LCUI/ui/widgets/textview.h>
#include <LCUI/ui/widgets/button.h>
#include <LCUI/ui/widgets/anchor.h>
#include <LCUI/ui/widgets/canvas.h>
#include <LCUI/ui/widgets/scrollbar.h>
#include <LCUI/ui/widgets/textedit.h>
#include <LCUI/ui/widgets/textcaret.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

static struct lcui_ui_t {
	lcui_display_mode_t mode;
	ui_mutation_observer_t *observer;

	/** list_t<app_window_t> */
	list_t windows;
} lcui_ui;

static void lcui_dispatch_ui_mouse_event(ui_event_type_t type,
					 app_event_t *app_evt)
{
	ui_event_t e = { 0 };
	float scale = ui_get_scale();

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

	scale = ui_get_scale();
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
	if (mode == lcui_ui.mode) {
		return;
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
		scale = ui_get_scale();
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

void lcui_init_ui(void)
{
	ui_init();
	ui_server_init();
	ui_widget_resize(ui_root(), DEFAULT_WINDOW_WIDTH,
			 DEFAULT_WINDOW_HEIGHT);
	list_create(&lcui_ui.windows);
	lcui_set_ui_display_mode(LCUI_DISPLAY_MODE_DEFAULT);
	lcui_init_ui_preset_widgets();
	app_on_event(APP_EVENT_CLOSE, lcui_on_window_destroy, NULL);
}

void lcui_destroy_ui(void)
{
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

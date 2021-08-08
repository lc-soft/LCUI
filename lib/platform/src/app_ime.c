#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <yutil.h>
#include "platform.h"
#include "internal.h"

typedef struct ime_t_ {
	int id;
	char *name;
	ime_handler_t handler;
	list_node_t node;
} ime_t;

static struct app_ime_t {
	int id_count;
	list_t list;

	ime_t *ime;
	LCUI_BOOL enable_caps_lock;
	LCUI_BOOL active;
} app_ime;

static ime_t *ime_get(int ime_id)
{
	list_node_t *node;
	for (list_each(node, &app_ime.list)) {
		ime_t *ime = node->data;
		if (ime->id == ime_id) {
			return ime;
		}
	}
	return NULL;
}

static ime_t *ime_get_by_name(const char *name)
{
	list_node_t *node;

	for (list_each(node, &app_ime.list)) {
		ime_t *ime = node->data;
		if (strcmp(ime->name, name) == 0) {
			return ime;
		}
	}
	return NULL;
}

int ime_add(const char *name, ime_handler_t *handler)
{
	size_t len;
	ime_t *ime;

	if (!app_ime.active) {
		return -1;
	}
	if (ime_get_by_name(name)) {
		return -2;
	}
	ime = malloc(sizeof(ime_t));
	if (!ime) {
		return -ENOMEM;
	}
	len = strlen(name) + 1;
	ime->name = malloc(len * sizeof(char));
	if (!ime->name) {
		return -ENOMEM;
	}
	app_ime.id_count += 1;
	ime->id = app_ime.id_count;
	ime->node.data = ime;
	strncpy(ime->name, name, len);
	memcpy(&ime->handler, handler, sizeof(ime_handler_t));
	list_append_node(&app_ime.list, &ime->node);
	return ime->id;
}

static LCUI_BOOL ime_open(ime_t *ime)
{
	if (ime && ime->handler.open) {
		return ime->handler.open();
	}
	return FALSE;
}

static LCUI_BOOL ime_close(ime_t *ime)
{
	if (ime && ime->handler.close) {
		logger_debug("[ime] close engine: %s\n", ime->name);
		return ime->handler.close();
	}
	return FALSE;
}

LCUI_BOOL ime_select(int ime_id)
{
	ime_t *ime = ime_get(ime_id);
	if (ime) {
		ime_close(app_ime.ime);
		logger_debug("[ime] select engine: %s\n", ime->name);
		app_ime.ime = ime;
		ime_open(app_ime.ime);
		return TRUE;
	}
	return FALSE;
}

LCUI_BOOL ime_select_by_name(const char *name)
{
	ime_t *ime = ime_get_by_name(name);
	if (ime) {
		ime_close(app_ime.ime);
		app_ime.ime = ime;
		ime_open(app_ime.ime);
		return TRUE;
	}
	return FALSE;
}

void ime_switch(void)
{
	ime_t *ime;
	if (app_ime.ime && app_ime.ime->node.next) {
		ime = app_ime.ime->node.next->data;
		ime_close(app_ime.ime);
		app_ime.ime = ime;
		ime_open(app_ime.ime);
	}
}

static void ime_on_destroy(void *arg)
{
	ime_t *ime = arg;
	if (app_ime.ime == ime) {
		app_ime.ime = NULL;
	}
	free(ime->name);
	free(ime);
}

LCUI_BOOL ime_check_char_key(int key)
{
	switch (key) {
	case KEY_TAB:
	case KEY_ENTER:
	case KEY_SPACE:
	case KEY_SEMICOLON:
	case KEY_MINUS:
	case KEY_EQUAL:
	case KEY_COMMA:
	case KEY_PERIOD:
	case KEY_SLASH:
	case KEY_BRACKETLEFT:
	case KEY_BACKSLASH:
	case KEY_BRACKETRIGHT:
	case KEY_APOSTROPHE:
		return TRUE;
	default:
		if (key >= KEY_0 && key <= KEY_Z) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

static void ime_to_text(app_event_t *e)
{
	app_ime.ime->handler.totext(e->key.code);
}

LCUI_BOOL ime_process_key(app_event_t *e)
{
	LCUI_BOOL is_presed = FALSE;

	/* 根据事件类型判定按键状态 */
	if (e->type == APP_EVENT_KEYUP) {
		/* 如果是 caps lock 按键被释放 */
		if (e->key.code == KEY_CAPITAL) {
			app_ime.enable_caps_lock = !app_ime.enable_caps_lock;
			return FALSE;
		}
	} else {
		is_presed = TRUE;
		/* 如果按下的是 shift 键，但没释放，则直接退出 */
		if (e->key.code == KEY_SHIFT) {
			return FALSE;
		}
	}
	if (app_ime.ime && app_ime.ime->handler.prockey) {
		return app_ime.ime->handler.prockey(e->key.code, is_presed);
	}
	return FALSE;
}

int ime_commit(const wchar_t *str, size_t len)
{
	app_event_t e;

	app_composition_event_init(&e, str, len);
	app_process_event(&e);
	app_event_destroy(&e);
	return 0;
}

static void ime_on_keydown(app_event_t *e, void *arg)
{
	if (ime_process_key(e)) {
		ime_to_text(e);
	}
}

void ime_set_caret(int x, int y)
{
	if (app_ime.ime && app_ime.ime->handler.setcaret) {
		app_ime.ime->handler.setcaret(x, y);
	}
}

void app_init_ime(void)
{
	list_create(&app_ime.list);
	app_ime.active = TRUE;
	app_on_event(APP_EVENT_KEYDOWN, ime_on_keydown, NULL);
#ifdef LCUI_PLATFORM_UWP
	return;
#else
#ifdef LCUI_PLATFORM_WIN_DESKTOP
	ime_select(ime_add_win32());
#else
	ime_select(ime_add_linux());
#endif
#endif
}

void app_destroy_ime(void)
{
	app_ime.active = FALSE;
	ime_close(app_ime.ime);
	list_destroy_without_node(&app_ime.list, ime_on_destroy);
}

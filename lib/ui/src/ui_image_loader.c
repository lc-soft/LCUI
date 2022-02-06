#include <stdlib.h>
#include <assert.h>
#include <LCUI.h>
#include <LCUI/image.h>
#include <LCUI/worker.h>
#include "../include/ui.h"
#include "internal.h"

typedef struct ui_image_event_listener_t {
	list_node_t node;
	void *data;
	LCUI_BOOL is_fresh;
	ui_image_event_handler_t handler;
} ui_image_event_listener_t;

struct ui_image_t {
	pd_canvas_t data;
	LCUI_BOOL loaded;
	char *path;
	size_t refs_count;
	/** list_t<ui_image_event_listener_t> */
	list_t listeners;
};

typedef struct ui_image_event_t {
	list_node_t node;
	void *data;
	ui_image_t *image;
	ui_image_event_handler_t handler;
} ui_image_event_t;

typedef struct ui_image_loader_t {
	/** dict_t<string, ui_image_t> */
	dict_t *dict;
	dict_type_t dict_type;

	/** list_t<ui_image_event_t> */
	list_t events;
	LCUI_Worker worker;
} ui_image_loader_t;

static ui_image_loader_t ui_image_loader;

static void ui_image_destroy(void *privdata, void *data)
{
	ui_image_t *image = data;
	pd_canvas_destroy(&image->data);
	free(image->path);
	image->refs_count = 0;
	list_destroy_without_node(&image->listeners, free);
	image->path = NULL;
	free(image);
}

static void ui_image_loader_load(void *arg1, void *arg2)
{
	list_node_t *node;
	ui_image_t *image = arg1;
	ui_image_event_listener_t *listener;
	ui_image_event_t *e;

	if (!image->loaded) {
		if (LCUI_ReadImageFile(image->path, &image->data) != 0) {
			return;
		}
		image->loaded = TRUE;
	}
	for (list_each(node, &image->listeners)) {
		listener = node->data;
		if (!listener->is_fresh) {
			continue;
		}
		e = malloc(sizeof(ui_image_event_t));
		if (!e) {
			return;
		}
		e->image = image;
		e->data = listener->data;
		e->handler = listener->handler;
		list_append_node(&ui_image_loader.events, &e->node);
		listener->is_fresh = FALSE;
	}
}

static void ui_post_image_task(ui_image_t *image)
{
	LCUI_TaskRec task = { 0 };

	task.arg[0] = image;
	task.func = ui_image_loader_load;
	LCUIWorker_PostTask(ui_image_loader.worker, &task);
}

ui_image_t *ui_load_image(const char *path)
{
	ui_image_t *image;

	image = dict_fetch_value(ui_image_loader.dict, path);
	if (image) {
		return image;
	}
	image = malloc(sizeof(ui_image_t));
	if (!image) {
		return NULL;
	}
	image->loaded = FALSE;
	image->refs_count = 0;
	image->path = strdup2(path);
	pd_canvas_init(&image->data);
	list_create(&image->listeners);
	ui_post_image_task(image);
	return image;
}

pd_canvas_t *ui_image_get_data(ui_image_t *image)
{
	return &image->data;
}

const char *ui_image_get_path(ui_image_t *image)
{
	return image->path;
}

void ui_image_add_ref(ui_image_t *image)
{
	image->refs_count++;
}

void ui_image_remove_ref(ui_image_t *image)
{
	assert(image->refs_count > 0);
	image->refs_count--;
}

int ui_image_add_event_listener(ui_image_t *image, ui_image_event_handler_t handler, void *data)
{
	ui_image_event_listener_t *listener;

	listener = malloc(sizeof(ui_image_event_listener_t));
	if (!listener) {
		return -1;
	}
	listener->handler = handler;
	listener->data = data;
	listener->is_fresh = TRUE;
	list_append_node(&image->listeners, &listener->node);
	return 0;
}

int ui_image_remove_event_listener(ui_image_t *image, ui_image_event_handler_t handler, void *data)
{
	list_node_t *node;
	ui_image_event_listener_t *listener;

	for (list_each(node, &image->listeners)) {
		listener = node->data;
		if (listener->handler == handler && listener->data == data) {
			list_unlink(&image->listeners, node);
			free(listener);
			return 0;
		}
	}
	return -1;
}

void ui_process_image_events(void)
{
	list_node_t *node;
	list_t events = ui_image_loader.events;

	list_create(&events);
	list_concat(&events, &ui_image_loader.events);
	for (list_each(node, &ui_image_loader.events)) {
		ui_image_event_t *e = node->data;
		e->handler(e->image, e->data);
	}
	list_destroy(&events, NULL);
}

void ui_init_image_loader(void)
{
	ui_image_loader.dict_type.val_destructor = ui_image_destroy;
	ui_image_loader.dict = dict_create(&ui_image_loader.dict_type, NULL);
	ui_image_loader.worker = LCUIWorker_New();
	LCUIWorker_RunAsync(ui_image_loader.worker);
}

void ui_destroy_image_loader(void)
{
	LCUIWorker_Destroy(ui_image_loader.worker);
	dict_destroy(ui_image_loader.dict);
	ui_image_loader.dict = NULL;
}

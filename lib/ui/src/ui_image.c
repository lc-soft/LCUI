#include <stdlib.h>
#include <assert.h>
#include <yutil.h>
#include <pandagl.h>
#include <ui/image.h>

typedef struct ui_image_event_listener_t {
	list_node_t node;
	void *data;
	bool is_fresh;
	ui_image_event_handler_t handler;
} ui_image_event_listener_t;

struct ui_image_t {
	pd_canvas_t data;
	bool loaded;
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

	/** list_t<ui_image_t> */
	list_t images;
} ui_image_loader_t;

static ui_image_loader_t ui_image_loader;

static void ui_image_force_destroy(void *privdata, void *data)
{
	ui_image_t *image = data;
	pd_canvas_destroy(&image->data);
	free(image->path);
	image->refs_count = 0;
	list_destroy_without_node(&image->listeners, free);
	image->path = NULL;
	free(image);
}

static void ui_image_load(ui_image_t *image)
{
	list_t events;
	list_node_t *node;
	ui_image_event_t *e;
	ui_image_event_listener_t *listener;

	list_create(&events);
	if (!image->loaded) {
		if (pd_read_image_from_file(image->path, &image->data) != 0) {
			return;
		}
		image->loaded = true;
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
		list_append_node(&events, &e->node);
		listener->is_fresh = false;
	}
	list_concat(&ui_image_loader.events, &events);
}

ui_image_t *ui_image_create(const char *path)
{
	ui_image_t *image;

	image = dict_fetch_value(ui_image_loader.dict, path);
	if (image) {
		image->refs_count++;
		return image;
	}
	image = malloc(sizeof(ui_image_t));
	if (!image) {
		return NULL;
	}
	image->loaded = false;
	image->refs_count = 1;
	image->path = strdup2(path);
	pd_canvas_init(&image->data);
	list_create(&image->listeners);
	list_append(&ui_image_loader.images, image);
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

void ui_image_destroy(ui_image_t *image)
{
	assert(image->refs_count > 0);
	image->refs_count--;
}

int ui_image_add_event_listener(ui_image_t *image,
				ui_image_event_handler_t handler, void *data)
{
	ui_image_event_listener_t *listener;

	listener = malloc(sizeof(ui_image_event_listener_t));
	if (!listener) {
		return -1;
	}
	listener->handler = handler;
	listener->data = data;
	listener->is_fresh = true;
	list_append_node(&image->listeners, &listener->node);
	return 0;
}

int ui_image_remove_event_listener(ui_image_t *image,
				   ui_image_event_handler_t handler, void *data)
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

void ui_load_images(void)
{
	list_t list;
	list_node_t *node;

	list_create(&list);
	list_concat(&list, &ui_image_loader.images);
	for (list_each(node, &list)) {
		ui_image_load(node->data);
	}
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
	ui_image_loader.dict_type.val_destructor = ui_image_force_destroy;
	ui_image_loader.dict = dict_create(&ui_image_loader.dict_type, NULL);
	list_create(&ui_image_loader.images);
}

void ui_destroy_image_loader(void)
{
	list_destroy(&ui_image_loader.images, NULL);
	dict_destroy(ui_image_loader.dict);
	ui_image_loader.dict = NULL;
}

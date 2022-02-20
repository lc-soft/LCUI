#include <errno.h>
#include <LCUI/util.h>
#include "internal.h"

static struct app_events_t {
	/** list_t<app_event_t> */
	list_t queue;

	/** list_t<app_listener_t> */
	list_t listeners;

	app_event_dispatcher_t dispatcher;
} app_events;

int app_touch_event_init(app_event_t *e, touch_point_t *points, int n_points)
{
	e->type = APP_EVENT_TOUCH;
	e->touch.n_points = n_points;
	e->touch.points = malloc(sizeof(touch_point_t) * n_points);
	if (!e->touch.points) {
		return -ENOMEM;
	}
	for (n_points -= 1; n_points >= 0; --n_points) {
		e->touch.points[n_points] = points[n_points];
	}
	return 0;
}

int app_composition_event_init(app_event_t *e, const wchar_t *text, size_t len)
{
	if (len == 0) {
		len = wcslen(text);
	}
	e->type = APP_EVENT_COMPOSITION;
	e->text.length = len;
	e->text.text = malloc(sizeof(wchar_t) * (len + 1));
	if (!e->text.text) {
		return -ENOMEM;
	}
	wcscpy(e->text.text, text);
	return 0;
}

int app_event_copy(app_event_t *dest, app_event_t *src)
{
	switch (src->type) {
	case APP_EVENT_TOUCH:
		return app_touch_event_init(dest, src->touch.points,
					    src->touch.n_points);
	case APP_EVENT_TEXTINPUT:
		return app_composition_event_init(dest, src->text.text, 0);
	default:
		*dest = *src;
		break;
	}
	return 0;
}

void app_event_destroy(app_event_t *e)
{
	switch (e->type) {
	case APP_EVENT_TOUCH:
		if (e->touch.points) {
			free(e->touch.points);
		}
		e->touch.points = NULL;
		e->touch.n_points = 0;
		break;
	case APP_EVENT_COMPOSITION:
		if (e->text.text) {
			free(e->text.text);
		}
		e->text.text = NULL;
		e->text.length = 0;
		break;
	default:
		break;
	}
	e->type = APP_EVENT_NONE;
}

int app_post_event(app_event_t *e)
{
	app_event_t *ev;

	ev = malloc(sizeof(app_event_t));
	if (!ev) {
		return -ENOMEM;
	}
	app_event_copy(ev, e);
	list_append(&app_events.queue, ev);
	return 0;
}

int app_add_event_listener(int event_type, app_event_handler_t handler,
			   void *data)
{
	app_event_listener_t *listener;

	listener = malloc(sizeof(app_event_listener_t));
	if (!listener) {
		return -ENOMEM;
	}
	listener->handler = handler;
	listener->data = data;
	listener->type = event_type;
	list_append(&app_events.listeners, listener);
	return 0;
}

int app_remove_event_listener(int event_type, app_event_handler_t handler)
{
	list_node_t *node, *prev;
	app_event_listener_t *listener;

	for (list_each(node, &app_events.listeners)) {
		prev = node->prev;
		listener = node->data;
		if (listener->handler == handler &&
		    listener->type == event_type) {
			list_delete_node(&app_events.listeners, node);
			free(listener);
			node = prev;
			return 0;
		}
	}
	return -1;
}

int app_process_event(app_event_t *e)
{
	int count = 0;
	list_node_t *node;
	app_event_listener_t *listener;

	for (list_each(node, &app_events.listeners)) {
		listener = node->data;
		if (listener->type == e->type) {
			listener->handler(e, listener->data);
			++count;
		}
	}
	if (app_events.dispatcher) {
		app_events.dispatcher(e);
	}
	return count;
}

void app_process_events(void)
{
	app_event_t *e;

	while ((e = list_get(&app_events.queue, 0)) != NULL) {
		list_delete(&app_events.queue, 0);
		app_process_event(e);
		app_event_destroy(e);
		free(e);
	}
}

void app_set_event_dispatcher(app_event_dispatcher_t dispatcher)
{
	app_events.dispatcher = dispatcher;
}

void app_init_events(void)
{
	list_create(&app_events.queue);
}

void app_destroy_events(void)
{
	app_set_event_dispatcher(NULL);
	list_destroy(&app_events.queue, free);
}

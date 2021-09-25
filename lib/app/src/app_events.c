#include <errno.h>
#include <app.h>

static struct app_events_t {
	/** LinkedList<app_event_t> */
	LinkedList queue;

	/** LinkedList<app_listener_t> */
	LinkedList listeners;
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
		return app_touch_event_init(dest, src->touch.points, src->touch.n_points);
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
	}
	e->type = APP_EVENT_NONE;
}

void app_init_events(void)
{
	LinkedList_Init(&app_events.queue);
}

void app_destroy_events(void)
{
	LinkedList_Clear(&app_events.queue, free);
}

int app_post_event(app_event_t *e)
{
	app_event_t *ev;

	ev = malloc(sizeof(app_event_t));
	if (!ev) {
		return -ENOMEM;
	}
	app_event_copy(ev, e);
	LinkedList_Append(e->data, ev);
	return 0;
}

int app_process_event(app_event_t *e)
{
	int count = 0;
	LinkedListNode *node;
	app_event_listener_t *listener;

	for (LinkedList_Each(node, &app_events.listeners)) {
		listener = node->data;
		if (listener->type == e->type) {
			listener->handler(e, listener->data);
			++count;
		}
	}
	return count;
}

int app_poll_event(app_event_t *e)
{
	app_event_t *ev;

	if (app_events.queue.length < 1) {
		return 0;
	}
	ev = LinkedList_Get(&app_events.queue, 0);
	if (ev) {
		*e = *ev;
		LinkedList_Delete(&app_events.queue, 0);
		free(ev);
	}
	return 1;
}

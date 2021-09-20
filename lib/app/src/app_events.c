#include <errno.h>
#include <LCUI.h>
#include "../include/app.h"

static struct app_events_t {
	/** LinkedList<app_event_t> */
	LinkedList queue;
} app_events;

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
	app_event_t *ev = malloc(sizeof(app_event_t));
	if (!ev) {
		return -ENOMEM;
	}
	*ev = *e;
	LinkedList_Append(e->data, ev);
	return 0;
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

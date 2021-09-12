#include <LCUI.h>
#include "../include/app.h"

static struct app_events_t {
	/** LinkedList<app_event_t> */
	LinkedList queue;
} app_events;

void app_init_events(void)
{

}

void app_destroy_events(void)
{

}

void app_post_event(app_event_t *e)
{

}

int app_wait_event(app_event_t *e)
{

}

int app_wait_event_timeout(app_event_t *e, int timeout)
{

}

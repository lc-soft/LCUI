#ifndef LIBPLAT_INCLUDE_PLATFORM_EVENTS_H
#define LIBPLAT_INCLUDE_PLATFORM_EVENTS_H

#include "types.h"
#include "common.h"

LIBPLAT_BEGIN_DECLS

LIBPLAT_PUBLIC int app_process_native_events(app_process_events_option_t option);

LIBPLAT_PUBLIC int app_add_native_event_listener(int event_type,
					   app_native_event_handler_t handler,
					   void *data);
LIBPLAT_PUBLIC int app_remove_native_event_listener(
    int event_type, app_native_event_handler_t handler);

LIBPLAT_INLINE int app_on_native_event(int event_type,
			       app_native_event_handler_t handler, void *data)
{
	return app_add_native_event_listener(event_type, handler, data);
}

LIBPLAT_INLINE int app_off_native_event(int event_type,
				app_native_event_handler_t handler)
{
	return app_remove_native_event_listener(event_type, handler);
}

// Events

LIBPLAT_PUBLIC int app_touch_event_init(app_event_t *e, touch_point_t *points,
				  int n_points);
LIBPLAT_PUBLIC int app_composition_event_init(app_event_t *e, const wchar_t *text,
					size_t len);
LIBPLAT_PUBLIC int app_event_copy(app_event_t *dest, app_event_t *src);
LIBPLAT_PUBLIC void app_event_destroy(app_event_t *e);
LIBPLAT_PUBLIC int app_touch_event_init(app_event_t *e, touch_point_t *points,
				  int n_points);
LIBPLAT_PUBLIC void app_init_events(void);
LIBPLAT_PUBLIC void app_destroy_events(void);
LIBPLAT_PUBLIC int app_post_event(app_event_t *e);
LIBPLAT_PUBLIC int app_process_event(app_event_t *e);

LIBPLAT_PUBLIC int app_add_event_listener(int event_type, app_event_handler_t handler,
				    void *data);
LIBPLAT_PUBLIC int app_remove_event_listener(int event_type,
				       app_event_handler_t handler);

LIBPLAT_INLINE int app_on_event(int event_type, app_event_handler_t handler, void *data)
{
	return app_add_event_listener(event_type, handler, data);
}

LIBPLAT_INLINE int app_off_event(int event_type, app_event_handler_t handler)
{
	return app_remove_event_listener(event_type, handler);
}

LIBPLAT_PUBLIC void app_set_event_dispatcher(app_event_dispatcher_t dispatcher);
LIBPLAT_PUBLIC void app_init_events(void);
LIBPLAT_PUBLIC void app_destroy_events(void);
LIBPLAT_PUBLIC int app_post_event(app_event_t *e);
LIBPLAT_PUBLIC void app_process_events(void);

LIBPLAT_END_DECLS

#endif

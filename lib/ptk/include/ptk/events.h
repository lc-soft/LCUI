/*
 * lib/ptk/include/ptk/events.h
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef PTK_INCLUDE_PLATFORM_EVENTS_H
#define PTK_INCLUDE_PLATFORM_EVENTS_H

#include "types.h"
#include "common.h"

PTK_BEGIN_DECLS

PTK_PUBLIC int ptk_process_native_events(ptk_process_events_option_t option);

PTK_PUBLIC int ptk_add_native_event_listener(int event_type,
					   ptk_native_event_handler_t handler,
					   void *data);
PTK_PUBLIC int ptk_remove_native_event_listener(
    int event_type, ptk_native_event_handler_t handler);

PTK_INLINE int ptk_on_native_event(int event_type,
			       ptk_native_event_handler_t handler, void *data)
{
	return ptk_add_native_event_listener(event_type, handler, data);
}

PTK_INLINE int ptk_off_native_event(int event_type,
				ptk_native_event_handler_t handler)
{
	return ptk_remove_native_event_listener(event_type, handler);
}

// Events

PTK_PUBLIC int ptk_touch_event_init(ptk_event_t *e, touch_point_t *points,
				  int n_points);
PTK_PUBLIC int ptk_composition_event_init(ptk_event_t *e, const wchar_t *text,
					size_t len);
PTK_PUBLIC int ptk_event_copy(ptk_event_t *dest, ptk_event_t *src);
PTK_PUBLIC void ptk_event_destroy(ptk_event_t *e);
PTK_PUBLIC int ptk_touch_event_init(ptk_event_t *e, touch_point_t *points,
				  int n_points);
PTK_PUBLIC void ptk_tick(void);
PTK_PUBLIC int ptk_post_event(ptk_event_t *e);
PTK_PUBLIC int ptk_process_event(ptk_event_t *e);

PTK_PUBLIC int ptk_add_event_listener(int event_type, ptk_event_handler_t handler,
				    void *data);
PTK_PUBLIC int ptk_remove_event_listener(int event_type,
				       ptk_event_handler_t handler);

PTK_INLINE int ptk_on_event(int event_type, ptk_event_handler_t handler, void *data)
{
	return ptk_add_event_listener(event_type, handler, data);
}

PTK_INLINE int ptk_off_event(int event_type, ptk_event_handler_t handler)
{
	return ptk_remove_event_listener(event_type, handler);
}

PTK_PUBLIC void ptk_set_event_dispatcher(ptk_event_dispatcher_t dispatcher);
PTK_PUBLIC int ptk_post_event(ptk_event_t *e);
PTK_PUBLIC void ptk_process_events(void);

PTK_END_DECLS

#endif

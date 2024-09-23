/*
 * lib/ptk/src/events.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <errno.h>
#include <yutil.h>
#include "events.h"

static struct {
        /** list_t<ptk_event_t> */
        list_t queue;

        /** list_t<app_listener_t> */
        list_t listeners;

        timer_list_t *timers;

        ptk_event_dispatcher_t dispatcher;
} ptk_events;

int ptk_touch_event_init(ptk_event_t *e, touch_point_t *points, int n_points)
{
        e->type = PTK_EVENT_TOUCH;
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

int ptk_composition_event_init(ptk_event_t *e, const wchar_t *text, size_t len)
{
        if (len == 0) {
                len = wcslen(text);
        }
        e->type = PTK_EVENT_COMPOSITION;
        e->text.length = len;
        e->text.text = malloc(sizeof(wchar_t) * (len + 1));
        if (!e->text.text) {
                return -ENOMEM;
        }
        wcscpy(e->text.text, text);
        return 0;
}

int ptk_event_copy(ptk_event_t *dest, ptk_event_t *src)
{
        switch (src->type) {
        case PTK_EVENT_TOUCH:
                return ptk_touch_event_init(dest, src->touch.points,
                                            src->touch.n_points);
        case PTK_EVENT_TEXTINPUT:
                return ptk_composition_event_init(dest, src->text.text, 0);
        default:
                *dest = *src;
                break;
        }
        return 0;
}

void ptk_event_destroy(ptk_event_t *e)
{
        switch (e->type) {
        case PTK_EVENT_TOUCH:
                if (e->touch.points) {
                        free(e->touch.points);
                }
                e->touch.points = NULL;
                e->touch.n_points = 0;
                break;
        case PTK_EVENT_COMPOSITION:
                if (e->text.text) {
                        free(e->text.text);
                }
                e->text.text = NULL;
                e->text.length = 0;
                break;
        default:
                break;
        }
        e->type = PTK_EVENT_NONE;
}

int ptk_post_event(ptk_event_t *e)
{
        ptk_event_t *ev;

        ev = malloc(sizeof(ptk_event_t));
        if (!ev) {
                return -ENOMEM;
        }
        ptk_event_copy(ev, e);
        list_append(&ptk_events.queue, ev);
        return 0;
}

int ptk_add_event_listener(int event_type, ptk_event_handler_t handler,
                           void *data)
{
        ptk_event_listener_t *listener;

        listener = malloc(sizeof(ptk_event_listener_t));
        if (!listener) {
                return -ENOMEM;
        }
        listener->handler = handler;
        listener->data = data;
        listener->type = event_type;
        list_append(&ptk_events.listeners, listener);
        return 0;
}

int ptk_remove_event_listener(int event_type, ptk_event_handler_t handler)
{
        list_node_t *node, *prev;
        ptk_event_listener_t *listener;

        for (list_each(node, &ptk_events.listeners)) {
                prev = node->prev;
                listener = node->data;
                if (listener->handler == handler &&
                    listener->type == event_type) {
                        list_delete_node(&ptk_events.listeners, node);
                        free(listener);
                        node = prev;
                        return 0;
                }
        }
        return -1;
}

void ptk_tick(void)
{
        ptk_event_t tick_event = { .type = PTK_EVENT_TICK };
        ptk_process_event(&tick_event);
}

int ptk_reset_timer(int timer_id, long ms)
{
        return timer_reset(ptk_events.timers, timer_id, ms);
}

int ptk_clear_timer(int timer_id)
{
        return timer_destroy(ptk_events.timers, timer_id);
}

int ptk_set_timeout(long ms, ptk_timer_cb cb, void *cb_arg)
{
        return timer_list_add_timeout(ptk_events.timers, ms, cb,
                                      cb_arg);
}

int ptk_set_interval(long ms, ptk_timer_cb cb, void *cb_arg)
{
        return timer_list_add_interval(ptk_events.timers, ms, cb,
                                      cb_arg);
}

int ptk_process_event(ptk_event_t *e)
{
        int count = 0;
        list_node_t *node;
        ptk_event_listener_t *listener;

        for (list_each(node, &ptk_events.listeners)) {
                listener = node->data;
                if (listener->type == e->type) {
                        listener->handler(e, listener->data);
                        ++count;
                }
        }
        timer_list_process(ptk_events.timers);
        if (ptk_events.dispatcher) {
                ptk_events.dispatcher(e);
        }
        return count;
}

void ptk_process_events(void)
{
        ptk_event_t *e;

        while ((e = list_get(&ptk_events.queue, 0)) != NULL) {
                list_delete(&ptk_events.queue, 0);
                ptk_process_event(e);
                ptk_event_destroy(e);
                free(e);
        }
}

void ptk_set_event_dispatcher(ptk_event_dispatcher_t dispatcher)
{
        ptk_events.dispatcher = dispatcher;
}

void ptk_events_init(void)
{
        ptk_events.timers = timer_list_create();
        list_create(&ptk_events.queue);
}

void ptk_events_destroy(void)
{
        ptk_set_event_dispatcher(NULL);
        list_destroy(&ptk_events.queue, free);
        timer_list_destroy(ptk_events.timers);
}

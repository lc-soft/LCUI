/*
 * lib/router/src/history.c
 *
 * Copyright (c) 2023-2024, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"
#include <router/route.h>
#include <router/history.h>

router_history_t *router_history_create(void)
{
        router_history_t *history;

        history = malloc(sizeof(router_history_t));
        history->index = 0;
        history->current = NULL;
        list_create(&history->watchers);
        list_create(&history->routes);
        return history;
}

static void router_history_on_destroy_route(void *data)
{
        router_route_destroy(data);
}

void router_history_destroy(router_history_t *history)
{
        history->index = 0;
        history->current = NULL;
        list_destroy_without_node(&history->watchers, free);
        list_destroy(&history->routes, router_history_on_destroy_route);
        free(history);
}

router_watcher_t *router_history_watch(router_history_t *history,
                                       router_callback_t callback, void *data)
{
        router_watcher_t *watcher;

        watcher = malloc(sizeof(router_watcher_t));
        watcher->node.data = watcher;
        watcher->node.next = NULL;
        watcher->node.prev = NULL;
        watcher->data = data;
        watcher->callback = callback;
        list_append_node(&history->watchers, &watcher->node);
        return watcher;
}

void router_history_unwatch(router_history_t *history,
                            router_watcher_t *watcher)
{
        list_unlink(&history->watchers, &watcher->node);
        free(watcher);
}

static void router_history_change(router_history_t *history, router_route_t *to)
{
        router_watcher_t *watcher;
        router_linkedlist_node_t *node;

        for (list_each(node, &history->watchers)) {
                watcher = node->data;
                watcher->callback(watcher->data, to, history->current);
        }
        history->current = to;
}

void router_history_push(router_history_t *history, router_route_t *route)
{
        int index = 0;
        router_linkedlist_node_t *next;
        router_linkedlist_node_t *node = NULL;

        for (list_each(node, &history->routes)) {
                if (index <= history->index) {
                        index++;
                        continue;
                }
                while (node) {
                        next = node->next;
                        router_route_destroy(node->data);
                        list_delete_node(&history->routes, node);
                        node = next;
                }
                break;
        }
        router_history_change(history, route);
        list_append(&history->routes, route);
        history->index = (int)history->routes.length - 1;
}

void router_history_replace(router_history_t *history, router_route_t *route)
{
        router_linkedlist_node_t *node;

        node = list_get_node(&history->routes, history->index);
        router_history_change(history, route);
        router_route_destroy(node->data);
        list_insert(&history->routes, history->index, route);
        list_delete_node(&history->routes, node);
}

void router_history_go(router_history_t *history, int delta)
{
        history->index += delta;
        if (history->index < 0) {
                history->index = 0;
        } else if ((size_t)history->index >= history->routes.length) {
                history->index = (int)history->routes.length - 1;
        }
        router_history_change(history,
                              list_get(&history->routes, history->index));
}

size_t router_history_get_index(const router_history_t *history)
{
        return history->index;
}

size_t router_history_get_length(const router_history_t *history)
{
        return history->routes.length;
}

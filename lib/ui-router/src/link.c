/*
 * lib/ui-router/src/link.c
 *
 * Copyright (c) 2023, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include "private.h"
#include <ui_router.h>

// Refer: https://router.vuejs.org/api/#router-link-props

typedef struct ui_router_link_t {
        char *active_class;
        char *exact_active_class;
        bool exact;
        bool replace;
        router_t *router;
        router_location_t *to;
        router_route_record_t *record;
        router_watcher_t *watcher;
} ui_router_link_t;

static ui_widget_prototype_t *ui_router_link_proto;

static void ui_router_link_on_route_update(void *w, const router_route_t *to,
                                           const router_route_t *from)
{
        ui_router_link_t *link;
        router_route_t *route;
        router_resolved_t *resolved;
        bool is_same_route;

        link = ui_widget_get_data(w, ui_router_link_proto);
        resolved = router_resolve(link->router, link->to, FALSE);
        route = router_resolved_get_route(resolved);
        // https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/src/components/link.js#L65
        is_same_route = router_is_same_route(to, route);
        if (is_same_route) {
                ui_widget_add_class(w, link->exact_active_class);
        } else {
                ui_widget_remove_class(w, link->exact_active_class);
        }
        do {
                if (link->exact) {
                        if (is_same_route) {
                                ui_widget_add_class(w, link->active_class);
                                break;
                        }
                } else if (router_is_included_route(to, route)) {
                        ui_widget_add_class(w, link->active_class);
                        break;
                }
                ui_widget_remove_class(w, link->active_class);
        } while (0);
        router_resolved_destroy(resolved);
}

static void ui_router_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_router_link_t *link;

        link = ui_widget_get_data(w, ui_router_link_proto);
        if (link->replace) {
                router_replace(link->router, link->to);
        } else {
                router_push(link->router, link->to);
        }
}

static void ui_router_link_on_set_attr(ui_widget_t *w, const char *name,
                                       const char *value)
{
        ui_router_link_t *link;
        router_location_t *location;

        link = ui_widget_get_data(w, ui_router_link_proto);
        if (strcmp(name, "to") == 0) {
                location = router_location_create(NULL, value);
                ui_router_link_set_location(w, location);
                router_location_destroy(location);
        } else if (strcmp(name, "exact") == 0) {
                ui_router_link_set_exact(w, strcmp(value, "exact") == 0);
        } else if (strcmp(name, "exact-active-class") == 0) {
                if (link->exact_active_class) {
                        free(link->exact_active_class);
                }
                link->exact_active_class = strdup(value);
        } else if (strcmp(name, "active-class") == 0) {
                if (link->active_class) {
                        free(link->active_class);
                }
                link->active_class = strdup(value);
        } else {
                ui_router_link_proto->proto->setattr(w, name, value);
        }
}

static void ui_router_link_on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *name = NULL;
        router_t *router;

        ui_router_link_t *link;
        ui_widget_t *parent;

        link = ui_widget_get_data(w, ui_router_link_proto);
        for (parent = w; parent; parent = parent->parent) {
                name = ui_widget_get_attr(parent, "router");
                if (name) {
                        break;
                }
        }
        if (!name) {
                name = "default";
        }
        router = router_get_by_name(name);
        link->watcher = router_watch(router, ui_router_link_on_route_update, w);
        link->router = router;
        ui_widget_off(w, "ready", ui_router_link_on_ready, NULL);
}

static void ui_router_link_on_init(ui_widget_t *w)
{
        ui_router_link_t *link;

        link = ui_widget_add_data(w, ui_router_link_proto,
                                  sizeof(ui_router_link_t));
        link->active_class = strdup("router-link-active");
        link->exact_active_class = strdup("router-link-exact-active");
        link->to = NULL;
        link->replace = FALSE;
        link->exact = FALSE;
        link->router = NULL;
        link->watcher = NULL;
        ui_widget_on(w, "ready", ui_router_link_on_ready, NULL);
        ui_widget_on(w, "click", ui_router_on_click, NULL);
        ui_router_link_proto->proto->init(w);
}

static void ui_router_link_on_destroy(ui_widget_t *w)
{
        ui_router_link_t *link;

        link = ui_widget_get_data(w, ui_router_link_proto);
        if (link->router) {
                router_unwatch(link->router, link->watcher);
        }
        router_location_destroy(link->to);
        router_mem_free(link->active_class);
        router_mem_free(link->exact_active_class);
        link->watcher = NULL;
        link->to = NULL;
        ui_router_link_proto->proto->destroy(w);
}

void ui_router_link_set_location(ui_widget_t *w, router_location_t *location)
{
        ui_router_link_t *link;

        link = ui_widget_get_data(w, ui_router_link_proto);
        if (link->to) {
                router_location_destroy(link->to);
        }
        link->to = router_location_duplicate(location);
}

void ui_router_link_set_exact(ui_widget_t *w, bool exact)
{
        ui_router_link_t *link;

        link = ui_widget_get_data(w, ui_router_link_proto);
        link->exact = exact;
}

void ui_register_router_link(void)
{
        ui_router_link_proto =
            ui_create_widget_prototype("router-link", "text");
        ui_router_link_proto->init = ui_router_link_on_init;
        ui_router_link_proto->setattr = ui_router_link_on_set_attr;
        ui_router_link_proto->destroy = ui_router_link_on_destroy;
}

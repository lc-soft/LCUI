#include "private.h"
#include <ui_router.h>

typedef struct ui_router_view_t {
        size_t index;
        router_t *router;
        router_watcher_t *watcher;
        bool keep_alive;
        dict_t *cache;
        dict_type_t cache_dict_type;
        ui_widget_t *matched_widget;
} ui_router_view_t;

static ui_widget_prototype_t *ui_router_view_proto;

static ui_widget_t *ui_router_view_get_matched(ui_widget_t *w,
                                               const router_route_t *route)
{
        const char *name;
        const char *component_name;
        const router_route_record_t *record;
        ui_router_view_t *view;
        ui_widget_t *component;

        view = ui_widget_get_data(w, ui_router_view_proto);
        name = ui_widget_get_attribute_value(w, "name");
        record = router_route_get_matched_record(route, view->index);
        if (!record) {
                logger_error("%s", "no matching route found");
                return NULL;
        }
        if (!name) {
                name = "default";
        }
        component_name = router_route_record_get_component(record, name);
        if (view->keep_alive) {
                component = dict_fetch_value(view->cache, component_name);
                if (!component) {
                        component = ui_create_widget(component_name);
                        dict_add(view->cache, (void *)component_name,
                                 component);
                }
        } else {
                component = ui_create_widget(component_name);
        }
        return component;
}

static void ui_router_view_on_route_update(void *w, const router_route_t *to,
                                           const router_route_t *from)
{
        ui_router_view_t *view;

        view = ui_widget_get_data(w, ui_router_view_proto);
        if (view->matched_widget && view->keep_alive) {
                ui_widget_unlink(view->matched_widget);
        }
        view->matched_widget = ui_router_view_get_matched(w, to);
        ui_widget_empty(w);
        ui_widget_append(w, view->matched_widget);
}

static void ui_router_view_on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        size_t index;
        router_t *router;
        const char *name = NULL;
        const router_route_t *route;

        ui_widget_t *parent;
        ui_router_view_t *view;

        view = ui_widget_get_data(w, ui_router_view_proto);
        for (index = 0, parent = w->parent; parent; parent = parent->parent) {
                if (ui_check_widget_type(parent, "router-view")) {
                        ++index;
                }
        }
        for (parent = w; parent; parent = parent->parent) {
                name = ui_widget_get_attribute_value(parent, "router");
                if (name) {
                        break;
                }
        }
        if (!name) {
                name = "default";
        }
        router = router_get_by_name(name);
        route = router_get_current_route(router);
        view->watcher = router_watch(router, ui_router_view_on_route_update, w);
        view->router = router;
        view->index = index;
        if (route) {
                view->matched_widget = ui_router_view_get_matched(w, route);
                ui_widget_append(w, view->matched_widget);
        } else {
                view->matched_widget = NULL;
        }
        ui_widget_off(w, "ready", ui_router_view_on_ready, NULL);
}

ui_widget_t *ui_router_view_get_matched_widget(ui_widget_t *w)
{
        ui_router_view_t *view;

        view = ui_widget_get_data(w, ui_router_view_proto);
        return view->matched_widget;
}

static void ui_router_view_on_init(ui_widget_t *w)
{
        ui_router_view_t *view;

        view = ui_widget_add_data(w, ui_router_view_proto,
                                  sizeof(ui_router_view_t));
        view->router = NULL;
        view->watcher = NULL;
        dict_init_string_copy_key_type(&view->cache_dict_type);
        view->cache = dict_create(&view->cache_dict_type, NULL);
        view->keep_alive = FALSE;
        ui_widget_on(w, "ready", ui_router_view_on_ready, NULL, NULL);
}

static void ui_router_view_on_destroy(ui_widget_t *w)
{
        ui_router_view_t *view;

        view = ui_widget_get_data(w, ui_router_view_proto);
        if (view->router) {
                router_unwatch(view->router, view->watcher);
        }
        dict_destroy(view->cache);
        view->cache = NULL;
        view->watcher = NULL;
        view->router = NULL;
}

void ui_register_router_view(void)
{
        ui_router_view_proto = ui_create_widget_prototype("router-view", NULL);
        ui_router_view_proto->init = ui_router_view_on_init;
        ui_router_view_proto->destroy = ui_router_view_on_destroy;
}

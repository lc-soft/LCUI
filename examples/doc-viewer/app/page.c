#include <stdio.h>
#include <router.h>
#include "page.tsx.h"
#include "page.h"
#include "locale.h"

typedef struct {
        root_page_react_t base;
} root_page_t;

static void root_page_on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        router_t *router = router_get_by_name("AppRouter");
        router_location_t *location;
        char path[128];

        ui_widget_off(w, "ready", root_page_on_ready, NULL);
        if (!router) {
                return;
        }
        snprintf(path, sizeof(path), "/%s/home", app_get_locale());
        location = router_location_create(NULL, path);
        router_replace(router, location);
        router_location_destroy(location);
}

static void root_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, root_page_proto, sizeof(root_page_t));
        root_page_react_init(w);
        ui_widget_on(w, "ready", root_page_on_ready, NULL);
        root_page_update(w);
}

static void root_page_destroy(ui_widget_t *w)
{
        root_page_react_destroy(w);
}

void root_page_update(ui_widget_t *w)
{
        root_page_react_update(w);
}

ui_widget_t *ui_create_root_page(void)
{
        return ui_create_widget_with_prototype(root_page_proto);
}

void ui_register_root_page(void)
{
        root_page_init_prototype();
        root_page_proto->init = root_page_init;
        root_page_proto->destroy = root_page_destroy;
}

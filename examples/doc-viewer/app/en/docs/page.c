#include <router.h>
#include "page.tsx.h"
#include "page.h"

typedef struct {
        en_docs_page_react_t base;
} en_docs_page_t;

static void en_docs_page_on_ready(ui_widget_t *w, ui_event_t *e, void *arg)
{
        router_t *router = router_get_by_name("AppRouter");
        router_location_t *location;

        ui_widget_off(w, "ready", en_docs_page_on_ready, NULL);
        if (!router) {
                return;
        }
        location =
            router_location_create(NULL, "/en/docs/overview/quick-start");
        router_replace(router, location);
        router_location_destroy(location);
}

static void en_docs_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_docs_page_proto, sizeof(en_docs_page_t));
        en_docs_page_react_init(w);
        ui_widget_on(w, "ready", en_docs_page_on_ready, NULL);
        en_docs_page_update(w);
}

static void en_docs_page_destroy(ui_widget_t *w)
{
        en_docs_page_react_destroy(w);
}

void en_docs_page_update(ui_widget_t *w)
{
        en_docs_page_react_update(w);
}

ui_widget_t *ui_create_en_docs_page(void)
{
        return ui_create_widget_with_prototype(en_docs_page_proto);
}

void ui_register_en_docs_page(void)
{
        en_docs_page_init_prototype();
        en_docs_page_proto->init = en_docs_page_init;
        en_docs_page_proto->destroy = en_docs_page_destroy;
}

#include <router.h>
#include "page.tsx.h"
#include "page.h"

static void widgets_index_on_nav(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *path = ui_widget_get_attr(w, "data-to");
        router_t *router = router_get_by_name("AppRouter");
        router_location_t *location;

        if (!path || !router) {
                return;
        }
        location = router_location_create(NULL, path);
        router_replace(router, location);
        router_location_destroy(location);
}
typedef struct {
        zh_cn_docs_widgets_page_react_t base;
} zh_cn_docs_widgets_page_t;

static void zh_cn_docs_widgets_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, zh_cn_docs_widgets_page_proto,
                           sizeof(zh_cn_docs_widgets_page_t));
        zh_cn_docs_widgets_page_react_init(w);
        zh_cn_docs_widgets_page_update(w);
}

static void zh_cn_docs_widgets_page_destroy(ui_widget_t *w)
{
        zh_cn_docs_widgets_page_react_destroy(w);
}

void zh_cn_docs_widgets_page_update(ui_widget_t *w)
{
        zh_cn_docs_widgets_page_react_update(w);
}

ui_widget_t *ui_create_zh_cn_docs_widgets_page(void)
{
        return ui_create_widget_with_prototype(zh_cn_docs_widgets_page_proto);
}

void ui_register_zh_cn_docs_widgets_page(void)
{
        zh_cn_docs_widgets_page_init_prototype();
        zh_cn_docs_widgets_page_proto->init = zh_cn_docs_widgets_page_init;
        zh_cn_docs_widgets_page_proto->destroy =
            zh_cn_docs_widgets_page_destroy;
}

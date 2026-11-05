#include "page.tsx.h"
#include "page.h"

typedef struct {
        en_home_page_react_t base;
} en_home_page_t;

static void en_home_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_home_page_proto, sizeof(en_home_page_t));
        en_home_page_react_init(w);
        en_home_page_update(w);
}

static void en_home_page_destroy(ui_widget_t *w)
{
        en_home_page_react_destroy(w);
}

void en_home_page_update(ui_widget_t *w)
{
        en_home_page_react_update(w);
}

ui_widget_t *ui_create_en_home_page(void)
{
        return ui_create_widget_with_prototype(en_home_page_proto);
}

void ui_register_en_home_page(void)
{
        en_home_page_init_prototype();
        en_home_page_proto->init = en_home_page_init;
        en_home_page_proto->destroy = en_home_page_destroy;
}

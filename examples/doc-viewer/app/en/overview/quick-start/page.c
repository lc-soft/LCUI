#include "page.tsx.h"
#include "page.h"

typedef struct {
        en_overview_quick_start_page_react_t base;
        // Add additional states to your component here
        // ...
} en_overview_quick_start_page_t;

static void en_overview_quick_start_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_overview_quick_start_page_proto,
                           sizeof(en_overview_quick_start_page_t));
        en_overview_quick_start_page_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        en_overview_quick_start_page_update(w);
}

static en_overview_quick_start_page_t *en_overview_quick_start_page_get(
    ui_widget_t *w)
{
        return ui_widget_get_data(w, en_overview_quick_start_page_proto);
}

static void en_overview_quick_start_page_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        en_overview_quick_start_page_react_destroy(w);
}

void en_overview_quick_start_page_update(ui_widget_t *w)
{
        en_overview_quick_start_page_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_en_overview_quick_start_page(void)
{
        return ui_create_widget_with_prototype(
            en_overview_quick_start_page_proto);
}

void ui_register_en_overview_quick_start_page(void)
{
        en_overview_quick_start_page_init_prototype();
        en_overview_quick_start_page_proto->init =
            en_overview_quick_start_page_init;
        en_overview_quick_start_page_proto->destroy =
            en_overview_quick_start_page_destroy;
}

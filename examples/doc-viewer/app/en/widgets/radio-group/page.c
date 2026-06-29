#include "page.tsx.h"
#include "page.h"

typedef struct {
        en_widgets_radio_group_page_react_t base;
        // Add additional states to your component here
        // ...
} en_widgets_radio_group_page_t;

static void en_widgets_radio_group_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_widgets_radio_group_page_proto,
                           sizeof(en_widgets_radio_group_page_t));

        en_widgets_radio_group_page_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        en_widgets_radio_group_page_update(w);
}

static en_widgets_radio_group_page_t *en_widgets_radio_group_page_get(
    ui_widget_t *w)
{
        return ui_widget_get_data(w, en_widgets_radio_group_page_proto);
}

static void en_widgets_radio_group_page_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        en_widgets_radio_group_page_react_destroy(w);
}

void en_widgets_radio_group_page_update(ui_widget_t *w)
{
        en_widgets_radio_group_page_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_en_widgets_radio_group_page(void)
{
        return ui_create_widget_with_prototype(
            en_widgets_radio_group_page_proto);
}

void ui_register_en_widgets_radio_group_page(void)
{
        en_widgets_radio_group_page_init_prototype();
        en_widgets_radio_group_page_proto->init =
            en_widgets_radio_group_page_init;
        en_widgets_radio_group_page_proto->destroy =
            en_widgets_radio_group_page_destroy;
}

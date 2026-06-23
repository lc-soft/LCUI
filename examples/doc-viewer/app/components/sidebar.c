#include "sidebar.h"
#include "sidebar.tsx.h"

typedef struct {
        sidebar_react_t base;
        // Add additional states to your component here
        // ...
} sidebar_t;

static void sidebar_init(ui_widget_t *w)
{
        ui_widget_add_data(w, sidebar_proto, sizeof(sidebar_t));
        sidebar_proto->proto->init(w);
        sidebar_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        sidebar_update(w);
}

static sidebar_t *sidebar_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, sidebar_proto);
}

static void sidebar_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        sidebar_react_destroy(w);
}

void sidebar_update(ui_widget_t *w)
{
        sidebar_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_sidebar(void)
{
        return ui_create_widget_with_prototype(sidebar_proto);
}

void ui_register_sidebar(void)
{
        sidebar_init_prototype();
        sidebar_proto->init = sidebar_init;
        sidebar_proto->destroy = sidebar_destroy;
}

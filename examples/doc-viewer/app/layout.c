#include "layout.h"
#include "layout.tsx.h"

typedef struct {
        root_layout_react_t base;
        // Add additional states to your component here
        // ...
} root_layout_t;

static void root_layout_init(ui_widget_t *w)
{
        ui_widget_add_data(w, root_layout_proto, sizeof(root_layout_t));
        root_layout_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        root_layout_update(w);
}

static root_layout_t *root_layout_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, root_layout_proto);
}

static void root_layout_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        root_layout_react_destroy(w);
}

void root_layout_update(ui_widget_t *w)
{
        root_layout_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_root_layout(void)
{
        return ui_create_widget_with_prototype(root_layout_proto);
}

void ui_register_root_layout(void)
{
        root_layout_init_prototype();
        root_layout_proto->init = root_layout_init;
        root_layout_proto->destroy = root_layout_destroy;
}

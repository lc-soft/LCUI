#include "layout.tsx.h"
#include "layout.h"

typedef struct {
        en_docs_layout_react_t base;
        // Add additional states to your component here
        // ...
} en_docs_layout_t;

static void en_docs_layout_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_docs_layout_proto, sizeof(en_docs_layout_t));

        en_docs_layout_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        en_docs_layout_update(w);
}

static en_docs_layout_t *en_docs_layout_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, en_docs_layout_proto);
}

static void en_docs_layout_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        en_docs_layout_react_destroy(w);
}

void en_docs_layout_update(ui_widget_t *w)
{
        en_docs_layout_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_en_docs_layout(void)
{
        return ui_create_widget_with_prototype(en_docs_layout_proto);
}

void ui_register_en_docs_layout(void)
{
        en_docs_layout_init_prototype();
        en_docs_layout_proto->init = en_docs_layout_init;
        en_docs_layout_proto->destroy = en_docs_layout_destroy;
}

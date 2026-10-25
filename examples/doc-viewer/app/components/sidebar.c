#include "sidebar.h"
#include "sidebar.tsx.h"

typedef struct {
        docs_sidebar_react_t base;
        // Add additional states to your component here
        // ...
} docs_sidebar_t;

static void docs_sidebar_init(ui_widget_t *w)
{
        ui_widget_add_data(w, docs_sidebar_proto, sizeof(docs_sidebar_t));
        docs_sidebar_proto->proto->init(w);
        docs_sidebar_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        docs_sidebar_update(w);
}

static docs_sidebar_t *docs_sidebar_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, docs_sidebar_proto);
}

static void docs_sidebar_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        docs_sidebar_react_destroy(w);
}

void docs_sidebar_update(ui_widget_t *w)
{
        docs_sidebar_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_docs_sidebar(void)
{
        return ui_create_widget_with_prototype(docs_sidebar_proto);
}

void ui_register_docs_sidebar(void)
{
        docs_sidebar_init_prototype();
        docs_sidebar_proto->init = docs_sidebar_init;
        docs_sidebar_proto->destroy = docs_sidebar_destroy;
}

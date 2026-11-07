#include "page.tsx.h"
#include "page.h"

typedef struct {
        en_docs_widgets_page_react_t base;
} en_docs_widgets_page_t;

static void en_docs_widgets_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, en_docs_widgets_page_proto,
                           sizeof(en_docs_widgets_page_t));
        en_docs_widgets_page_react_init(w);
        en_docs_widgets_page_update(w);
}

static void en_docs_widgets_page_destroy(ui_widget_t *w)
{
        en_docs_widgets_page_react_destroy(w);
}

void en_docs_widgets_page_update(ui_widget_t *w)
{
        en_docs_widgets_page_react_update(w);
}

ui_widget_t *ui_create_en_docs_widgets_page(void)
{
        return ui_create_widget_with_prototype(en_docs_widgets_page_proto);
}

void ui_register_en_docs_widgets_page(void)
{
        en_docs_widgets_page_init_prototype();
        en_docs_widgets_page_proto->init = en_docs_widgets_page_init;
        en_docs_widgets_page_proto->destroy = en_docs_widgets_page_destroy;
}

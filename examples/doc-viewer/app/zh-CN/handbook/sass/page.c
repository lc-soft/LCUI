#include "page.tsx.h"
#include "page.h"

typedef struct {
        zh_cn_handbook_sass_page_react_t base;
        // Add additional states to your component here
        // ...
} zh_cn_handbook_sass_page_t;

static void zh_cn_handbook_sass_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, zh_cn_handbook_sass_page_proto,
                           sizeof(zh_cn_handbook_sass_page_t));

        zh_cn_handbook_sass_page_react_init(w);
        // Write the initialization code for your component here
        // such as state initialization, event binding, etc
        // ...

        zh_cn_handbook_sass_page_update(w);
}

static zh_cn_handbook_sass_page_t *zh_cn_handbook_sass_page_get(ui_widget_t *w)
{
        return ui_widget_get_data(w, zh_cn_handbook_sass_page_proto);
}

static void zh_cn_handbook_sass_page_destroy(ui_widget_t *w)
{
        // Write code here to destroy the relevant resources of the component
        // ...

        zh_cn_handbook_sass_page_react_destroy(w);
}

void zh_cn_handbook_sass_page_update(ui_widget_t *w)
{
        zh_cn_handbook_sass_page_react_update(w);
        // Write code here to update other content of your component
        // ...
}

ui_widget_t *ui_create_zh_cn_handbook_sass_page(void)
{
        return ui_create_widget_with_prototype(zh_cn_handbook_sass_page_proto);
}

void ui_register_zh_cn_handbook_sass_page(void)
{
        zh_cn_handbook_sass_page_init_prototype();
        zh_cn_handbook_sass_page_proto->init = zh_cn_handbook_sass_page_init;
        zh_cn_handbook_sass_page_proto->destroy =
            zh_cn_handbook_sass_page_destroy;
}

#include "page.tsx.h"
#include "page.h"

typedef struct {
        zh_cn_home_page_react_t base;
} zh_cn_home_page_t;

static void zh_cn_home_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, zh_cn_home_page_proto, sizeof(zh_cn_home_page_t));
        zh_cn_home_page_react_init(w);
        zh_cn_home_page_update(w);
}

static void zh_cn_home_page_destroy(ui_widget_t *w)
{
        zh_cn_home_page_react_destroy(w);
}

void zh_cn_home_page_update(ui_widget_t *w)
{
        zh_cn_home_page_react_update(w);
}

ui_widget_t *ui_create_zh_cn_home_page(void)
{
        return ui_create_widget_with_prototype(zh_cn_home_page_proto);
}

void ui_register_zh_cn_home_page(void)
{
        zh_cn_home_page_init_prototype();
        zh_cn_home_page_proto->init = zh_cn_home_page_init;
        zh_cn_home_page_proto->destroy = zh_cn_home_page_destroy;
}

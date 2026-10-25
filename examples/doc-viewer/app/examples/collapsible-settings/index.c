#include "index.tsx.h"
#include "index.h"

#define create_radius_field collapsible_settings__create_radius_field
#include "../../../../../docs/examples/collapsible-settings/main.c"
#undef create_radius_field

typedef struct {
        collapsible_settings_demo_react_t base;
} collapsible_settings_demo_t;

void collapsible_settings_demo_update(ui_widget_t *w)
{
        collapsible_settings_demo_react_update(w);
}

static void collapsible_settings_demo_init(ui_widget_t *w)
{
        collapsible_settings_demo_t *_that =
            ui_widget_add_data(w, collapsible_settings_demo_proto,
                               sizeof(collapsible_settings_demo_t));
        collapsible_settings_demo_proto->proto->init(w);
        collapsible_settings_demo_react_init(w);
        collapsible_settings_init(_that->base.refs.preview);
        collapsible_settings_demo_update(w);
}

static void collapsible_settings_demo_destroy(ui_widget_t *w)
{
        collapsible_settings_demo_proto->proto->destroy(w);
        collapsible_settings_demo_react_destroy(w);
}

ui_widget_t *ui_create_collapsible_settings_demo(void)
{
        return ui_create_widget_with_prototype(collapsible_settings_demo_proto);
}

void ui_register_collapsible_settings_demo(void)
{
        collapsible_settings_demo_init_prototype();
        collapsible_settings_demo_proto->init = collapsible_settings_demo_init;
        collapsible_settings_demo_proto->destroy =
            collapsible_settings_demo_destroy;
}

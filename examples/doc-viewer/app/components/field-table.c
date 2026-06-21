#include <string.h>
#include <LCUI.h>
#include <LCUI/widgets.h>
#include "field-table.tsx.h"
#include "field-table.h"

typedef struct {
        field_table_provider_react_t base;
} field_table_provider_t;

static ui_widget_t *find_ancestor_with_class(ui_widget_t *w, const char *cls)
{
        while (w) {
                if (ui_widget_has_class(w, cls))
                        return w;
                w = w->parent;
        }
        return NULL;
}

static void on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *target = e->target;
        ui_widget_t *row, *details;

        if (ui_widget_has_class(target, "field-table-header"))
                return;
        row = find_ancestor_with_class(target, "field-table-row");
        if (!row) {
                return;
        }
        details = ui_widget_next(row);
        if (ui_widget_has_class(row, "expanded")) {
                ui_widget_remove_class(row, "expanded");
                ui_widget_remove_class(details, "expanded");
        } else {
                ui_widget_add_class(row, "expanded");
                ui_widget_add_class(details, "expanded");
        }
}

static void field_table_provider_init(ui_widget_t *w)
{
        ui_widget_add_data(w, field_table_provider_proto,
                           sizeof(field_table_provider_t));
        field_table_provider_react_init(w);
        ui_widget_on(w, "click", on_click, NULL);
}

static void field_table_provider_destroy(ui_widget_t *w)
{
        field_table_provider_react_destroy(w);
}

void ui_register_field_table_provider(void)
{
        field_table_provider_init_prototype();
        field_table_provider_proto->init = field_table_provider_init;
        field_table_provider_proto->destroy = field_table_provider_destroy;
}

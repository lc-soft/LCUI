#include "index.tsx.h"
#include "index.h"

#define create_file_row collapsible_file_tree__create_file_row
#define create_folder_row collapsible_file_tree__create_folder_row
#include "../../../../../docs/examples/collapsible-file-tree/main.c"
#undef create_file_row
#undef create_folder_row

typedef struct {
        collapsible_file_tree_demo_react_t base;
} collapsible_file_tree_demo_t;

void collapsible_file_tree_demo_update(ui_widget_t *w)
{
        collapsible_file_tree_demo_react_update(w);
}

static void collapsible_file_tree_demo_init(ui_widget_t *w)
{
        collapsible_file_tree_demo_t *_that =
            ui_widget_add_data(w, collapsible_file_tree_demo_proto,
                               sizeof(collapsible_file_tree_demo_t));
        collapsible_file_tree_demo_proto->proto->init(w);
        collapsible_file_tree_demo_react_init(w);
        collapsible_file_tree_init(_that->base.refs.preview);
        collapsible_file_tree_demo_update(w);
}

static void collapsible_file_tree_demo_destroy(ui_widget_t *w)
{
        collapsible_file_tree_demo_proto->proto->destroy(w);
        collapsible_file_tree_demo_react_destroy(w);
}

ui_widget_t *ui_create_collapsible_file_tree_demo(void)
{
        return ui_create_widget_with_prototype(
            collapsible_file_tree_demo_proto);
}

void ui_register_collapsible_file_tree_demo(void)
{
        collapsible_file_tree_demo_init_prototype();
        collapsible_file_tree_demo_proto->init =
            collapsible_file_tree_demo_init;
        collapsible_file_tree_demo_proto->destroy =
            collapsible_file_tree_demo_destroy;
}

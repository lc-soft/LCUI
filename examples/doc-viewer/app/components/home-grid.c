/*
 * app/components/home-grid.c - Responsive gallery used by the doc-viewer home
 * page.
 *
 * Cards are declared as direct children of <home-grid>.  They are distributed
 * over four flex columns once; narrow windows then hide the middle columns
 * (window >=1400: 1-2-3-4, >=1024: 1-2-4, otherwise: 1-4).  Keeping the
 * assignment fixed avoids reshuffling cards when the window is resized.
 * LCUI has no media queries, so the strategy lives in C.
 */

#include <stdlib.h>
#include <string.h>
#include <LCUI.h>
#include <css.h>
#include <ui/mutation_observer.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include "home-grid.h"

#define HOME_GRID_COLUMNS 4

typedef struct {
        bool pending;
        bool distributing;
        int visible_columns;
        ui_widget_t *column_widgets[HOME_GRID_COLUMNS];
        ui_mutation_observer_t *observer;
} home_grid_data_t;

static ui_widget_prototype_t *home_grid_proto;
static ui_widget_prototype_t *home_column_proto;

static home_grid_data_t *home_grid_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, home_grid_proto);
}

static bool home_grid_is_column(ui_widget_t *w)
{
        return w->proto == home_column_proto;
}

static bool home_grid_has_direct_cards(ui_widget_t *w)
{
        list_node_t *node;

        for (list_each(node, &w->children)) {
                ui_widget_t *child = node->data;

                if (!home_grid_is_column(child)) {
                        return true;
                }
        }
        return false;
}

static void home_grid_ensure_columns(ui_widget_t *w, home_grid_data_t *data)
{
        int i;

        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                if (data->column_widgets[i]) {
                        continue;
                }
                data->column_widgets[i] =
                    ui_create_widget_with_prototype(home_column_proto);
                ui_widget_append(w, data->column_widgets[i]);
                /* Force the next visibility pass to initialize the column. */
                data->visible_columns = 0;
        }
}

static size_t home_grid_collect_cards(ui_widget_t *w, home_grid_data_t *data,
                                      ui_widget_t **cards)
{
        size_t count = 0;
        size_t i;
        size_t j;
        ui_widget_t *child;
        list_node_t *node;

        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                for (j = 0;
                     (child = ui_widget_get_child(data->column_widgets[i], j));
                     ++j) {
                        if (cards) {
                                cards[count] = child;
                        }
                        ++count;
                }
        }
        for (list_each(node, &w->children)) {
                child = node->data;
                if (home_grid_is_column(child)) {
                        continue;
                }
                if (cards) {
                        cards[count] = child;
                }
                ++count;
        }
        return count;
}

static int home_grid_column_of(ui_widget_t *card, size_t index)
{
        const char *value = ui_widget_get_attr(card, "data-column");

        if (value) {
                int column = atoi(value);

                if (column >= 1 && column <= HOME_GRID_COLUMNS) {
                        return column - 1;
                }
        }
        return (int)(index % HOME_GRID_COLUMNS);
}

/* Cards are spread over all four columns regardless of the current width so
 * that hiding a column only affects which cards are visible, not where the
 * remaining ones sit. */
static void home_grid_apply(ui_widget_t *w, home_grid_data_t *data)
{
        size_t count;
        size_t i;
        ui_widget_t **cards;

        data->distributing = true;
        home_grid_ensure_columns(w, data);
        count = home_grid_collect_cards(w, data, NULL);
        cards = count ? malloc(sizeof(*cards) * count) : NULL;
        if (cards) {
                home_grid_collect_cards(w, data, cards);
                for (i = 0; i < count; ++i) {
                        ui_widget_append(
                            data->column_widgets[home_grid_column_of(cards[i],
                                                                     i)],
                            cards[i]);
                }
                free(cards);
        }
        data->pending = false;
        data->distributing = false;
}

/* Window breakpoints: >=1400 -> 4 columns, >=1024 -> 3, otherwise 2.  Read
 * the root content width so transient widths of intermediate containers
 * (scrollbar/padding resolution) cannot change the column count. */
static int home_grid_columns_for_window(void)
{
        ui_widget_t *root = ui_root();
        float width;

        if (!root) {
                return 0;
        }
        width = root->content_box.width;
        if (width <= 0) {
                return 0;
        }
        if (width >= 1400) {
                return 4;
        }
        if (width >= 1024) {
                return 3;
        }
        return 2;
}

/* Wide screens show all four columns; medium hides the third one; narrow
 * keeps only the first and the last one. Column widths are left to flexbox:
 * every visible home-column has flex:1 1 0 and min-width:0, so the used grid
 * width is divided equally without feeding fixed widths back into the scroll
 * content's min-content size. */
static void home_grid_update_visibility(ui_widget_t *w)
{
        home_grid_data_t *data = home_grid_get_data(w);
        bool visible[HOME_GRID_COLUMNS] = { true, true, true, true };
        int columns;
        int i;

        if (!data) {
                return;
        }
        columns = home_grid_columns_for_window();
        if (columns == 0 || columns == data->visible_columns) {
                return;
        }
        if (columns == 3) {
                visible[2] = false;
        } else if (columns == 2) {
                visible[1] = false;
                visible[2] = false;
        }
        data->visible_columns = columns;
        for (i = 0; i < HOME_GRID_COLUMNS; ++i) {
                ui_widget_t *column = data->column_widgets[i];

                if (!column) {
                        continue;
                }
                ui_widget_set_style_string(column, "display",
                                           visible[i] ? "flex" : "none");
        }
        ui_widget_request_reflow(w);
}
static void home_grid_on_mutation(ui_mutation_list_t *records,
                                  ui_mutation_observer_t *observer, void *arg)
{
        ui_widget_t *w = arg;
        home_grid_data_t *data = home_grid_get_data(w);
        list_node_t *node;

        if (!data || data->distributing) {
                return;
        }
        for (list_each(node, records)) {
                ui_mutation_record_t *record = node->data;

                if (record->type != UI_MUTATION_RECORD_TYPE_CHILD_LIST) {
                        continue;
                }
                /* Only newly appended cards need a placement pass.  Cards that
                 * are already inside a column are not direct children. */
                if (home_grid_has_direct_cards(w)) {
                        data->pending = true;
                        ui_widget_request_update(w);
                }
                break;
        }
}

/* afterlayout is dispatched after the updater has cleared the current pass's
 * flags. Updating column styles here keeps the request alive for the next
 * pass; doing it from proto->resize during the current reflow would let
 * ui_process_mutations() clear the column's pending style update unseen. */
static void home_grid_on_root_afterlayout(ui_widget_t *w, ui_event_t *e,
                                          void *arg)
{
        home_grid_update_visibility(e->data);
}

static void home_grid_on_link(ui_widget_t *w, ui_event_t *e, void *arg)
{
        home_grid_data_t *data = home_grid_get_data(w);

        if (!data) {
                return;
        }
        /* A pre-rendered page may attach a complete grid after its first
         * mutation-observer pass. Schedule the placement now so the first
         * layout after remount already uses columns and their real width. */
        if (home_grid_has_direct_cards(w)) {
                data->pending = true;
        }
        ui_widget_request_reflow(w);
}

static void home_grid_on_update(ui_widget_t *w, ui_task_type_t task)
{
        home_grid_data_t *data = home_grid_get_data(w);

        if (!data || data->distributing || task != UI_TASK_BEFORE_UPDATE) {
                return;
        }
        if (data->pending) {
                home_grid_apply(w, data);
        }
        home_grid_update_visibility(w);
}

static void home_grid_on_destroy(ui_widget_t *w)
{
        home_grid_data_t *data = home_grid_get_data(w);

        if (ui_root()) {
                ui_widget_off(ui_root(), "afterlayout",
                              home_grid_on_root_afterlayout, w);
        }
        if (data && data->observer) {
                ui_mutation_observer_destroy(data->observer);
                data->observer = NULL;
        }
}

static void home_grid_on_init(ui_widget_t *w)
{
        ui_mutation_observer_init_t options = { .child_list = true };
        home_grid_data_t *data =
            ui_widget_add_data(w, home_grid_proto, sizeof(*data));

        memset(data, 0, sizeof(*data));
        data->observer = ui_mutation_observer_create(home_grid_on_mutation, w);
        ui_mutation_observer_observe(data->observer, w, options);
        ui_widget_on(w, "link", home_grid_on_link, w);
        ui_widget_on(ui_root(), "afterlayout", home_grid_on_root_afterlayout,
                     w);
}

void ui_register_home_grid(void)
{
        home_column_proto = ui_create_widget_prototype("home-column", NULL);
        home_grid_proto = ui_create_widget_prototype("home-grid", NULL);
        home_grid_proto->init = home_grid_on_init;
        home_grid_proto->update = home_grid_on_update;
        home_grid_proto->destroy = home_grid_on_destroy;
}

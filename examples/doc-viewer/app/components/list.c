/*
 * app/components/list.c - Implementations of <ol>, <ul> and <li>
 * widgets used by MDX-derived content in the doc-viewer.
 *
 * Each <li> creates a "marker" child of type text on init.  The marker
 * carries the bullet character or running number and is positioned
 * absolutely at the top-left of the li so it does not participate in
 * flow.  Real content goes into siblings appended after the marker by
 * the page generator (typically a single <text>).
 *
 * The parent <ol>/<ul> attaches a mutation observer on its own
 * child_list.  Whenever children are added or removed, every li gets
 * its marker text refreshed:
 *   - <ul> uses a fixed bullet ("\u2022").
 *   - <ol> uses 1-based decimal numbering.  The widest number's digit
 *     count determines a uniform padding-left applied to every li so
 *     the markers visually align.
 *
 * Units are px (LCUI does not currently support em on padding).  See
 * .opencode/skills/lcui-cli/SKILL.md.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI.h>
#include <LCUI/widgets.h>
#include <ui/mutation_observer.h>
#include <ui/prototype.h>
#include <ui/style.h>
#include <css/types.h>
#include "list.h"

typedef struct {
        ui_mutation_observer_t *observer;
        int ordered;
} list_data_t;

typedef struct {
        ui_widget_t *marker;
} li_data_t;

static ui_widget_prototype_t *ol_proto;
static ui_widget_prototype_t *ul_proto;
static ui_widget_prototype_t *li_proto;

static list_data_t *list_get_data(ui_widget_t *w)
{
        if (ui_check_widget_type(w, "ol")) {
                return ui_widget_get_data(w, ol_proto);
        }
        return ui_widget_get_data(w, ul_proto);
}

static void refresh_markers(ui_widget_t *w, int ordered)
{
        list_node_t *node;
        size_t count = 0;
        size_t index = 0;
        int digits = 1;
        size_t t;
        css_numeric_value_t padding;

        for (list_each(node, &w->children)) {
                ui_widget_t *child = node->data;
                if (ui_check_widget_type(child, "li")) {
                        count++;
                }
        }
        for (t = count; t >= 10; t /= 10) {
                digits++;
        }
        padding = (css_numeric_value_t)(8 + digits * 12);
        for (list_each(node, &w->children)) {
                ui_widget_t *child = node->data;
                li_data_t *ld;

                if (!ui_check_widget_type(child, "li")) {
                        continue;
                }
                ld = ui_widget_get_data(child, li_proto);
                if (!ld || !ld->marker) {
                        continue;
                }
                ++index;
                if (ordered) {
                        char buf[16];
                        snprintf(buf, sizeof(buf), "%zu.", index);
                        ui_text_set_content(ld->marker, buf);
                } else {
                        /* UTF-8 encoding of U+2022 BULLET */
                        ui_text_set_content(ld->marker, "\xe2\x80\xa2");
                }
                ui_widget_set_style_unit_value(child, css_prop_padding_left,
                                               padding, CSS_UNIT_PX);
        }
}

static void list_on_mutation(ui_mutation_list_t *records,
                             ui_mutation_observer_t *observer, void *arg)
{
        ui_widget_t *w = arg;
        list_data_t *that = list_get_data(w);
        list_node_t *node;

        (void)observer;
        if (!that) {
                return;
        }
        for (list_each(node, records)) {
                ui_mutation_record_t *r = node->data;
                if (r->type == UI_MUTATION_RECORD_TYPE_CHILD_LIST) {
                        refresh_markers(w, that->ordered);
                        break;
                }
        }
}

static void list_init_common(ui_widget_t *w, ui_widget_prototype_t *proto,
                             int ordered)
{
        list_data_t *that = ui_widget_add_data(w, proto, sizeof(list_data_t));
        ui_mutation_observer_init_t options = { 0 };

        that->ordered = ordered;
        that->observer = ui_mutation_observer_create(list_on_mutation, w);
        options.child_list = true;
        ui_mutation_observer_observe(that->observer, w, options);
        refresh_markers(w, ordered);
}

static void ol_init(ui_widget_t *w)
{
        list_init_common(w, ol_proto, 1);
}

static void ul_init(ui_widget_t *w)
{
        list_init_common(w, ul_proto, 0);
}

static void list_widget_destroy(ui_widget_t *w)
{
        list_data_t *that = list_get_data(w);
        if (that && that->observer) {
                ui_mutation_observer_destroy(that->observer);
                that->observer = NULL;
        }
}

static void li_init(ui_widget_t *w)
{
        li_data_t *that = ui_widget_add_data(w, li_proto, sizeof(li_data_t));
        that->marker = ui_create_widget("text");
        ui_widget_add_class(that->marker, "list-marker");
        ui_widget_prepend(w, that->marker);
}

void ui_register_list_widgets(void)
{
        ol_proto = ui_create_widget_prototype("ol", NULL);
        ol_proto->init = ol_init;
        ol_proto->destroy = list_widget_destroy;
        ul_proto = ui_create_widget_prototype("ul", NULL);
        ul_proto->init = ul_init;
        ul_proto->destroy = list_widget_destroy;
        li_proto = ui_create_widget_prototype("li", NULL);
        li_proto->init = li_init;
}

/*
 * src/widgets/collapsible.c: -- Collapsible widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <string.h>
#include <LCUI/widgets/collapsible.h>
#include <ui/css.h>
#include <ui/events.h>
#include <ui/prototype.h>

struct ui_collapsible {
        bool open;
};

static ui_widget_prototype_t *ui_collapsible_prototype;
static ui_widget_prototype_t *ui_collapsible_trigger_prototype;
static ui_widget_prototype_t *ui_collapsible_content_prototype;

static const char *ui_collapsible_css = "\
collapsible {\
  display: block;\
}\
collapsible-trigger {\
  cursor: pointer;\
}\
collapsible-content {\
  display: block;\
}\
collapsible:closed collapsible-content {\
  display: none;\
}\
";

static bool ui_collapsible_value_is_true(const char *value)
{
        return value &&
               (strcmp(value, "true") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "on") == 0 || strcmp(value, "1") == 0);
}

static ui_collapsible_t *ui_collapsible_get_data(ui_widget_t *w)
{
        return ui_widget_get_data(w, ui_collapsible_prototype);
}

static void ui_collapsible_sync(ui_widget_t *w, bool open, bool notify)
{
        ui_collapsible_t *data = ui_collapsible_get_data(w);

        if (!data || data->open == open) {
                return;
        }
        data->open = open;
        if (open) {
                ui_widget_remove_status(w, "closed");
                ui_widget_add_status(w, "open");
        } else {
                ui_widget_remove_status(w, "open");
                ui_widget_add_status(w, "closed");
        }
        if (notify) {
                ui_event_t e;

                ui_event_init(&e, open ? "open" : "close");
                ui_widget_emit_event(w, e, NULL);
        }
}

static ui_widget_t *ui_collapsible_find_owner(ui_widget_t *w)
{
        for (; w; w = w->parent) {
                if (ui_check_widget_prototype(w, ui_collapsible_prototype)) {
                        return w;
                }
        }
        return NULL;
}

static bool ui_collapsible_is_trigger(ui_widget_t *w)
{
        if (ui_check_widget_prototype(w, ui_collapsible_trigger_prototype)) {
                return true;
        }
        return ui_collapsible_value_is_true(
            ui_widget_get_attr(w, "data-collapsible-trigger"));
}

static void ui_collapsible_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *target;
        ui_widget_t *owner;

        owner = ui_collapsible_find_owner(e->target);
        if (owner != w) {
                return;
        }
        for (target = e->target; target && target != w;
             target = target->parent) {
                if (!ui_collapsible_is_trigger(target)) {
                        continue;
                }
                ui_collapsible_set_open(w, !ui_collapsible_get_open(w));
                e->cancel_bubble = true;
                return;
        }
}

static void ui_collapsible_on_init(ui_widget_t *w)
{
        ui_collapsible_t *data;

        data = ui_widget_add_data(w, ui_collapsible_prototype,
                                  sizeof(ui_collapsible_t));
        data->open = false;
        ui_widget_add_status(w, "closed");
        ui_widget_on(w, "click", ui_collapsible_on_click, NULL);
}

static void ui_collapsible_on_set_attr(ui_widget_t *w, const char *name,
                                       const char *value)
{
        if (strcmp(name, "open") == 0) {
                ui_collapsible_sync(w, ui_collapsible_value_is_true(value),
                                    true);
        } else if (strcmp(name, "default-open") == 0) {
                if (!ui_widget_get_attr(w, "open")) {
                        ui_collapsible_sync(
                            w, ui_collapsible_value_is_true(value), false);
                }
        }
}

ui_widget_t *ui_create_collapsible(void)
{
        return ui_create_widget_with_prototype(ui_collapsible_prototype);
}

ui_widget_t *ui_create_collapsible_trigger(void)
{
        return ui_create_widget_with_prototype(
            ui_collapsible_trigger_prototype);
}

ui_widget_t *ui_create_collapsible_content(void)
{
        return ui_create_widget_with_prototype(
            ui_collapsible_content_prototype);
}

void ui_collapsible_set_open(ui_widget_t *collapsible, bool open)
{
        ui_widget_set_attr(collapsible, "open", open ? "true" : "false");
}

bool ui_collapsible_get_open(ui_widget_t *collapsible)
{
        ui_collapsible_t *data = ui_collapsible_get_data(collapsible);

        return data ? data->open : false;
}

void ui_register_collapsible(void)
{
        ui_collapsible_prototype =
            ui_create_widget_prototype("collapsible", NULL);
        ui_collapsible_trigger_prototype =
            ui_create_widget_prototype("collapsible-trigger", NULL);
        ui_collapsible_content_prototype =
            ui_create_widget_prototype("collapsible-content", NULL);
        ui_collapsible_prototype->init = ui_collapsible_on_init;
        ui_collapsible_prototype->setattr = ui_collapsible_on_set_attr;
        ui_load_css_string(ui_collapsible_css, __FILE__);
}

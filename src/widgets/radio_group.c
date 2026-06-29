/*
 * src/widgets/radio_group.c: -- RadioGroup and RadioGroupItem widgets
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI/widgets/radio_group.h>
#include <LCUI/widgets.h>
#include <ui/base.h>
#include <ui/events.h>
#include <css.h>

static ui_widget_prototype_t *ui_radio_group_prototype = NULL;
static ui_widget_prototype_t *ui_radio_group_item_prototype = NULL;

static const char *ui_radio_group_css = "\
radio-group {\
  display: inline-block;\
}\
radio-group-item {\
  display: inline-flex;\
  align-items: center;\
  justify-content: center;\
  width: 16px; height: 16px;\
  border: 1px solid #ccc;\
  border-radius: 8px;\
  vertical-align: middle;\
}\
radio-group-item:checked {\
  border-color: #1e88e5;\
}\
radio-group-item .indicator {\
  display: none;\
  width: 8px; height: 8px;\
  background-color: #1e88e5; border-radius: 4px;\
}\
radio-group-item:checked .indicator { display: block; }\
radio-group-item:disabled { opacity: 0.5; }";

static bool ui_widget_is_radio_item(ui_widget_t *w)
{
        return w && w->proto == ui_radio_group_item_prototype;
}

struct radio_update_args {
        const char *target_value;
};

static void ui_radio_group_clear_each(ui_widget_t *w, void *arg)
{
        if (ui_widget_is_radio_item(w)) {
                ui_radio_group_item_set_checked(w, 0);
        }
}

static void ui_radio_group_sync_each(ui_widget_t *w, void *arg)
{
        struct radio_update_args *args = arg;
        const char *v;

        if (!ui_widget_is_radio_item(w)) {
                return;
        }
        v = ui_widget_get_attr(w, "value");
        if (args->target_value && v && strcmp(args->target_value, v) == 0) {
                ui_radio_group_item_set_checked(w, 1);
        } else {
                ui_radio_group_item_set_checked(w, 0);
        }
}

void ui_radio_group_update(ui_widget_t *w)
{
        struct radio_update_args args;

        args.target_value = ui_widget_get_attr(w, "value");
        ui_widget_each(w, ui_radio_group_sync_each, &args);
}

static void ui_radio_group_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_widget_t *target;
        const char *value;
        ui_event_t ev;

        target = e->target;
        while (target && target != w) {
                if (ui_widget_is_radio_item(target)) {
                        break;
                }
                target = target->parent;
        }
        if (!target || target == w || target->disabled) {
                return;
        }
        ui_widget_each(w, ui_radio_group_clear_each, NULL);
        ui_radio_group_item_set_checked(target, 1);
        value = ui_widget_get_attr(target, "value");
        ui_widget_set_attr(w, "value", value);
        ui_event_init(&ev, "change");
        ui_widget_emit_event(w, ev, NULL);
}

static void ui_radio_group_on_init(ui_widget_t *w)
{
        ui_widget_on(w, "click", ui_radio_group_on_click, NULL);
}

static void ui_radio_group_item_sync_status(ui_widget_t *w)
{
        if (ui_radio_group_item_get_checked(w)) {
                ui_widget_add_status(w, "checked");
        } else {
                ui_widget_remove_status(w, "checked");
        }
}

static void ui_radio_group_item_on_set_attr(ui_widget_t *w, const char *name,
                                            const char *value)
{
        if (strcmp(name, "checked") == 0) {
                ui_radio_group_item_sync_status(w);
        }
}

static void ui_radio_group_item_on_init(ui_widget_t *w)
{
        ui_widget_t *indicator;

        indicator = ui_create_widget(NULL);
        ui_widget_add_class(indicator, "indicator");
        ui_widget_append(w, indicator);
        ui_radio_group_item_sync_status(w);
}

ui_widget_t *ui_create_radio_group(void)
{
        return ui_create_widget_with_prototype(ui_radio_group_prototype);
}

ui_widget_t *ui_create_radio_group_item(void)
{
        return ui_create_widget_with_prototype(ui_radio_group_item_prototype);
}

void ui_register_radio_group(void)
{
        ui_radio_group_prototype =
            ui_create_widget_prototype("radio-group", NULL);
        ui_radio_group_prototype->init = ui_radio_group_on_init;

        ui_radio_group_item_prototype =
            ui_create_widget_prototype("radio-group-item", NULL);
        ui_radio_group_item_prototype->init = ui_radio_group_item_on_init;
        ui_radio_group_item_prototype->setattr =
            ui_radio_group_item_on_set_attr;

        ui_load_css_string(ui_radio_group_css, __FILE__);
}

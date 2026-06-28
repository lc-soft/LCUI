/*
 * src/widgets/checkbox.c: -- Checkbox widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI/widgets/checkbox.h>
#include <LCUI/widgets/text.h>
#include <LCUI/widgets.h>
#include <ui/base.h>
#include <ui/events.h>
#include <css.h>

static ui_widget_prototype_t *ui_checkbox_prototype = NULL;

static const char *ui_checkbox_css = "\
checkbox {\
  display: inline-block;\
  width: 16px;\
  height: 16px;\
  flex-shrink: 0;\
  border: 1px solid #ccc;\
  vertical-align: middle;\
  text-align: center;\
  line-height: 14px;\
}\
checkbox:checked {\
  background-color: #1e88e5;\
  color: #fff;\
  border-color: #1e88e5;\
}\
checkbox:indeterminate {\
  background-color: #1e88e5;\
  color: #fff;\
  border-color: #1e88e5;\
}\
checkbox:disabled { opacity: 0.5; }";

static void ui_checkbox_update_visual(ui_widget_t *w)
{
        const char *mark;
        int checked;
        int indeterminate;

        checked = ui_checkbox_get_checked(w);
        indeterminate = ui_checkbox_get_indeterminate(w);
        if (indeterminate) {
                mark = "\xe2\x80\x94";
        } else if (checked) {
                mark = "\xe2\x88\x9a";
        } else {
                mark = "";
        }
        ui_text_set_content(w, mark);
}

static void ui_checkbox_sync_status(ui_widget_t *w)
{
        if (ui_checkbox_get_checked(w)) {
                ui_widget_add_status(w, "checked");
        } else {
                ui_widget_remove_status(w, "checked");
        }
        if (ui_checkbox_get_indeterminate(w)) {
                ui_widget_add_status(w, "indeterminate");
        } else {
                ui_widget_remove_status(w, "indeterminate");
        }
}

static void ui_checkbox_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        ui_event_t ev;

        if (w->disabled) {
                return;
        }
        if (ui_checkbox_get_indeterminate(w)) {
                ui_checkbox_set_indeterminate(w, 0);
                ui_checkbox_set_checked(w, 1);
        } else {
                ui_checkbox_set_checked(w, !ui_checkbox_get_checked(w));
        }
        ui_event_init(&ev, "change");
        ui_widget_emit_event(w, ev, NULL);
}

static void ui_checkbox_on_set_attr(ui_widget_t *w, const char *name,
                                    const char *value)
{
        if (strcmp(name, "checked") == 0 ||
            strcmp(name, "indeterminate") == 0) {
                ui_checkbox_update_visual(w);
                ui_checkbox_sync_status(w);
        }
}

static void ui_checkbox_on_init(ui_widget_t *w)
{
        ui_checkbox_prototype->proto->init(w);
        ui_widget_on(w, "click", ui_checkbox_on_click, NULL);
        ui_checkbox_update_visual(w);
        ui_checkbox_sync_status(w);
}

ui_widget_t *ui_create_checkbox(void)
{
        return ui_create_widget("checkbox");
}

void ui_register_checkbox(void)
{
        ui_checkbox_prototype = ui_create_widget_prototype("checkbox", "text");
        ui_checkbox_prototype->init = ui_checkbox_on_init;
        ui_checkbox_prototype->setattr = ui_checkbox_on_set_attr;
        ui_load_css_string(ui_checkbox_css, __FILE__);
}

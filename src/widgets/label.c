/*
 * src/widgets/label.c: -- Label widget
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI/widgets/label.h>
#include <LCUI/widgets.h>
#include <ui/base.h>
#include <ui/events.h>
#include <css.h>

static ui_widget_prototype_t *ui_label_prototype = NULL;

static const char *ui_label_css = "label:disabled {\
opacity: 0.6;\
}";

static void ui_label_on_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        const char *for_id;
        ui_widget_t *target;
        ui_event_t forwarded;

        if (w->disabled) {
                return;
        }
        for_id = ui_widget_get_attr(w, "for");
        if (!for_id) {
                return;
        }
        target = ui_get_widget(for_id);
        if (!target) {
                return;
        }
        if (e->target == target) {
                return;
        }
        ui_event_init(&forwarded, "click");
        ui_widget_emit_event(target, forwarded, NULL);
}

static void ui_label_on_init(ui_widget_t *w)
{
        ui_label_prototype->proto->init(w);
        ui_widget_on(w, "click", ui_label_on_click, NULL);
}

ui_widget_t *ui_create_label(void)
{
        return ui_create_widget("label");
}

void ui_register_label(void)
{
        ui_label_prototype = ui_create_widget_prototype("label", "text");
        ui_label_prototype->init = ui_label_on_init;
        ui_load_css_string(ui_label_css, __FILE__);
}

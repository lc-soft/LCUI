/*
 * src/widgets/progress.c: -- Progress widget
 *
 * Copyright (c) 2026, Liu chao <lc-soft@live.cn> All rights reserved.

 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI,
 * distributed under the MIT License found in the
 * LICENSE.TXT file in the
 * root directory of this source tree.
 */

#include <stdlib.h>
#include <ui.h>
#include <css.h>
#include <yutil.h>
#include <LCUI/widgets/progress.h>

typedef struct ui_progress {
        float value;
        ui_widget_t *track;
        ui_widget_t *indicator;
} ui_progress_t;

static ui_widget_prototype_t *ui_progress_proto;
static ui_widget_prototype_t *ui_progress_track_proto;
static ui_widget_prototype_t *ui_progress_indicator_proto;

static const char *ui_progress_css = "\
progress {\
\twidth: 100%;\
\theight: 4px;\
\tbackground-color: #e0e0e0;\
}\
\
progress-track {\
\twidth: 100%;\
\theight: 100%;\
}\
\
progress-indicator {\
\theight: 100%;\
\twidth: 0;\
\tbackground-color: #4caf50;\
}\
";

static void ui_progress_update_indicator(ui_widget_t *w)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);
        float percentage;

        if (!that->track || !that->indicator) {
                return;
        }
        percentage = y_max(0.0f, y_min(100.0f, that->value));
        ui_widget_set_style_unit_value(that->indicator, css_prop_width,
                                       percentage, CSS_UNIT_PERCENT);
}

static void ui_progress_on_init(ui_widget_t *w)
{
        ui_progress_t *that;
        ui_widget_t *track;
        ui_widget_t *indicator;

        that = ui_widget_add_data(w, ui_progress_proto, sizeof(ui_progress_t));
        that->value = 0.0f;
        track = ui_create_widget("progress-track");
        indicator = ui_create_widget("progress-indicator");
        ui_widget_append(track, indicator);
        ui_widget_append(w, track);
        that->track = track;
        that->indicator = indicator;
}

void ui_progress_set_value(ui_widget_t *w, float value)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);

        that->value = y_max(0.0f, y_min(100.0f, value));
        ui_progress_update_indicator(w);
}

static void ui_progress_on_set_attr(ui_widget_t *w, const char *name,
                                    const char *value)
{
        if (strcmp(name, "value") == 0 && value) {
                ui_progress_set_value(w, (float)atof(value));
        }
}

float ui_progress_get_value(ui_widget_t *w)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);

        return that->value;
}

ui_widget_t *ui_create_progress(void)
{
        return ui_create_widget_with_prototype(ui_progress_proto);
}

void ui_register_progress(void)
{
        ui_progress_track_proto =
            ui_create_widget_prototype("progress-track", NULL);
        ui_progress_indicator_proto =
            ui_create_widget_prototype("progress-indicator", NULL);
        ui_progress_proto = ui_create_widget_prototype("progress", NULL);
        ui_progress_proto->init = ui_progress_on_init;
        ui_progress_proto->setattr = ui_progress_on_set_attr;
        ui_load_css_string(ui_progress_css, __FILE__);
}

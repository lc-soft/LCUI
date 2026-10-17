/*
 * src/widgets/progress.c: -- Progress widget
 *
 * Copyright (c) 2026, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ui.h>
#include <css.h>
#include <yutil.h>
#include <LCUI/widgets/progress.h>
#include <LCUI/widgets/text.h>

typedef struct ui_progress {
        float value;
        ui_widget_t *track;
        ui_widget_t *indicator;
        ui_mutation_observer_t *observer;
} ui_progress_t;

static ui_widget_prototype_t *ui_progress_proto;
static ui_widget_prototype_t *ui_progress_track_proto;
static ui_widget_prototype_t *ui_progress_indicator_proto;
static ui_widget_prototype_t *ui_progress_label_proto;
static ui_widget_prototype_t *ui_progress_value_proto;

static const char *ui_progress_css = "\
progress {\
\twidth: 100%;\
}\
\
progress-indicator {\
\twidth: 0;\
}\
";

static void ui_progress_update_indicator(ui_widget_t *w)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);
        ui_widget_t *child;
        char text[32];
        size_t i;
        float percentage;

        if (!that->track || !that->indicator) {
                return;
        }
        percentage = y_max(0.0f, y_min(100.0f, that->value));
        ui_widget_set_style_unit_value(that->indicator, css_prop_width,
                                       percentage, CSS_UNIT_PERCENT);
        snprintf(text, sizeof(text), "%g%%", that->value);
        for (i = 0; (child = ui_widget_get_child(w, i)); ++i) {
                if (ui_check_widget_prototype(child, ui_progress_value_proto)) {
                        ui_text_set_content(child, text);
                }
        }
}

/* The indicator must stay last in the flex line so that optional
 * progress-label / progress-value children sit above the track. Users append
 * those children after the widget is created, so move the track back to the
 * end whenever the child list changes. */
static void ui_progress_on_mutation(ui_mutation_list_t *list,
                                    ui_mutation_observer_t *observer, void *arg)
{
        ui_widget_t *w = arg;
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);
        size_t count = w->children.length;

        if (count > 0 && ui_widget_get_child(w, count - 1) != that->track) {
                ui_widget_append(w, that->track);
        }
        ui_progress_update_indicator(w);
}

static void ui_progress_on_init(ui_widget_t *w)
{
        ui_mutation_observer_init_t options = { .child_list = true };
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
        that->observer =
            ui_mutation_observer_create(ui_progress_on_mutation, w);
        ui_mutation_observer_observe(that->observer, w, options);
}

static void ui_progress_on_destroy(ui_widget_t *w)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);

        ui_mutation_observer_destroy(that->observer);
}

void ui_progress_set_value(ui_widget_t *w, float value)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);

        that->value = y_max(0.0f, y_min(100.0f, value));
        ui_progress_update_indicator(w);
}

float ui_progress_get_value(ui_widget_t *w)
{
        ui_progress_t *that = ui_widget_get_data(w, ui_progress_proto);

        return that->value;
}

static void ui_progress_on_set_attr(ui_widget_t *w, const char *name,
                                    const char *value)
{
        if (strcmp(name, "value") == 0 && value) {
                ui_progress_set_value(w, (float)atof(value));
        }
}

ui_widget_t *ui_create_progress(void)
{
        return ui_create_widget_with_prototype(ui_progress_proto);
}

ui_widget_t *ui_create_progress_label(void)
{
        return ui_create_widget_with_prototype(ui_progress_label_proto);
}

ui_widget_t *ui_create_progress_value(void)
{
        return ui_create_widget_with_prototype(ui_progress_value_proto);
}

void ui_register_progress(void)
{
        ui_progress_track_proto =
            ui_create_widget_prototype("progress-track", NULL);
        ui_progress_indicator_proto =
            ui_create_widget_prototype("progress-indicator", NULL);
        ui_progress_label_proto =
            ui_create_widget_prototype("progress-label", "text");
        ui_progress_value_proto =
            ui_create_widget_prototype("progress-value", "text");
        ui_progress_proto = ui_create_widget_prototype("progress", NULL);
        ui_progress_proto->init = ui_progress_on_init;
        ui_progress_proto->destroy = ui_progress_on_destroy;
        ui_progress_proto->setattr = ui_progress_on_set_attr;
        ui_load_css_string(ui_progress_css, __FILE__);
}

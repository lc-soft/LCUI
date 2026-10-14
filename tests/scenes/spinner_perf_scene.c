/*
 * tests/scenes/spinner_perf_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#include <math.h>
#include <string.h>
#include <LCUI.h>
#include "spinner_perf_scene.h"

static const char *spinner_perf_scene_css = "\
.perf-box {\
  position: relative;\
  width: 848px;\
  height: 632px;\
}\
.perf-scrollarea {\
  position: absolute;\
  left: 0;\
  top: 0;\
  right: 0;\
  bottom: 0;\
}\
.perf-content {\
  display: flex;\
  flex-wrap: wrap;\
  align-content: flex-start;\
}\
.perf-small {\
  width: 40px;\
  height: 40px;\
  margin: 4px;\
}\
.perf-big {\
  width: 400px;\
  height: 400px;\
  margin: 4px;\
}\
.perf-big spinner-content {\
  display: flex;\
  align-items: center;\
  justify-content: center;\
}\
.perf-big spinner-content text {\
  width: 340px;\
  white-space: normal;\
  text-align: center;\
  font-size: 20px;\
  color: #2f6fb3;\
}\
.perf-status {\
  position: absolute;\
  right: 8px;\
  top: 8px;\
  padding: 4px 8px;\
  background-color: rgba(0, 0, 0, 0.65);\
  color: #ffffff;\
  font-size: 14px;\
}\
";

static const char *spinner_perf_big_text =
    "Generating response...\n"
    "Please wait, this may take a moment.\n"
    "We are preparing your results.";

static ui_widget_t *create_big_spinner(void)
{
        ui_widget_t *spinner = ui_create_spinner();
        ui_widget_t *content = ui_create_widget("spinner-content");
        ui_widget_t *text = ui_create_widget("text");

        ui_text_set_content(text, spinner_perf_big_text);
        ui_widget_append(content, text);
        ui_widget_add_class(spinner, "perf-big");
        ui_spinner_set_content(spinner, content);
        return spinner;
}

void spinner_perf_scene_build(spinner_perf_scene_t *scene)
{
        ui_widget_t *vscrollbar;
        int i;

        memset(scene, 0, sizeof(*scene));
        ui_widget_resize(ui_root(), SPINNER_PERF_BOX_W, SPINNER_PERF_BOX_H);
        ui_load_css_string(spinner_perf_scene_css, __FILE__);

        scene->box = ui_create_widget(NULL);
        ui_widget_add_class(scene->box, "perf-box");
        ui_root_append(scene->box);

        scene->scrollarea = ui_create_scrollarea();
        ui_widget_add_class(scene->scrollarea, "perf-scrollarea");
        scene->content = ui_create_scrollarea_content();
        ui_widget_add_class(scene->content, "perf-content");

        for (i = 0; i < SPINNER_PERF_COUNT; ++i) {
                scene->spinners[i] = ui_create_spinner();
                ui_widget_add_class(scene->spinners[i], "perf-small");
                ui_widget_append(scene->content, scene->spinners[i]);
        }
        scene->count = SPINNER_PERF_COUNT;

        for (i = 0; i < SPINNER_PERF_BIG_COUNT; ++i) {
                scene->big[i] = create_big_spinner();
                ui_widget_append(scene->content, scene->big[i]);
        }

        vscrollbar = ui_create_scrollbar();
        ui_widget_append(scene->scrollarea, scene->content);
        ui_widget_append(scene->scrollarea, vscrollbar);
        ui_widget_append(scene->box, scene->scrollarea);

        scene->status = ui_create_widget("text");
        ui_widget_add_class(scene->status, "perf-status");
        ui_text_set_content(scene->status, "FPS --");
        ui_widget_append(scene->box, scene->status);
}

void spinner_perf_scene_advance(spinner_perf_scene_t *scene, double degrees)
{
        int i;

        scene->angle = fmod(scene->angle + degrees, 360.0);
        for (i = 0; i < scene->count; ++i) {
                ui_spinner_set_angle(scene->spinners[i], scene->angle);
        }
        for (i = 0; i < SPINNER_PERF_BIG_COUNT; ++i) {
                ui_spinner_set_angle(scene->big[i], scene->angle);
        }
}

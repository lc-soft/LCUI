/*
 * tests/scenes/breadcrumb_scene.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_SCENES_BREADCRUMB_SCENE_H
#define LCUI_TESTS_SCENES_BREADCRUMB_SCENE_H

#include <ui.h>

typedef struct breadcrumb_scene {
        ui_widget_t *wide;
        ui_widget_t *narrow;
        ui_widget_t *custom;
        ui_widget_t *explicit_mode;
} breadcrumb_scene_t;

void breadcrumb_scene_build(breadcrumb_scene_t *scene);

#endif /* LCUI_TESTS_SCENES_BREADCRUMB_SCENE_H */

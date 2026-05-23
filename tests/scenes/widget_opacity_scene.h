/*
 * tests/scenes/widget_opacity_scene.h
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_SCENES_WIDGET_OPACITY_SCENE_H
#define LCUI_TESTS_SCENES_WIDGET_OPACITY_SCENE_H

#include <ui.h>

typedef struct widget_opacity_scene {
        ui_widget_t *parent;
        ui_widget_t *child;
        ui_widget_t *text;
} widget_opacity_scene_t;

void widget_opacity_scene_build(widget_opacity_scene_t *scene);

#endif

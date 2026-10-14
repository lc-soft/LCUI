/*
 * tests/scenes/spinner_scene.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_SCENES_SPINNER_SCENE_H
#define LCUI_TESTS_SCENES_SPINNER_SCENE_H

#include <ui.h>

typedef struct spinner_scene {
        ui_widget_t *spinner;
        ui_widget_t *content;
} spinner_scene_t;

void spinner_scene_build(spinner_scene_t *scene);

#endif /* LCUI_TESTS_SCENES_SPINNER_SCENE_H */

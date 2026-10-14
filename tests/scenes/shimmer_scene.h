/*
 * tests/scenes/shimmer_scene.h
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef LCUI_TESTS_SCENES_SHIMMER_SCENE_H
#define LCUI_TESTS_SCENES_SHIMMER_SCENE_H

#include <ui.h>

typedef struct shimmer_scene {
        ui_widget_t *shimmer;
        ui_widget_t *content;
} shimmer_scene_t;

void shimmer_scene_build(shimmer_scene_t *scene);

#endif /* LCUI_TESTS_SCENES_SHIMMER_SCENE_H */

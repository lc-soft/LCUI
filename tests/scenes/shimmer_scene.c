/*
 * tests/scenes/shimmer_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "shimmer_scene.h"

void shimmer_scene_build(shimmer_scene_t *scene)
{
        ui_widget_t *pack, *root;

        pack = ui_load_xml_file("test_shimmer.xml");
        root = ui_root();
        ui_widget_append(root, pack);
        ui_widget_unwrap(pack);
        scene->shimmer = ui_get_widget("sh");
        scene->content = ui_get_widget("sc");
        /* The shimmer box size is set in code (not via an ID CSS rule) so that
         * tests can resize it with ui_widget_resize without cascade fights,
         * and so the preview has a visible default box. */
        ui_widget_resize(scene->shimmer, 80, 80);
}

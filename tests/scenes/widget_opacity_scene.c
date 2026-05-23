/*
 * tests/scenes/widget_opacity_scene.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "widget_opacity_scene.h"

void widget_opacity_scene_build(widget_opacity_scene_t *scene)
{
        ui_widget_t *pack, *root;

        pack = ui_load_xml_file("test_widget_opacity.xml");
        root = ui_root();
        ui_widget_append(root, pack);
        ui_widget_unwrap(pack);
        scene->parent = ui_get_widget("parent");
        scene->child = ui_get_widget("child");
        scene->text = ui_get_widget("current-opacity");
}

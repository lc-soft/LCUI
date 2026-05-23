/*
 * tests/scenes/block_layout_scene.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "block_layout_scene.h"

void block_layout_scene_build(void)
{
        ui_widget_t *root;
        ui_widget_t *wrapper;

        wrapper = ui_load_xml_file("test_block_layout.xml");
        root = ui_root();
        ui_widget_append(root, wrapper);
        ui_widget_unwrap(wrapper);
        ui_update();
}

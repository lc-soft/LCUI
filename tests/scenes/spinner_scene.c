/*
 * tests/scenes/spinner_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "spinner_scene.h"

void spinner_scene_build(spinner_scene_t *scene)
{
        ui_widget_t *pack, *root;

        pack = ui_load_xml_file("test_spinner.xml");
        root = ui_root();
        ui_widget_append(root, pack);
        ui_widget_unwrap(pack);
        scene->spinner = ui_get_widget("sp");
        scene->content = ui_get_widget("sc");
        /* The spinner box size is set in code (not via an ID CSS rule) so that
         * tests can resize it with ui_widget_resize without cascade fights,
         * and so the preview has a visible default box. */
        ui_widget_resize(scene->spinner, 40, 40);
}

/*
 * tests/scenes/progress_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "progress_scene.h"

static const char *progress_scene_css = "\
.progress {\
	width: 200px;\
	margin: 20px;\
}\
";

void progress_scene_build(void)
{
        ui_widget_t *progress = ui_create_progress();

        ui_widget_resize(ui_root(), 800, 200);
        ui_load_css_string(progress_scene_css, __FILE__);
        ui_widget_set_id(progress, "main");
        ui_widget_add_class(progress, "progress");
        ui_root_append(progress);
}

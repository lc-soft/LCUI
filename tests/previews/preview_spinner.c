/*
 * tests/previews/preview_spinner.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include "preview_window.h"
#include <ptk/main.h>
#include "../scenes/spinner_scene.h"

int main(int argc, char **argv)
{
        spinner_scene_t scene;

        lcui_init();
        spinner_scene_build(&scene);
        ui_widget_set_title(ui_root(), L"spinner preview");
        preview_show_default_window();
        return lcui_main();
}

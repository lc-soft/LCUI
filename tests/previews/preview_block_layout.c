/*
 * tests/previews/preview_block_layout.c
 *
 * Copyright (c) 2026, Liu Chao <i@lc-soft.io> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <LCUI.h>
#include <ptk/main.h>
#include "../scenes/block_layout_scene.h"

int main(int argc, char **argv)
{
        (void)argc;
        (void)argv;
        lcui_init();
        ui_widget_set_title(ui_root(), L"block layout preview");
        block_layout_scene_build();
        return lcui_main();
}

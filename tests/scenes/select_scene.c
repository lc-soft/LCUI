/*
 * tests/scenes/select_scene.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <LCUI.h>
#include "select_scene.h"

void select_scene_build(ui_widget_t *parent)
{
        ui_widget_t *select = ui_create_select();

        ui_select_set_placeholder(select, "Choose a theme");
        ui_select_add_item(select, "Light", "light");
        ui_select_add_item(select, "Dark", "dark");
        ui_select_add_item(select, "System", "system");
        ui_widget_append(parent, select);
}

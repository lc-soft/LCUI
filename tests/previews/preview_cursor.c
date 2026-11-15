/*
 * tests/previews/preview_cursor.c
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdio.h>
#include <LCUI.h>
#include "preview_window.h"
#include <ptk/main.h>
#include "../scenes/cursor_scene.h"

static const char *cursor_value_names[] = {
        "auto",        "default",     "none",      "context-menu",
        "help",        "pointer",     "progress",  "wait",
        "cell",        "crosshair",   "text",      "vertical-text",
        "alias",       "copy",        "move",      "no-drop",
        "not-allowed", "grab",        "grabbing",  "all-scroll",
        "col-resize",  "row-resize",  "n-resize",  "e-resize",
        "s-resize",    "w-resize",    "ew-resize", "ns-resize",
        "ne-resize",   "nw-resize",   "se-resize", "sw-resize",
        "nesw-resize", "nwse-resize", "zoom-in",   "zoom-out",
};

/* 36 宫格：每格绝对定位并设置 cursor:<值>，人工悬停核对平台观感 */
static void build_cursor_grid(float left0, float top0)
{
        size_t i;
        char value[64];

        for (i = 0;
             i < sizeof(cursor_value_names) / sizeof(cursor_value_names[0]);
             ++i) {
                ui_widget_t *cell = ui_create_widget(NULL);
                ui_widget_t *label = ui_create_widget("text");
                float left = left0 + (float)(i % 6) * 118.f;
                float top = top0 + (float)(i / 6) * 42.f;

                ui_text_set_content(label, cursor_value_names[i]);
                ui_widget_set_style_string(cell, "position", "absolute");
                snprintf(value, 64, "%.0fpx", left);
                ui_widget_set_style_string(cell, "left", value);
                snprintf(value, 64, "%.0fpx", top);
                ui_widget_set_style_string(cell, "top", value);
                ui_widget_set_style_string(cell, "width", "112px");
                ui_widget_set_style_string(cell, "height", "36px");
                ui_widget_set_style_string(cell, "cursor",
                                           cursor_value_names[i]);
                ui_widget_append(cell, label);
                ui_root_append(cell);
        }
}

int main(int argc, char **argv)
{
        lcui_init();
        ui_widget_set_title(ui_root(), L"cursor preview");
        cursor_scene_build();
        build_cursor_grid(0.f, 160.f);
        ui_widget_resize(ui_root(), 720, 420);
        preview_show_default_window();
        return lcui_main();
}

/*
 * tests/scenes/cursor_scene.c
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
#include "cursor_scene.h"

static const char *cursor_scene_css = "\
.hov:hover {\
	cursor: crosshair;\
}\
";

/* 布局（root 320x400，绝对定位，坐标与测试用例硬编码一致）：
 * tile-plain    (0,0)     120x40  无 cursor，测回写 auto
 * tile-pointer  (130,0)   120x40  cursor: pointer
 *   tile-inherit (0,0)     60x20  未设置，测继承
 *   tile-default (60,0)    60x20  cursor: default，测截断
 *   底部空白     (0,20)   120x20  测自身显式值
 * tile-hover    (0,50)    120x40  经 .hov:hover 规则变 crosshair
 * tile-none     (130,50)  120x40  cursor: none
 * widget-button (0,100)   120x40  真实 button，测主题默认 pointer
 * widget-textinput (130,100) 120x40  真实输入框，测默认 text
 */
static void set_box_style(ui_widget_t *w, int left, int top, int width,
                          int height)
{
        char value[64];

        snprintf(value, 64, "%dpx", left);
        ui_widget_set_style_string(w, "left", value);
        snprintf(value, 64, "%dpx", top);
        ui_widget_set_style_string(w, "top", value);
        snprintf(value, 64, "%dpx", width);
        ui_widget_set_style_string(w, "width", value);
        snprintf(value, 64, "%dpx", height);
        ui_widget_set_style_string(w, "height", value);
        ui_widget_set_style_string(w, "position", "absolute");
}

static ui_widget_t *make_tile(const char *id, int left, int top, int width,
                              int height, const char *cursor,
                              const char *class_name)
{
        ui_widget_t *w = ui_create_widget(NULL);

        ui_widget_set_id(w, id);
        if (class_name) {
                ui_widget_add_class(w, class_name);
        }
        if (cursor) {
                ui_widget_set_style_string(w, "cursor", cursor);
        }
        set_box_style(w, left, top, width, height);
        return w;
}

void cursor_scene_build(void)
{
        ui_widget_t *button;
        ui_widget_t *input;
        ui_widget_t *pointer_tile;

        ui_widget_resize(ui_root(), 320, 400);
        ui_load_css_string(cursor_scene_css, __FILE__);

        ui_root_append(make_tile("tile-plain", 0, 0, 120, 40, NULL, NULL));

        pointer_tile =
            make_tile("tile-pointer", 130, 0, 120, 40, "pointer", NULL);
        ui_widget_append(pointer_tile,
                         make_tile("tile-inherit", 0, 0, 60, 20, NULL, NULL));
        ui_widget_append(pointer_tile, make_tile("tile-default", 60, 0, 60, 20,
                                                 "default", NULL));
        ui_root_append(pointer_tile);

        ui_root_append(make_tile("tile-hover", 0, 50, 120, 40, NULL, "hov"));
        ui_root_append(make_tile("tile-none", 130, 50, 120, 40, "none", NULL));

        button = ui_create_button();
        ui_widget_set_id(button, "widget-button");
        ui_text_set_content_w(button, L"OK");
        set_box_style(button, 0, 100, 120, 40);
        ui_root_append(button);

        input = ui_create_widget("textinput");
        ui_widget_set_id(input, "widget-textinput");
        ui_textinput_set_text_w(input, L"input");
        set_box_style(input, 130, 100, 120, 40);
        ui_root_append(input);
}

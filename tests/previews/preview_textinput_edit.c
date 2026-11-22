/*
 * tests/previews/preview_textinput_edit.c
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

static ui_widget_t *add_input(const char *left, const char *top,
                              const char *width, const char *height)
{
        ui_widget_t *input = ui_create_widget("textinput");

        ui_widget_set_style_string(input, "position", "absolute");
        ui_widget_set_style_string(input, "left", left);
        ui_widget_set_style_string(input, "top", top);
        ui_widget_set_style_string(input, "width", width);
        ui_widget_set_style_string(input, "height", height);
        ui_root_append(input);
        return input;
}

int main(int argc, char **argv)
{
        ui_widget_t *input;

        lcui_init();
        ui_widget_set_title(ui_root(), L"textinput edit preview");

        input = add_input("20px", "20px", "330px", "32px");
        ui_textinput_set_placeholder(input, "Single line, try Shift+Arrows");
        ui_textinput_set_text(input, "Hello world");

        input = add_input("20px", "70px", "330px", "32px");
        ui_textinput_set_password_char(input, L'*');
        ui_textinput_set_text(input, "secret");

        input = add_input("20px", "120px", "330px", "32px");
        ui_textinput_set_read_only(input, true);
        ui_textinput_set_text(input, "Read-only, still selectable");

        input = add_input("20px", "170px", "330px", "110px");
        ui_textinput_enable_multiline(input, true);
        ui_textinput_set_text_w(
            input,
            L"Line one\n"
            L"Line two with a longer sentence that should wrap inside the "
            L"box when it exceeds the available width.\n"
            L"Line three");

        ui_widget_resize(ui_root(), 380, 310);
        preview_show_default_window();
        return lcui_main();
}

/*
 * src/widgets/button.c: -- Button widget
 *
 * Copyright (c) 2018-2025, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found in the
 * LICENSE.TXT file in the root directory of this source tree.
 */

#include <stdlib.h>
#include <string.h>
#include <ui.h>
#include <css.h>
#include <LCUI/widgets/button.h>
#include <LCUI/widgets/text.h>

static ui_widget_prototype_t *ui_button_prototype = NULL;

static const char *ui_button_css = css_string(

button {
	padding: 5px 10px;
	text-align: center;
	display: inline-block;
	background-color: #fff;
	border: 1px solid #eaeaea;
}

button:hover {
	border-color: #cbcbcb;
	background-color: #e6e6e6;
}

button:active {
	background-color: #d7d7d7;
}

button:disabled {
	color: #aaa;
}

);

static void ui_button_on_init(ui_widget_t* w)
{
	ui_button_prototype->proto->init(w);
	w->tab_index = 0;
}

void ui_button_set_text_w(ui_widget_t* w, const wchar_t *wstr)
{
	ui_text_set_content_w(w, wstr);
}

void ui_button_set_text(ui_widget_t* w, const char *str)
{
	ui_text_set_content(w, str);
}

void ui_register_button(void)
{
	ui_button_prototype = ui_create_widget_prototype("button", "text");
	ui_button_prototype->init = ui_button_on_init;
	ui_load_css_string(ui_button_css, __FILE__);
}

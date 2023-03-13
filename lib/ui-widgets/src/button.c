/*
 * button.c -- Button widget
 *
 * Copyright (c) 2018-2022, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <LCUI/ui.h>
#include <css.h>
#include "../include/button.h"
#include "../include/textview.h"

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
	ui_textview_set_text_w(w, wstr);
}

void ui_button_set_text(ui_widget_t* w, const char *str)
{
	ui_textview_set_text(w, str);
}

void ui_register_button(void)
{
	ui_button_prototype = ui_create_widget_prototype("button", "textview");
	ui_button_prototype->init = ui_button_on_init;
	ui_load_css_string(ui_button_css, __FILE__);
}

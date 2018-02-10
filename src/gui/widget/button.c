/*
 * button.c -- LCUI‘s Button widget
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
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
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/gui/css_parser.h>

static LCUI_WidgetPrototype prototype = NULL;

/** 按钮的 css 样式 */
static const char *button_css = CodeToString(

button {
	padding: 6px 12px;
	focusable: true;
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

static void Button_OnInit( LCUI_Widget w )
{
	prototype->proto->init( w );
	w->computed_style.focusable = TRUE;
}

void Button_SetTextW( LCUI_Widget w, const wchar_t *wstr )
{
	TextView_SetTextW( w, wstr );
}

void Button_SetText( LCUI_Widget w, const char *str )
{
	TextView_SetText( w, str );
}

void LCUIWidget_AddButton( void )
{
	prototype = LCUIWidget_NewPrototype( "button", "textview" );
	prototype->init = Button_OnInit;
	LCUI_LoadCSSString( button_css, __FILE__ );
}

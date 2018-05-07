/*
 * textcaret.c -- textcaret widget, used in textedit.
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

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textcaret.h>
#include <LCUI/gui/css_parser.h>

#define CARET_HIDE 0
#define CARET_SHOW 1

/** 文本插入符相关数据 */
typedef struct LCUI_TextCaretRec_ {
	int type;
	int state;		/**< 当前状态 */
	int blink_time;		/**< 闪烁间隔 */
	int timer_id;		/**< 定时器 */
	LCUI_BOOL need_show;
} LCUI_TextCaretRec, *LCUI_TextCaret;

static LCUI_WidgetPrototype prototype = NULL;

static const char *textcaret_css = CodeToString(

textcaret {
	pointer-events: none;
	focusable: false;
	width: 1px;
	height:14px;
	position: absolute;
	background-color: #000;
}

);

void TextCaret_BlinkShow( LCUI_Widget widget )
{
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	if( !caret->need_show ) {
		return;
	}
	caret->state = CARET_SHOW;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
	Widget_Show( widget );
}

void TextCaret_BlinkHide( LCUI_Widget widget )
{
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	caret->state = CARET_HIDE;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
	Widget_Hide( widget );
}

static void TextCaret_OnBlink( void *arg )
{
	LCUI_Widget widget = arg;
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	if( caret->need_show ) {
		if( caret->state == CARET_HIDE ) {
			caret->state = CARET_SHOW;
			Widget_Show( widget );
		} else {
			caret->state = CARET_HIDE;
			Widget_Hide( widget );
		}
	}
}

void TextCaret_SetVisible( LCUI_Widget widget, LCUI_BOOL visible )
{
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	caret->need_show = visible;
	if( caret->need_show ) {
		TextCaret_BlinkShow( widget );
	} else {
		TextCaret_BlinkHide( widget );
	}
}

static void TextCaret_OnInit( LCUI_Widget widget )
{
	LCUI_TextCaret caret;
	const size_t data_size = sizeof( LCUI_TextCaretRec );
	caret = Widget_AddData( widget, prototype, data_size );
	caret->type = 0;
	caret->need_show = FALSE;
	caret->state = CARET_HIDE;
	caret->blink_time = 500;
	caret->timer_id = LCUITimer_Set( caret->blink_time, 
					 TextCaret_OnBlink, widget, TRUE );
}

void TextCaret_SetBlinkTime( LCUI_Widget widget, unsigned int n_ms )
{
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	caret->blink_time = n_ms;
	LCUITimer_Reset( caret->timer_id, caret->blink_time );
}

static void TextCaret_OnDestroy( LCUI_Widget widget )
{
	LCUI_TextCaret caret = Widget_GetData( widget, prototype );
	LCUITimer_Free( caret->timer_id );
	caret->timer_id = -1;
}

void LCUIWidget_AddTextCaret( void )
{
	prototype = LCUIWidget_NewPrototype( "textcaret", NULL );
	prototype->init = TextCaret_OnInit;
	prototype->destroy = TextCaret_OnDestroy;
	LCUI_LoadCSSString( textcaret_css, __FILE__ );
}

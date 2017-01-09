/* ***************************************************************************
 * textcaret.c -- textcaret widget, used in textedit.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * textcaret.c -- 文本光标，主要用于文本编辑框中提示当前文本插入点。
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textcaret.h>

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
	LCUI_LoadCSSString( textcaret_css, NULL );
}

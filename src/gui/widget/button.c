/* ***************************************************************************
 * button.c -- LCUI‘s Button widget
 *
 * Copyright (C) 2015-2016 by Liu Chao <lc-soft@live.cn>
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
 * button.c -- LCUI 的按钮部件
 *
 * 版权所有 (C) 2015-2016 归属于 刘超 <lc-soft@live.cn>
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

#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>

typedef struct LCUI_ButtonRec_ {
	LCUI_Widget text;
} LCUI_ButtonRec, *LCUI_Button;

static LCUI_WidgetPrototype prototype = NULL;

/** 按钮的 css 样式 */
static const char *button_css = CodeToString(

button {
	padding: 6px 12px;
	focusable: true;
	border: 1px solid #eaeaea;
	background-color: #fff;
}

button:hover {
	border-color: #cbcbcb;
	background-color: #e6e6e6;
}

button:active {
	background-color: #d7d7d7;
}

button:disabled textview {
	color: #aaa;
}

);

static void Button_OnInit( LCUI_Widget w )
{
	const size_t data_size = sizeof( LCUI_ButtonRec );
	LCUI_Button btn = Widget_AddData( w, prototype, data_size );
	btn->text = LCUIWidget_New( "textview" );
	w->computed_style.focusable = TRUE;
	TextView_SetTextAlign( btn->text, SV_CENTER );
	Widget_Append( w, btn->text );
	Widget_Show( btn->text );
}

void Button_SetTextW( LCUI_Widget w, const wchar_t *wstr )
{
	LCUI_Button btn = Widget_GetData( w, prototype );
	TextView_SetTextW( btn->text, wstr );
}

void Button_SetText( LCUI_Widget w, const char *str )
{
	LCUI_Button btn = Widget_GetData( w, prototype );
	TextView_SetText( btn->text, str );
}

/** 添加按钮部件类型 */
void LCUIWidget_AddButton( void )
{
	prototype = LCUIWidget_NewPrototype( "button", NULL );
	prototype->init = Button_OnInit;
	prototype->settext = Button_SetText;
	LCUI_LoadCSSString( button_css, NULL );
}

/* ***************************************************************************
 * button.c -- LCUI‘s Button widget
 * 
 * Copyright (C) 2012-2015 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2012-2015 归属于 刘超 <lc-soft@live.cn>
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
#include <LCUI/font.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>

typedef struct LCUI_Button {
	LCUI_Widget text;
} LCUI_Button;

static void Button_OnInit( LCUI_Widget w )
{
	LCUI_Button *btn;
	btn = Widget_NewPrivateData( w, LCUI_Button );
	btn->text = LCUIWidget_New("textview");
	TextView_SetTextAlign( btn->text, SV_CENTER );
	Widget_Append( w, btn->text );
	Widget_Show( btn->text );
}

static void Button_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{

}

static void Button_OnDestroy( LCUI_Widget w )
{

}

static void Button_OnTask( LCUI_Widget w )
{

}

void Button_SetTextW( LCUI_Widget w, const wchar_t *wstr )
{
	LCUI_Button *btn = (LCUI_Button*)w->private_data;
	TextView_SetTextW( btn->text, wstr );
}

void Button_SetText( LCUI_Widget w, const char *str )
{
	LCUI_Button *btn = (LCUI_Button*)w->private_data;
	TextView_SetText( btn->text, str );
}

/** 按钮的 css 样式 */
static const char *button_css = ToString(

button {
	padding: 6px 12px;
	border: 1px solid #eaeaea;
	background-color: #fff;
}

button:hover {
	border-color: #cbcbcb;
	background-color: #e6e6e6;
}

button:active {
	background-color:rgb(215,215,215);
}

);

/** 添加按钮部件类型 */
void LCUIWidget_AddButton( void )
{
	LCUI_WidgetClass *wc = LCUIWidget_AddClass( "button" );
	wc->methods.init = Button_OnInit;
	wc->methods.paint = Button_OnPaint;
	wc->methods.destroy = Button_OnDestroy;
	wc->task_handler = Button_OnTask;
	LCUICSS_LoadString( button_css );
}

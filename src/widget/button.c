/* ***************************************************************************
 * button.c -- LCUI‘s Button widget
 * 
 * Copyright (C) 2012-2013 by
 * Liu Chao
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
 * 版权所有 (C) 2013 归属于
 * 刘超
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_BUTTON_H
#include LC_GRAPH_H
#include LC_DRAW_H

static void Button_ExecDefalutUpdate( LCUI_Widget *widget )
{
	LCUI_Border border;
	LCUI_RGB border_color;

	switch( widget->state ) {
	case WIDGET_STATE_NORMAL:
		Widget_SetBackgroundColor( widget, RGB(230,230,230) );
		/* 获得焦点的按钮，使用另一种颜色的边框 */
		if( Widget_GetFocus(widget) ) {
			border_color = RGB(50,150,255);
		} else {
			border_color = RGB(172,172,172);
		}
		break;
	case WIDGET_STATE_OVERLAY :
		Widget_SetBackgroundColor( widget, RGB(232,242,252) );
		border_color = RGB(126,180,234);
		break;
	case WIDGET_STATE_ACTIVE :
		Widget_SetBackgroundColor( widget, RGB(207,230,252) );
		border_color = RGB(86,157,229);
		break;
	case WIDGET_STATE_DISABLE :
		Widget_SetBackgroundColor( widget, RGB(200,200,200) );
		border_color = RGB(172,172,172);
		break;
		default : break;
	}
	border = Border(1, BORDER_STYLE_SOLID, border_color );
	Widget_SetBorder( widget, border );
	Widget_SetBackgroundTransparent( widget, FALSE );
}

static void Button_ExecCustomUpdate( LCUI_Widget *widget )
{
	LCUI_Button *btn;
	LCUI_Graph *img;
	
	btn = Widget_GetPrivData( widget );
	switch(widget->state) {
	case WIDGET_STATE_NORMAL: img = &btn->btn_normal; break;
	case WIDGET_STATE_OVERLAY: img = &btn->btn_over; break;
	case WIDGET_STATE_ACTIVE: img = &btn->btn_down; break;
	case WIDGET_STATE_DISABLE: img = &btn->btn_disable; break;
	default : img = NULL; break;
	}
	Widget_SetBackgroundImage( widget, img );
	/* 如果图像不可用，则使用默认样式 */
	if( !Graph_IsValid(img) ) {
		Button_ExecDefalutUpdate( widget );
	} else {
		Widget_SetBackgroundTransparent( widget, TRUE );
		Widget_SetBackgroundLayout( widget, LAYOUT_STRETCH );
	}
}

static void Button_ExecUpdate( LCUI_Widget *widget )
{
	if(widget->style_id == BUTTON_STYLE_CUSTOM) {
		Button_ExecCustomUpdate( widget );
	} else {
		Button_ExecDefalutUpdate( widget );
	}
	Widget_Refresh( widget );
}

static void 
Button_ProcFocusOut( LCUI_Widget *widget, LCUI_WidgetEvent *unused )
{
	Widget_Update( widget );
}


/* 初始化按钮部件的数据 */
static void
Button_Init( LCUI_Widget *widget )
{
	int valid_state;
	LCUI_Button *button;
	
	button = WidgetPrivData_New(widget, sizeof(LCUI_Button));
	/* 初始化图像数据 */ 
	Graph_Init(&button->btn_disable);
	Graph_Init(&button->btn_normal);
	Graph_Init(&button->btn_focus);
	Graph_Init(&button->btn_down);
	Graph_Init(&button->btn_over);
	
	valid_state = (WIDGET_STATE_NORMAL | WIDGET_STATE_ACTIVE);
	valid_state |= (WIDGET_STATE_DISABLE | WIDGET_STATE_OVERLAY);
	Widget_SetValidState( widget, valid_state );
	button->label = Widget_New("label");/* 创建label部件 */ 
	/* 将按钮部件作为label部件的容器 */
	Widget_Container_Add(widget, button->label);
	/* label部件居中显示 */
	Widget_SetAlign(button->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Widget_Show(button->label); /* 显示label部件 */
	/* 启用自动尺寸调整，以适应内容 */
	Widget_SetAutoSize( widget, TRUE, AUTOSIZE_MODE_GROW_AND_SHRINK);
	Widget_SetStyleID( widget, BUTTON_STYLE_DEFAULT );
	/* 关联EVENT_FOCUSOUT事件，以在按钮失去焦点时重绘按钮 */
	Widget_Event_Connect( widget, EVENT_FOCUSOUT, Button_ProcFocusOut );
}

/* 获取嵌套在按钮部件里的label部件 */
LCUI_API LCUI_Widget*
Button_GetLabel( LCUI_Widget *widget )
{
	LCUI_Button *button = (LCUI_Button*)Widget_GetPrivData(widget);
	return button->label;
}

/* 自定义按钮在各种状态下显示的位图 */
LCUI_API void
Button_CustomStyle(	LCUI_Widget *widget, LCUI_Graph *normal, 
			LCUI_Graph *over, LCUI_Graph *down, 
			LCUI_Graph *focus, LCUI_Graph *disable)
{
	LCUI_Button *btn_data;
	btn_data = Widget_GetPrivData(widget);
	if( Graph_IsValid(normal) ) {
		btn_data->btn_normal = *normal;
	} else {
		Graph_Init( &btn_data->btn_normal );
	}
	if( Graph_IsValid(over) ) {
		btn_data->btn_over = *over;
	} else {
		Graph_Init( &btn_data->btn_over );
	}
	if( Graph_IsValid(down) ) {
		btn_data->btn_down = *down;
	} else {
		Graph_Init( &btn_data->btn_down );
	}
	if( Graph_IsValid(focus) ) {
		btn_data->btn_focus = *focus;
	} else {
		Graph_Init( &btn_data->btn_focus );
	}
	if( Graph_IsValid(disable) ) {
		btn_data->btn_disable = *disable;
	} else {
		Graph_Init( &btn_data->btn_disable );
	}
	/* 设定为自定义风格 */
	Widget_SetStyleID(widget, BUTTON_STYLE_CUSTOM);
	Widget_Draw(widget); /* 重新绘制部件 */
}

/* 设定按钮部件显示的文本内容 */
LCUI_API void
Button_Text( LCUI_Widget *widget, const char *text )
{
	LCUI_Button *button;
	LCUI_Widget *label;
	
	button = (LCUI_Button*)Widget_GetPrivData(widget);
	label = button->label;
	/* 设定部件显示的文本 */
	Label_Text( label, text );
}

LCUI_API void
Button_TextW( LCUI_Widget *widget, const wchar_t *text )
{
	LCUI_Button *button;
	LCUI_Widget *label;
	
	button = (LCUI_Button*)Widget_GetPrivData(widget);
	label = button->label;
	Label_TextW( label, text );
}

/* 创建一个带文本内容的按钮 */
LCUI_API LCUI_Widget*
Button_New( const char *text )
{
	LCUI_Widget *widget;
	widget = Widget_New("button");
	Button_Text(widget, text);
	return widget;
}

/* 将按钮部件类型注册至部件库 */
LCUI_API void
Register_Button(void)
{
	/* 添加部件类型 */
	WidgetType_Add("button");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("button", Button_Init,		FUNC_TYPE_INIT);
	WidgetFunc_Add("button", Button_ExecUpdate,	FUNC_TYPE_UPDATE);
}

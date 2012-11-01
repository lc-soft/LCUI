/* ***************************************************************************
 * LCUI_ScrollBar.c -- LCUI's ScrollBar widget
 * 
 * Copyright (C) 2012 by
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
 * LCUI_Window.c -- LCUI 的滚动条部件
 *
 * 版权所有 (C) 2012 归属于 
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

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_GRAPH_H

typedef struct _LCUI_ScrollBar
{
	/* 这两个控制滑块的位置 */
	int max_num;
	int current_num;
	/* 这两个控制滑块的宽度 */
	int max_size;
	int current_size;
	int direction;
	LCUI_Widget *widget;	/* 滑块 */
}
LCUI_ScrollBar;


/**************************** 私有函数 **********************************/
static void 
ScrollBar_Drag( LCUI_Widget *widget, LCUI_DragEvent *event )
{
	LCUI_Pos pos, offset;
	
	pos = Get_Widget_Global_Pos( widget );
	offset = Pos_Sub( event->new_pos, pos ); 
	pos = Pos_Add( pos, offset ); 
	pos = GlobalPos_ConvTo_RelativePos( widget, pos ); 
	Move_Widget(widget, pos);
}

/*
 * 创建一个部件，作为滚动条的容器
 * 再创建一个部件，作为滚动条
 * 限制滚动条的移动范围
 * */
static void 
ScrollBar_Init( LCUI_Widget *widget )
{
	LCUI_ScrollBar *scrollbar;
	scrollbar = Widget_Create_PrivData(widget, sizeof(LCUI_ScrollBar));
	scrollbar->max_num = 100;
	scrollbar->current_num = 100;
	scrollbar->max_size = 100;
	scrollbar->current_size = 100;
	scrollbar->direction = 0;
	
	scrollbar->widget = Create_Widget("button");
	Widget_Container_Add( widget, scrollbar->widget );
	Set_Widget_Size( scrollbar->widget, "100%", "100%" );
	Limit_Widget_Pos( scrollbar->widget, Pos(0,0), Pos(0,0) ); 
	Set_Widget_Border( widget, RGB(100,100,100), Border(1,1,1,1) );
	Set_Widget_Padding( widget, Padding(1,1,1,1) );
	Show_Widget( scrollbar->widget );
	Widget_Drag_Event_Connect( scrollbar->widget, ScrollBar_Drag );
}

void 
ScrollBar_Update( LCUI_Widget *widget )
{
	static int pos;
	static double scale;
	static char scale_str[256];
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Get_Widget_PrivData( widget );
	/* 计算比例，之后转换成字符串 */
	scale = scrollbar->current_size*1.0 / scrollbar->max_size; 
	sprintf( scale_str, "%.2lf%%", scale*100 );
	//printf( "scale: %s\n", scale_str );
	/* 判断滚动条的方向 */
	if( scrollbar->direction == 1 ) { /* 横向 */
		Set_Widget_Size( scrollbar->widget, NULL, scale_str );
	} else { /* 纵向 */
		Set_Widget_Size( scrollbar->widget, NULL, scale_str );
		pos = Get_Container_Height( widget ) - _Get_Widget_Height( scrollbar->widget ); 
		Limit_Widget_Pos( scrollbar->widget, Pos(0,0), Pos( 0, pos ) );
	}
}

void 
Register_ScrollBar()
{
	WidgetType_Add( "scrollbar" );
	WidgetFunc_Add( "scrollbar", ScrollBar_Init, FUNC_TYPE_INIT );
	WidgetFunc_Add( "scrollbar", ScrollBar_Update, FUNC_TYPE_UPDATE );
}
/**********************************************************************/


/**************************** 公共函数 **********************************/
LCUI_Widget *
Get_ScrollBar( LCUI_Widget *widget )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Get_Widget_PrivData( widget );
	return scrollbar->widget;
}

void 
ScrollBar_Set_MaxNum( LCUI_Widget *widget, int max_num )
{
	
}

void 
ScrollBar_Set_MaxSize( LCUI_Widget *widget, int max_size )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Get_Widget_PrivData( widget );
	scrollbar->max_size = max_size;
	Update_Widget( widget );
}

void 
ScrollBar_Set_CurrentNum( LCUI_Widget *widget, int current_num )
{
	
}

void 
ScrollBar_Set_CurrentSize( LCUI_Widget *widget, int current_size )
{
	
}

/* 设置滚动条是横向移动还是纵向移动 */
void 
ScrollBar_Set_Direction( LCUI_Widget *widget, int direction )
{
	if(direction) {
		
	}
}
/**********************************************************************/

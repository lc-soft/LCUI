/* ***************************************************************************
 * scrollbar.c -- LCUI's ScrollBar widget
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
 * scrollbar.c -- LCUI 的滚动条部件
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
#include LC_DRAW_H
#include LC_SCROLLBAR_H

typedef struct _LCUI_ScrollBar
{
	int direction;
	ScrollBar_Data data;
	
	/* 与滚动条部件连接的回调函数，当滚动条移动时，会调用它 */
	void (*callback_func)( ScrollBar_Data, void * );
	void *arg;
	
	LCUI_Widget *widget;	/* 滑块 */
}
LCUI_ScrollBar;


/**************************** 私有函数 **********************************/
static void
Move_ScrollBar( LCUI_Widget *widget, LCUI_Pos new_pos )
{
	double scale;
	int size;
	LCUI_ScrollBar *scrollbar;
	//_DEBUG_MSG("new_pos: %d,%d\n", new_pos.x, new_pos.y);
	scrollbar = Widget_GetPrivData( widget->parent );
	/* 使该坐标在限制范围内 */
	new_pos = Widget_GetValidPos( widget, new_pos );
	/* 判断滚动条的方向 */
	if( scrollbar->direction == 0 ) { /* 纵向 */
		/* 先得到容器高度 */
		size = Widget_GetContainerHeight( widget->parent );
		//_DEBUG_MSG("1,size: %d\n", size);
		/* 减去自身高度 */
		size -= _Widget_GetHeight( widget );
		//_DEBUG_MSG("2,size: %d\n", size);
		/* 再用当前坐标除以size，得到比例 */
		scale = new_pos.y * 1.0 / size;
		//_DEBUG_MSG("%.2f = %d * 1.0 / %d\n", scale, new_pos.y, size);
		/* 将max_num乘以比例，得出current_num的值 */
		scrollbar->data.current_num = scale * scrollbar->data.max_num;
		//_DEBUG_MSG("current_num: %d, scale: %.2f, max_num: %d\n", 
		//scrollbar->data.current_num, scale, scrollbar->data.max_num);
	} else { /* 横向 */
		size = Widget_GetContainerWidth( widget->parent );
		size -= _Widget_GetWidth( widget );
		scale = new_pos.x * 1.0 / size;
		scrollbar->data.current_num = scale * scrollbar->data.max_num;
	}
	//_DEBUG_MSG("widget:%p, pos: %d,%d\n", widget, new_pos.x, new_pos.y);
	Widget_Move( widget, new_pos );
}

static void 
ScrollBar_Drag( LCUI_Widget *widget, LCUI_WidgetEvent *event )
{
	static LCUI_Pos pos, offset;
	static LCUI_ScrollBar *scrollbar;
	if( !widget->parent ) {
		return;
	}
	
	scrollbar = Widget_GetPrivData( widget->parent );
	pos = Widget_GetGlobalPos( widget );
	offset = Pos_Sub( event->drag.new_pos, pos ); 
	pos = Pos_Add( pos, offset ); 
	pos = GlobalPos_ConvTo_RelativePos( widget, pos );
	
	Move_ScrollBar( widget, pos );
	/* 若函数指针有效，则调用回调函数 */
	if( scrollbar->callback_func ) {
		//_DEBUG_MSG("current_num: %d, max_num: %d\n", 
		//scrollbar->data.current_num, scrollbar->data.max_num);
		scrollbar->callback_func( scrollbar->data, scrollbar->arg );
	}
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
	
	scrollbar = WidgetPrivData_New(widget, sizeof(LCUI_ScrollBar));
	scrollbar->data.max_num = 100;
	scrollbar->data.current_num = 100;
	scrollbar->data.max_size = 100;
	scrollbar->data.current_size = 100;
	scrollbar->direction = 0;
	scrollbar->callback_func = NULL;
	
	scrollbar->widget = Widget_New("button");
	/* 禁用部件的自动尺寸调整 */
	Widget_SetAutoSize( scrollbar->widget, FALSE, 0 );
	
	Widget_Container_Add( widget, scrollbar->widget );
	Widget_SetSize( scrollbar->widget, "100%", "100%" );
	Widget_LimitPos( scrollbar->widget, Pos(0,0), Pos(0,0) );
	Widget_SetBorder( widget,
	 Border(1, BORDER_STYLE_SOLID, RGB(100,100,100)) );
	Widget_SetPadding( widget, Padding(1,1,1,1) );
	Widget_SetBackgroundColor( widget, RGB(200,200,200) );
	Widget_SetBackgroundTransparent( widget, FALSE );
	Widget_Show( scrollbar->widget );
	Widget_Event_Connect( scrollbar->widget, EVENT_DRAG, ScrollBar_Drag );
}

static void 
ScrollBar_Update( LCUI_Widget *widget )
{
	static int pos, max_len;
	static double scale;
	static char scale_str[256];
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	/* 计算比例，之后转换成字符串 */
	scale = scrollbar->data.current_size*1.0 / scrollbar->data.max_size; 
	sprintf( scale_str, "%.2lf%%", scale*100 );
	//_DEBUG_MSG("current_num: %d, max_num: %d\n", 
	//scrollbar->data.current_num, scrollbar->data.max_num);
	/* 判断滚动条的方向 */
	if( scrollbar->direction == 0 ) { /* 纵向 */
		Widget_SetSize( scrollbar->widget, NULL, scale_str );
		max_len = Widget_GetContainerHeight( widget );
		max_len -= _Widget_GetHeight( scrollbar->widget );
		Widget_LimitPos( scrollbar->widget, Pos(0,0), Pos( 0, max_len ) );
		
		scale = scrollbar->data.current_num*1.0 / scrollbar->data.max_num;
		pos = scale * max_len;
		//_DEBUG_MSG("num: %d / %d\n", 
		//scrollbar->data.current_num, scrollbar->data.max_num);
		//_DEBUG_MSG("ScrollBar_Update(), scale: %.2f\n", scale);
		//_DEBUG_MSG("ScrollBar_Update(), y: %d\n", pos);
		/* 移动滚动条 */
		Widget_Move( scrollbar->widget, Pos(0,pos) );
	} else { /* 横向 */ 
		Widget_SetSize( scrollbar->widget, scale_str, NULL );
		max_len = Widget_GetContainerWidth( widget );
		max_len -= _Widget_GetWidth( scrollbar->widget );
		Widget_LimitPos( scrollbar->widget, Pos(0,0), Pos(max_len,0) );
		scale = scrollbar->data.current_num*1.0 / scrollbar->data.max_num;
		pos = scale * max_len;
		//printf("ScrollBar_Update(), num: %d / %d\n", 
		//scrollbar->data.current_num, scrollbar->data.max_num);
		//printf("ScrollBar_Update(), scale: %.2f\n", scale);
		//printf("ScrollBar_Update(), x: %d\n", pos);
		Widget_Move( scrollbar->widget, Pos(pos, 0) );
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
/* 获取滚动条部件 */
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	return scrollbar->widget;
}

ScrollBar_Data 
ScrollBar_Get_Data ( LCUI_Widget *widget )
/* 获取滚动条的数据 */
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	return scrollbar->data;
}

/* 获取滚动条部件的滑块 */
LCUI_Widget *ScrollBar_GetWidget( LCUI_Widget *widget )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	return scrollbar->widget;
}

void 
ScrollBar_Set_MaxNum( LCUI_Widget *widget, int max_num )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	scrollbar->data.max_num = max_num;
	Widget_Update( widget );
}

void 
ScrollBar_Set_MaxSize( LCUI_Widget *widget, int max_size )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	scrollbar->data.max_size = max_size;
	Widget_Update( widget );
}

void 
ScrollBar_Set_CurrentNum( LCUI_Widget *widget, int current_num )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	scrollbar->data.current_num = current_num;
	Widget_Update( widget );
}

void 
ScrollBar_Set_CurrentSize( LCUI_Widget *widget, int current_size )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	scrollbar->data.current_size = current_size;
	Widget_Update( widget );
}

void
ScrollBar_Connect( 
		LCUI_Widget *widget,
		void (*callback_func)( ScrollBar_Data, void* ),
		void *arg
		)
/* 将回调函数与滚动条部件连接 */
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	scrollbar->callback_func = callback_func;
	scrollbar->arg = arg;
}

/* 设置滚动条是横向移动还是纵向移动 */
void 
ScrollBar_Set_Direction( LCUI_Widget *widget, int direction )
{
	LCUI_ScrollBar *scrollbar;
	
	scrollbar = Widget_GetPrivData( widget );
	if(direction == 0) { /* 纵向 */
		scrollbar->direction = 0;
	} else { /* 横向 */
		scrollbar->direction = 1;
	}
	Widget_Update( widget );
}
/**********************************************************************/

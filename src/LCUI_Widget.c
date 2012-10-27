/* ***************************************************************************
 * LCUI_Widget.c -- processing GUI widget 
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
 * LCUI_Widget.c -- 处理GUI部件
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
//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H 
#include LC_WIDGET_H
#include LC_MISC_H
#include LC_GRAPH_H
#include LC_DISPLAY_H
#include LC_DRAW_H
#include LC_MEM_H
#include LC_FONT_H 
#include LC_ERROR_H 
#include LC_CURSOR_H
#include LC_INPUT_H

#include <unistd.h>
#include <math.h>
#include <limits.h>


/* 声明函数 */
static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, DATATYPE type);


/*************************** Container ********************************/
void Widget_Container_Add(LCUI_Widget *container, LCUI_Widget *widget)
/* 功能：将部件添加至作为容器的部件内 */
{
	int pos;
	LCUI_Queue *old_queue, *new_queue;
	if(widget == NULL || container == widget->parent) {
		return;
	}
		
	if(container == NULL) {/* 如果容器为NULL，那就代表是以屏幕作为容器 */
		new_queue = &LCUI_Sys.widget_list; 
	} else {/* 否则，以指定部件为容器 */ 
		new_queue = &container->child; 
	}
	
	if(widget->parent != NULL){
	/* 如果部件有父部件，那就在父部件的子部件队列中 */ 
		old_queue = &widget->parent->child; 
	} else {
	/* 否则没有部件，那么这个部件在创建时就储存至系统部件队列中 */
		old_queue = &LCUI_Sys.widget_list;
	}
		
	/* 改变该部件的容器，需要将它从之前的容器中移除 */
	pos = WidgetQueue_Get_Pos(old_queue, widget);
	if(pos >= 0) {
		Queue_Delete_Pointer(old_queue, pos);
	}
	
	widget->parent = container; /* 保存父部件指针 */ 
	Queue_Add_Pointer(new_queue, widget); /* 添加至部件队列 */
	Update_Widget_Pos(widget);
}

int _Get_Widget_Container_Width(LCUI_Widget *widget)
/* 通过计算得出指定部件的容器的宽度，单位为像素 */
{
	if( widget->parent == NULL ) {
		return Get_Screen_Width();
	}
	if( widget->parent->w.which_one == 0 ) { 
		return widget->parent->w.px;
	}
	/* 既然容器的宽度也使用百分比，那么就递归调用计算父容器的宽度，直至为整数为止 */
	return widget->parent->w.scale * _Get_Widget_Container_Width( widget->parent );
}

int _Get_Widget_Container_Height(LCUI_Widget *widget)
/* 通过计算得出指定部件的容器的高度，单位为像素 */
{
	if( widget->parent == NULL ) {
		return Get_Screen_Height();
	}
	if( widget->parent->h.which_one == 0 ) {
		return widget->parent->h.px;
	} 
	return widget->parent->h.scale * _Get_Widget_Container_Height( widget->parent );
}

LCUI_Size _Get_Widget_Container_Size( LCUI_Widget *widget )
/* 获取部件容器的尺寸 */
{
	if(widget->parent == NULL) {
		return Get_Screen_Size();
	}
	return widget->parent->size;
}
/************************* Container End ******************************/

/***************************** Widget *********************************/

/*-------------------------- Widget Pos ------------------------------*/
int _Get_Widget_X( LCUI_Widget *widget )
/* 通过计算得出部件的x轴坐标 */
{
	if(widget->x.which_one == 0) {
		return widget->x.px;
	}
	return widget->x.scale * _Get_Widget_Container_Width( widget );
}
int _Get_Widget_Y( LCUI_Widget *widget )
/* 通过计算得出部件的y轴坐标 */
{
	if(widget->x.which_one == 0) {
		return widget->y.px;
	}
	return widget->y.scale * _Get_Widget_Container_Height( widget );
}

static int
_get_x_overlay_widget( LCUI_Queue *list, LCUI_Rect area, LCUI_Queue *out_data )
/* 获取在x轴上指定该区域重叠的部件 */
{
	int i, n, len;
	LCUI_Widget *wptr;
	
	if( Queue_Get_Total( out_data ) > 0 ) {
		Destroy_Queue( out_data );
	}
	len = Queue_Get_Total( list );
	for(i=0,n=0; i<len; ++i) {
		wptr = Queue_Get( list, i );
		/* 如果当前部件区域与area 在x轴上重叠，就添加至队列 */
		if( wptr->pos.x < area.x+area.width
		 && wptr->pos.x >= area.x ) {
			Queue_Add_Pointer( out_data, wptr );
			++n;
		}
	} 
	return n;
}

LCUI_Widget *
_get_max_y_widget( LCUI_Queue *widget_list )
/* 获取底边y轴坐标最大的部件 */
{
	int total, i, y, max_y;
	LCUI_Widget *widget, *tmp;
	
	total = Queue_Get_Total( widget_list );
	widget = Queue_Get( widget_list, 0 );
	
	for(max_y=0,i=0; i<total; ++i) {
		tmp = Queue_Get( widget_list, i );
		y = tmp->y.px + tmp->size.h;
		if(y > max_y) {
			max_y = y;
			widget = tmp;
		}
	}
	return widget;
}

static void Update_StaticPosType_ChildWidget( LCUI_Widget *widget )
/* 更新使用static定位类型的子部件 */
{
	printf("Update_StaticPosType_ChildWidget(): enter\n");
	print_widget_info( widget );
	int i, x, y, total, j, n;
	LCUI_Queue *queue;
	LCUI_Size container_size;
	LCUI_Pos new_pos;
	LCUI_Widget *wptr, *tmp, *tmp2;
	LCUI_Queue *old_row, *cur_row, *tmp_row, q1, q2, widget_list;
	LCUI_Rect area;
	
	new_pos = Pos(0,0);
	old_row = &q1;
	cur_row = &q2;
	Queue_Init( old_row, sizeof(LCUI_Widget*), NULL );
	Queue_Init( cur_row, sizeof(LCUI_Widget*), NULL );
	Queue_Init( &widget_list, sizeof(LCUI_Widget*), NULL );
	Queue_Using_Pointer( &widget_list );
	Queue_Using_Pointer( old_row );
	Queue_Using_Pointer( cur_row );
	
	if(widget == NULL) { 
		queue = &LCUI_Sys.widget_list;
	} else { 
		queue = &widget->child;
	}
	container_size = _Get_Widget_Size( widget );
	total = Queue_Get_Total( queue );
	printf("queue total: %d\n", total);
	for(i=total-1,y=0,x=0; i>=0; --i) {
		wptr = Queue_Get( queue, i );
		/* 过滤掉定位类型不是static和relative的部件 */
		if(wptr->pos_type != POS_TYPE_STATIC
		&& wptr->pos_type != POS_TYPE_RELATIVE ) {
			continue;
		}
		
		printf("0, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		if( y == 0 ) {/* 如果还在第一行，就直接记录部件 */
			/* 记录部件指针 */
			Queue_Add_Pointer( cur_row, wptr );
			/* 更新部件位置 */
			Move_Widget( wptr, new_pos );
			print_widget_info( wptr );
			printf("1, %d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
			if(new_pos.x + wptr->size.w > container_size.w) {
				new_pos.x = x = 0; 
				Destroy_Queue( old_row );
				tmp_row = old_row;
				old_row = cur_row;
				cur_row = tmp_row;
				tmp = Queue_Get( old_row, x ); 
				if(tmp != NULL) {
					new_pos.y = tmp->pos.y + tmp->size.h;
				}
				++y;
			} else { 
				/* 更新x轴坐标 */
				new_pos.x += wptr->size.w;
			}
			printf("y == 0\n");
			printf("1, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			
			continue;
		}
		printf("2,%d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
		/* 如果当前部件区块超出容器范围，y++，开始在下一行记录部件指针。*/
		if(new_pos.x + wptr->size.w > container_size.w) {
			new_pos.x = x = 0; 
			Destroy_Queue( old_row );
			tmp_row = old_row;
			old_row = cur_row;
			cur_row = tmp_row;
			tmp = Queue_Get( old_row, x );
			if(tmp != NULL) {
				new_pos.y = tmp->pos.y + tmp->size.h;
			}
			++y;
		}
		printf("2, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		/* 保存在新位置的部件的区域 */
		area = Rect( new_pos.x, new_pos.y, wptr->size.w, wptr->size.h );
		/* 如果有上一行记录,获取上面几行与当前部件在x轴上重叠的部件列表 */ 
		n = _get_x_overlay_widget( cur_row, area, &widget_list );
		printf("n: %d\n", n); 
		if( n <= 0 ) { /* 如果上一行没有与之重叠的部件 */
			Queue_Add_Pointer( cur_row, wptr ); 
			printf("3,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			Move_Widget( wptr, new_pos );
			new_pos.x = wptr->x.px + wptr->size.w;
			continue;
		}
		/* 获取底边y轴坐标最大的部件 */
		tmp = _get_max_y_widget( &widget_list );
		/* 如果上一行中的部件区块的底边超过当前区块的一半 */
		if( tmp->y.px+tmp->size.h > (wptr->y.px+wptr->size.h)/2.0 ) { 
			/* 将该部件左边的部件也一同记录到当前行 */
			for(j=0; j<n; ++j) {
				tmp2 = Queue_Get( &widget_list, j );
				Queue_Add_Pointer( cur_row, tmp2 );
				if(tmp2 == tmp) {
					break;
				}
			}
			new_pos.x = tmp->x.px + tmp->size.w; 
			continue;
		}
		new_pos.y = tmp->y.px+tmp->size.h;
		Queue_Add_Pointer( cur_row, wptr ); 
		printf("4,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		Move_Widget( wptr, new_pos ); 
		new_pos.x = tmp->x.px + wptr->size.w;
	}
	printf("Update_StaticPosType_ChildWidget(): quit\n");
}

LCUI_Pos __Get_Widget_Pos(LCUI_Widget *widget)
/* 功能：通过计算得出部件的位置，单位为像素 */
{
	LCUI_Pos pos;
	LCUI_Size size;
	pos.x = pos.y = 0; 
	if(widget->align != ALIGN_NONE) { 
		size = _Get_Widget_Container_Size( widget ); 
		pos = Align_Get_Pos(size, Get_Widget_Size(widget), widget->align);
		/* 加上偏移距离 */
		pos = Pos_Add(pos, widget->offset);
	} else {
		pos.x = _Get_Widget_X( widget );
		pos.y = _Get_Widget_Y( widget );
	} 
	return pos;
}

LCUI_Pos _Get_Widget_Pos(LCUI_Widget *widget)
/* 功能：通过计算得出部件的位置，单位为像素 */
{
	LCUI_Pos pos;
	LCUI_Size size;
	pos.x = pos.y = 0;
	
	switch(widget->pos_type) {
	    case POS_TYPE_STATIC:
	    case POS_TYPE_RELATIVE: 
		return widget->pos; 
		
	    default: 
		if(widget->align == ALIGN_NONE) { 
			pos.x = _Get_Widget_X( widget );
			pos.y = _Get_Widget_Y( widget );
			break;
		}
		size = _Get_Widget_Container_Size( widget ); 
		pos = Align_Get_Pos(size, Get_Widget_Size(widget), widget->align);
		/* 加上偏移距离 */
		pos = Pos_Add(pos, widget->offset);  
		break;
	}
	return pos;
}

LCUI_Pos Get_Widget_Pos(LCUI_Widget *widget)
/* 功能：获取部件的位置，单位为像素 */
{
	return widget->pos;
}

int _Get_Widget_MaxX( LCUI_Widget *widget ) 
{
	if(widget->max_x.which_one == 0) {
		return widget->max_x.px;
	}
	return widget->max_x.scale * _Get_Widget_Container_Width( widget );
}

int _Get_Widget_MaxY( LCUI_Widget *widget ) 
{
	if(widget->max_y.which_one == 0) {
		return widget->max_y.px;
	}
	return widget->max_y.scale * _Get_Widget_Container_Height( widget );
}

LCUI_Pos _Get_Widget_MaxPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Get_Widget_MaxX( widget );
	pos.y = _Get_Widget_MaxY( widget );
	return pos;
}

LCUI_Pos Get_Widget_MaxPos(LCUI_Widget *widget)
{
	return widget->max_pos;
}

int _Get_Widget_MinX( LCUI_Widget *widget ) 
{
	if(widget->min_x.which_one == 0) {
		return widget->min_x.px;
	}
	return widget->min_x.scale * _Get_Widget_Container_Width( widget );
}

int _Get_Widget_MinY( LCUI_Widget *widget ) 
{
	if(widget->min_y.which_one == 0) {
		return widget->min_y.px;
	}
	return widget->min_y.scale * _Get_Widget_Container_Height( widget );
}

LCUI_Pos _Get_Widget_MinPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Get_Widget_MinX( widget );
	pos.y = _Get_Widget_MinY( widget );
	return pos;
}

LCUI_Pos Get_Widget_MinPos(LCUI_Widget *widget)
{
	return widget->min_pos;
}
/*------------------------ END Widget Pos -----------------------------*/

/*------------------------- Widget Size -------------------------------*/
int _Get_Widget_MaxWidth( LCUI_Widget *widget ) 
{
	if(widget->max_w.which_one == 0) {
		return widget->max_w.px;
	}
	return widget->max_w.scale * _Get_Widget_Container_Width( widget );
}

int _Get_Widget_MaxHeight( LCUI_Widget *widget ) 
{
	if(widget->max_h.which_one == 0) {
		return widget->max_h.px;
	}
	return widget->max_h.scale * _Get_Widget_Container_Height( widget );
}

LCUI_Size _Get_Widget_MaxSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Get_Widget_MaxWidth( widget );
	size.h = _Get_Widget_MaxHeight( widget );
	return size;
}

LCUI_Size Get_Widget_MaxSize( LCUI_Widget *widget )
{
	return widget->max_size;
}

int _Get_Widget_MinWidth( LCUI_Widget *widget ) 
{
	if(widget->min_w.which_one == 0) {
		return widget->min_w.px;
	}
	return widget->min_w.scale * _Get_Widget_Container_Width( widget );
}

int _Get_Widget_MinHeight( LCUI_Widget *widget ) 
{
	if(widget->min_h.which_one == 0) {
		return widget->min_h.px;
	}
	return widget->min_h.scale * _Get_Widget_Container_Height( widget );
}

int _Get_Widget_Height(LCUI_Widget *widget)
{
	if(widget->h.which_one == 0) {
		return widget->h.px;
	}
	return widget->h.scale * _Get_Widget_Container_Height( widget );
}

int _Get_Widget_Width(LCUI_Widget *widget)
{
	if(widget->w.which_one == 0) {
		return widget->w.px;
	}
	return widget->w.scale * _Get_Widget_Container_Width( widget );
}

int Get_Widget_Height(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).h;
}

int Get_Widget_Width(LCUI_Widget *widget)
{
	return Get_Widget_Size(widget).w;
}

LCUI_Size _Get_Widget_MinSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Get_Widget_MinWidth( widget );
	size.h = _Get_Widget_MinHeight( widget );
	return size;
}

LCUI_Size Get_Widget_MinSize(LCUI_Widget *widget)
{
	return widget->min_size;
}

LCUI_Size Get_Widget_Size(LCUI_Widget *widget)
/* 功能：获取部件的尺寸 */
{
	return widget->size;
}

LCUI_Size _Get_Widget_Size(LCUI_Widget *widget)
/* 功能：通过计算获取部件的尺寸 */
{
	LCUI_Size size;
	size.w = _Get_Widget_Width( widget );
	size.h = _Get_Widget_Height( widget );
	return size;
}


LCUI_Rect Get_Widget_Rect(LCUI_Widget *widget)
/* 功能：获取部件的区域 */
{
	return Rect(widget->pos.x, widget->pos.y, 
			widget->size.w, widget->size.h);
}

/*------------------------ END Widget Size ----------------------------*/

void *Get_Widget_PrivData(LCUI_Widget *widget)
/* 功能：获取部件的私有数据结构体的指针 */
{
	if(widget == NULL) {
		return NULL;
	}
	return widget->private_data;
}

LCUI_Widget *Get_Widget_Parent(LCUI_Widget *widget)
/* 功能：获取部件的父部件 */
{
	return widget->parent;
}

void Get_Widget_Real_Graph(LCUI_Widget *widget, LCUI_Rect rect, LCUI_Graph *graph)
/*
 * 功能：获取指定区域中部件的实际图形
 * 说明：获取图形前，需要指定背景图，之后才能正常合成；获取的图形是指定区域中显示的实际图形
 **/
{
	//printf("Get_Widget_Real_Graph(): enter\n");
	//print_widget_info(widget);
	if( !widget->visible ) {
		return;
	}
	
	int x, y, i;
	LCUI_Graph temp;
	LCUI_Widget *child;
	/* 获取部件在屏幕内的坐标 */
	LCUI_Pos pos = Get_Widget_Global_Pos(widget);
	/* 计算部件图形粘贴至背景中的位置 */
	x = pos.x - rect.x;
	y = pos.y - rect.y;
	//printf("global pos: %d, %d\n", pos.x, pos.y);
	//printf("update area: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
	/*
	 * 由于父部件是子部件的容器，子部件的显示范围仅在父部件内，需
	 * 要调整这个矩形的区域，如果子部件的图形超出容器范围，则裁剪子
	 * 部件的图形并粘贴至背景图中。
	 **/
	 if(widget->parent != NULL) {/* 如果有父部件 */ 
		LCUI_Rect cut_rect;
		/* 获取该部件的有效显示区域 */
		cut_rect = Get_Widget_Valid_Rect(widget);
		/* 加上裁剪区域的起点坐标 */
		x += cut_rect.x;
		y += cut_rect.y;
		/* 引用图层中指定区域的图形 */
		Quote_Graph(&temp, &widget->graph, cut_rect);
		/* 合成之 */
		Graph_Mix(&widget->graph, &temp, Pos(x, y));
	} else { /* 先将父部件合成至背景图中 */
		Graph_Mix (graph, &widget->graph, Pos(x, y));
	}
	
	//printf("widget overlay pos: %d, %d\n", x, y);
	int total;
	LCUI_Rect tmp;
	
	total = Queue_Get_Total(&widget->child); 
	/* 貌似只需要x和y，区域尺寸靠背景图的信息即可得到 */
	for(i=total-1; i>=0; --i) {/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		//printf("get child widget: %p, type: %s\n", child, child->type.string);
		if(child == NULL) {
			continue;
		}
		if( child->visible ) {/* 如果有可见的子部件 */
			tmp = Get_Widget_Rect(child);
			tmp.x += pos.x;
			tmp.y += pos.y;
			//printf("tmp: %d, %d, %d, %d\n", tmp.x, tmp.y, tmp.width, tmp.height);
			//printf("rect: %d, %d, %d, %d\n", rect.x, rect.y, rect.width, rect.height);
			/* 如果与该区域重叠，将子部件合成至背景图中 */
			if (Rect_Is_Overlay(tmp, rect)) {
				//printf("overlay\n");
				Get_Widget_Real_Graph(child, rect, graph);  /* 递归调用 */
			}
			//else printf("not bverlay\n");
		} 
	}
	//printf("Get_Widget_Real_Graph(): quit\n");
}

void print_widget_info(LCUI_Widget *widget)
/* 
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 *  */
{
	if(widget != NULL) {
		printf("widget: %p, type: %s, visible: %d, pos: (%d,%d), size: (%d, %d)\n",
				widget, widget->type.string, widget->visible,
				widget->pos.x, widget->pos.y,
				widget->size.w, widget->size.h);  
	}
	else printf("NULL widget\n");
}


int Add_Widget_Refresh_Area (LCUI_Widget * widget, LCUI_Rect rect)
/* 功能：在指定部件的内部区域内设定需要刷新的区域 */
{
	DEBUG_MSG("Add_Widget_Refresh_Area():enter\n");
	if(widget == NULL) { 
		return Add_Screen_Refresh_Area(rect);
	}
	if (rect.width <= 0 || rect.height <= 0) { 
		return -1;
	}
	
	/* 调整矩形位置及尺寸 */
	rect = Get_Valid_Area(Get_Widget_Size(widget), rect);
	if( widget->visible ) { 
		LCUI_Sys.need_shift_area = IS_TRUE; 
	}
	/* 保存至队列中 */
	if(0 != RectQueue_Add (&widget->update_area, rect)) { 
		return -1;
	}
	
	DEBUG_MSG("Add_Widget_Refresh_Area():quit\n");
	return 0;
}

void Set_Widget_Border_Style(LCUI_Widget *widget, BORDER_STYLE style)
/* 功能：设定部件的边框风格 */
{
	widget->border_style = style;
	Draw_Widget(widget);
}

void Response_Status_Change(LCUI_Widget *widget)
/* 
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 *  */
{
	widget->status_response = TRUE;
}

void Shift_Widget_Refresh_Area(LCUI_Widget *widget)
/* 功能：转移部件的rect队列成员至父部件中 */
{
	DEBUG_MSG("Shift_Widget_Refresh_Area(): enter\n");
	int i, total;
	LCUI_Widget *child;
	LCUI_Rect rect;
	LCUI_Queue *widget_list;
	
	if(NULL == widget) {
		widget_list = &LCUI_Sys.widget_list; 
	} else {
		widget_list = &widget->child; 
	}
	total = Queue_Get_Total(widget_list);
	for(i=total-1; i>=0; --i) {/* 从底到顶遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(widget_list, i);
		if(child != NULL && child->visible ) {/* 如果有子部件 */
			/* 递归调用，将记录的区域转移至子部件内 */
			Shift_Widget_Refresh_Area(child);
			while( RectQueue_Get(&rect, 0, &child->update_area) ) {
				rect.x += child->pos.x;
				rect.y += child->pos.y;
				//printf("[%d]\033[1;34mShift_Widget_Refresh_Area(x:%d, y:%d, w:%d, h:%d)\033[0m\n", child->update_area.total_num, rect.x, rect.y, rect.width, rect.height);
				/* 将子部件的rect队列成员复制到自己这里 */ 
				Add_Widget_Refresh_Area(widget, rect); 
				if(Strcmp(&child->type, "button") == 0) {
					DEBUG_MSG("shift area:%d,%d,%d,%d\n", rect.x, rect.y, rect.width, rect.height);
				}
				/* 删除子部件rect队列成员 */
				Queue_Delete(&child->update_area, 0);
			}
		}
	} 
	DEBUG_MSG("Shift_Widget_Refresh_Area(): quit\n");
}

void Handle_Refresh_Area()
/*
 * 功能：处理已记录的刷新区域
 * 说明：此函数会将各个部件的rect队列中的处理掉，并将
 * 最终的局部刷新区域数据添加至屏幕刷新区域队列中，等
 * 待LCUI来处理。
 **/
{ 
	/* 如果flag标志的值为IS_TRUE */ 
	if ( LCUI_Sys.need_shift_area ) {	
		/* 转移部件内记录的区域至主记录中 */ 
		Shift_Widget_Refresh_Area ( NULL );
		/* 
		 * 复位标志，这是为了避免每次进入本函数时都要进入递归，并对各
		 * 个部件的矩形数据进行移动，因为这是浪费时间，降低了程序的效
		 * 率 
		 * */
		LCUI_Sys.need_shift_area = IS_FALSE; 
	} 
}


LCUI_Widget *Get_Parent_Widget(LCUI_Widget *widget, char *widget_type)
/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/
{
	LCUI_Widget *temp;
	if(widget == NULL) {
		return NULL; /* 本身是NULL就退出函数 */
	}
	temp = widget;
	while(temp->parent != NULL) {
		if(temp->parent != NULL
		  && Strcmp(&temp->parent->type, widget_type) == 0
		) {/* 如果指针有效，并且类型符合要求 */
			return temp->parent; /* 返回部件的指针 */
		}
		temp = temp->parent;/* 获取部件的父级部件指针 */
	}
	return NULL;
}

int LCUI_Destroy_App_Widgets(LCUI_ID app_id)
/* 功能：销毁指定ID的程序的所有部件 */
{
	int i, total;
	LCUI_Widget *temp;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for(i=0; i<total; i++) {
		temp = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list,i);
		if(temp->app_id == app_id) {
			/* 
			 * 在Queue_Delete()函数将队列中的部件移除时，会调用初始化部件队列时指
			 * 定的Destroy_Widget()函数进行部件数据相关的清理。
			 * */
			Queue_Delete(&LCUI_Sys.widget_list, i);
			/* 重新获取部件总数 */
			total = Queue_Get_Total(&LCUI_Sys.widget_list);
			--i;/* 当前位置的部件已经移除，空位已由后面部件填补，所以，--i */
		}
	}
	return 0;
}


LCUI_String Get_Widget_Style(LCUI_Widget *widget)
/* 功能：获取部件的类型 */
{
	return widget->style;
}

BORDER_STYLE Get_Widget_Border_Style(LCUI_Widget *widget)
/* 功能：获取部件的边框类型 */
{
	return widget->border_style;
}

void Set_Widget_Style(LCUI_Widget *widget, char *style)
/* 功能：设定部件的风格 */
{
	Strcpy(&widget->style, style); 
}

LCUI_Widget *Get_Widget_By_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/* 功能：获取部件中包含指定坐标的点的子部件 */
{ 
	if(widget == NULL) {
		return NULL;
	}
	
	int i, temp;
	LCUI_Widget *child;
	LCUI_Widget *w = widget; 
	
	for(i=0;;++i) {/* 从顶到底遍历子部件 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if(NULL == child) {
			break;
		}
		if( child->visible ) { 
			temp = Rect_Inside_Point( pos, Get_Widget_Rect(child) );
			if(temp == 1) {/* 如果这个点被包含在部件区域内 */
				/* 递归调用，改变相对坐标 */
				w = Get_Widget_By_Pos(child, Pos_Sub(pos, child->pos));
				if(w == NULL) {
					return widget;
				}
				break;
			} else {
				continue;
			}
		}
	}
	return w; 
}

LCUI_Widget *Get_Cursor_Overlay_Widget()
/* 功能：获取鼠标光标当前覆盖的部件 */
{ 
	int temp;
	int total,k; 
	LCUI_Widget *widget, *w = NULL;
	
	total = Queue_Get_Total(&LCUI_Sys.widget_list);
	for (k=0; k<total; ++k) {/* 从最顶到底，遍历部件 */ 
		widget = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, k);
		if(widget == NULL) {
			break;
		}
		if( !widget->visible ) {
			continue;
		}
			
		temp = Rect_Inside_Point(Get_Cursor_Pos(), Get_Widget_Rect(widget) );
		if(temp != 1) {
			continue; 
		}
		/* 如果这个点被包含在部件区域内，那就获取部件中包含指定坐标的点的子部件 */  
		w = Get_Widget_By_Pos(widget, Pos_Sub(Get_Cursor_Pos(), 
					Get_Widget_Pos(widget)));
		if(w == NULL) {
			w = widget; 
		}
		break;
	} 
	return w;  
}

LCUI_Widget *Get_Focus_Widget()
/*
 * 功能：获取当前焦点状态下的部件
 * 返回值：正常获取则返回部件的指针，否则，返回NULL
 **/
{
	#ifdef USE_JUNK___
	LCUI_App *app = Get_Self_AppPointer();
	LCUI_Widget *widget;
	widget = (LCUI_Widget*)Queue_Get(&app->widget_display, 0);
	if(widget != NULL) {
		while(widget->focus != NULL) {
			widget = widget->focus;
		}
	}
	else return NULL;
	return widget;
	#else 
	return NULL;
	#endif
}

int Widget_Is_Active(LCUI_Widget *widget)
/* 功能：判断部件是否为活动状态 */
{
	if(widget->status != KILLED) {
		return 1;
	}
	return 0;
}

LCUI_Rect Get_Widget_Valid_Rect(LCUI_Widget *widget)
/* 
 * 功能：获取部件在屏幕中实际显示的区域 
 * 说明：返回的是部件需要裁剪的区域
 * */
{
	LCUI_Pos pos;
	int w, h, temp; 
	LCUI_Rect cut_rect;
	cut_rect.x = 0;
	cut_rect.y = 0;
	cut_rect.width = widget->size.w;
	cut_rect.height = widget->size.h;
	
	pos = widget->pos; 
	if(widget->parent == NULL) {
		w = Get_Screen_Width();
		h = Get_Screen_Height();
	} else {
		w = widget->parent->size.w;
		h = widget->parent->size.h;
	}
	
	/* 获取需裁剪的区域 */
	if(pos.x < 0) {
		cut_rect.width += pos.x;
		cut_rect.x = 0 - pos.x; 
	}
	if(pos.x + widget->size.w > w) {
		cut_rect.width -= (pos.x +  widget->size.w - w); 
	}
	if(pos.y < 0) {
		cut_rect.height += pos.y;
		cut_rect.y = 0 - pos.y; 
	}
	if(pos.y + widget->size.h > h) {
		cut_rect.height -= (pos.y +  widget->size.h - h); 
	}
	if(widget->parent == NULL) {
		return cut_rect;
	}
		
	LCUI_Rect rect;
	/* 获取父部件的有效显示范围 */
	rect = Get_Widget_Valid_Rect(widget->parent);
	/* 如果父部件需要裁剪，那么，子部件根据情况，也需要进行裁剪 */
	if(rect.x > 0) {/* 如果裁剪区域的x轴坐标大于0 */
		/* 裁剪区域和部件区域是在同一容器中，只要得出两个区域的重叠区域即可 */
		temp = pos.x + cut_rect.x;
		if(temp < rect.x) { /* 如果部件的 x轴坐标+裁剪起点x轴坐标 小于它 */
			temp = rect.x - pos.x;		/* 新的裁剪区域起点x轴坐标 */
			cut_rect.width -= (temp - cut_rect.x);/* 改变裁剪区域的宽度 */
			cut_rect.x = temp;			/* 改变部件的裁剪区域的x坐标 */
		}
	}
	if(rect.y > 0) {
		temp = pos.y + cut_rect.y;
		if(pos.y < rect.y) {
			temp = rect.y - pos.y;
			cut_rect.height -= (temp - cut_rect.y);
			cut_rect.y = temp;
		}
	}
	if(rect.width < w) {/* 如果父部件裁剪区域的宽度小于父部件的宽度 */
		temp = pos.x+cut_rect.x+cut_rect.width;
		if(temp > rect.x+rect.width) /* 如果部件裁剪区域左边部分与父部件裁剪区域重叠 */
			cut_rect.width -= (temp-(rect.x+rect.width));
	}
	if(rect.height < h) {
		temp = pos.y+cut_rect.y+cut_rect.height;
		if(temp > rect.y+rect.height)
			cut_rect.height -= (temp-(rect.y+rect.height));
	} 
	
	return cut_rect;
}

int Empty_Widget()
/* 
 * 功能：用于检测程序的部件列表是否为空 
 * 返回值：
 *   1  程序的部件列表为空
 *   0  程序的部件列表不为空
 * */
{
	if(Queue_Get_Total(&LCUI_Sys.widget_list) <= 0) {
		return 1;
	}
	return 0;
}

static void WidgetData_Init(LCUI_Queue *queue);

void *Widget_Create_PrivData(LCUI_Widget *widget, size_t size)
/* 功能：为部件私有结构体指针分配内存 */
{
	widget->private_data = malloc(size);
	return widget->private_data;
}

LCUI_Widget *Create_Widget( const char *widget_type )
/* 
 * 功能：创建指定类型的窗口部件
 * 说明：创建出来的部件，默认是没有背景图时透明。
 * 返回值：成功则部件的指针，失败则返回NULL
 */
{
	int pos;
	LCUI_Widget *p, widget;
	LCUI_App *app;
	void (*func_init) (LCUI_Widget*);

	app = Get_Self_AppPointer(); 
	
	/*--------------- 初始化部件基本属性及数据 ------------------*/
	widget.auto_size	= IS_FALSE;
	widget.auto_size_mode	= AUTOSIZE_MODE_GROW_AND_SHRINK;
	widget.type_id		= 0;
	widget.status		= WIDGET_STATUS_NORMAL;
	widget.app_id		= app->id; 
	widget.parent		= NULL;
	widget.enabled		= TRUE;
	widget.visible		= FALSE;
	widget.pos		= Pos(0, 0);
	widget.size		= Size(0, 0); 
	widget.min_size		= Size(0, 0); 
	widget.max_size		= Size(INT_MAX, INT_MAX); 
	widget.align		= ALIGN_NONE; 
	widget.dock		= DOCK_TYPE_NONE;
	widget.offset		= Pos(0, 0); 
	widget.pos_type		= POS_TYPE_ABSOLUTE;
	widget.back_color	= RGB(238,243,250);
	widget.fore_color	= RGB(0,0,0);
	widget.border_color	= RGB(0,0,0);
	widget.border_style	= BORDER_STYLE_NONE;
	widget.private_data	= NULL;
	widget.bg_mode		= BG_MODE_TRANSPARENT;
	widget.status_response	= 0;
	/*------------------------- END --------------------------*/
	
	/*--------------- 初始化部件的附加属性 ------------------*/
	PX_P_t_init( &widget.x );
	PX_P_t_init( &widget.y );
	PX_P_t_init( &widget.max_x );
	PX_P_t_init( &widget.max_y );
	PX_P_t_init( &widget.min_x );
	PX_P_t_init( &widget.min_y );
	widget.max_x.px = INT_MAX;
	widget.max_y.px = INT_MAX;
	widget.min_x.px = INT_MIN;
	widget.min_y.px = INT_MIN;
	PX_P_t_init( &widget.w );
	PX_P_t_init( &widget.h );
	PX_P_t_init( &widget.max_w );
	PX_P_t_init( &widget.max_h );
	PX_P_t_init( &widget.min_w );
	PX_P_t_init( &widget.min_h );
	/*---------------------- END -----------------------*/
	
	/*------------- 函数指针初始化 ------------------*/
	widget.set_align = Set_Widget_Align;
	widget.set_alpha = Set_Widget_Alpha;
	widget.set_border = Set_Widget_Border;
	widget.show = Show_Widget;
	widget.hide = Hide_Widget;
	widget.resize = Resize_Widget;
	widget.move = Move_Widget;
	widget.enable = Enable_Widget;
	widget.disable = Disable_Widget;
	/*----------------- END -----------------------*/
	
	/* 初始化边框数据 */
	Border_Init(&widget.border);
	
	Graph_Init(&widget.graph);		/* 初始化部件图形数据 */
	Graph_Init(&widget.background_image);	/* 初始化背景图数据 */
	widget.graph.have_alpha = TRUE;		/* 部件图形设为有alpha通道 */
	RectQueue_Init(&widget.update_area);	/* 初始化区域更新队列 */
	EventQueue_Init(&widget.event);		/* 初始化部件的事件数据队列 */
	WidgetQueue_Init(&widget.child);	/* 初始化子部件集 */
	WidgetData_Init(&widget.data);		/* 初始化数据更新队列 */
	String_Init(&widget.type);
	String_Init(&widget.style);
	
	/* 最后，将该部件数据添加至部件队列中 */
	pos = Queue_Add(&LCUI_Sys.widget_list, &widget);
	p = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, pos);
	
	if(widget_type != NULL) {
		/* 验证部件类型是否有效 */
		if( !WidgetType_Valid(widget_type) ) {
			puts(WIDGET_ERROR_TYPE_NOT_FOUND);
			return NULL;
		}
		/* 保存部件类型 */
		Strcpy(&p->type, widget_type);
		p->type_id = WidgetType_Get_ID(widget_type);	/* 获取类型ID */
		/* 获取初始化部件私有结构体数据的函数指针 */ 
		func_init = Get_WidgetFunc_By_ID(p->type_id, FUNC_TYPE_INIT);
		/* 进行初始化 */
		func_init( p ); 
	} 
	return p;
}

void Delete_Widget(LCUI_Widget *widget)
/* 功能：删除一个部件 */
{
	int i, total;
	LCUI_Queue *p;
	LCUI_Widget *tmp;
	if(NULL == widget) {
		return;
	}
	if(widget->parent == NULL) {
		p = &LCUI_Sys.widget_list;
	} else {
		p = &widget->parent->child; 
	}
	
	total = Queue_Get_Total(p);
	for(i=0; i<total; ++i) {
		tmp = (LCUI_Widget*)Queue_Get(p, i);
		if(tmp == widget) {
			Queue_Delete(p, i);
			break;
		}
	}	
}

LCUI_Pos Count_Widget_Pos(LCUI_Widget *widget)
/* 功能：累计部件的位置坐标 */
{
	LCUI_Pos pos;
	if(widget->parent == NULL) {
		return widget->pos; 
	}
	pos = Count_Widget_Pos(widget->parent); 
	pos = Pos_Add(pos, widget->pos);
	return pos;
}

LCUI_Pos Get_Widget_Global_Pos(LCUI_Widget *widget)
/* 功能：获取部件的全局坐标 */
{
	return Count_Widget_Pos(widget);
}

void Set_Widget_BG_Mode(LCUI_Widget *widget, BG_MODE bg_mode)
/*
 * 功能：改变部件的背景模式
 * 说明：背景模式决定了部件在没有背景图的时候是使用背景色填充还是完全透明。
 **/
{
	widget->bg_mode = bg_mode;
}

void Set_Widget_Align(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset)
/* 功能：设定部件的对齐方式以及偏移距离 */
{
	if(widget == NULL) {
		return;
	}
	widget->align = align;
	widget->offset = offset;
	Update_Widget_Pos(widget);/* 更新位置 */
}

int Set_Widget_MaxSize( LCUI_Widget *widget, char *width, char *height )
/* 
 * 功能：设定部件的最大尺寸 
 * 说明：当值为0时，部件的尺寸不受限制，用法示例可参考Set_Widget_Size()函数 
 * */
{
	int n;
	n = get_PX_P_t( width, &widget->max_w );
	n += get_PX_P_t( width, &widget->max_h );
	return n;
}

int Set_Widget_MinSize( LCUI_Widget *widget, char *width, char *height )
/* 
 * 功能：设定部件的最小尺寸 
 * 说明：用法示例可参考Set_Widget_Size()函数 
 * */
{
	int n;
	n = get_PX_P_t( width, &widget->min_w );
	n += get_PX_P_t( width, &widget->min_h );
	return n;
}

void Limit_Widget_Size(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size)
/* 功能：限制部件的尺寸变动范围 */
{
	if(min_size.w < 0) {
		min_size.w = 0;
	}
	if(min_size.h < 0) {
		min_size.h = 0;
	}
	if(max_size.w < 0) {
		max_size.w = 0;
	}
	if(max_size.h < 0) {
		max_size.h = 0;
	}
	if(max_size.w < min_size.w) {
		max_size.w = min_size.w;
	}
	if(max_size.h < min_size.h) {
		max_size.h = min_size.h;
	}
		
	widget->min_w.px = min_size.w;
	widget->min_h.px = min_size.h;
	widget->min_w.which_one = 0;
	widget->min_h.which_one = 0;
	widget->max_w.px = max_size.w;
	widget->max_h.px = max_size.h;
	widget->max_w.which_one = 0;
	widget->max_h.which_one = 0;
}

void Limit_Widget_Pos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos)
/* 功能：限制部件的移动范围 */
{
	if(min_pos.x < 0) {
		min_pos.x = 0;
	}
	if(min_pos.y < 0) {
		min_pos.y = 0;
	}
	if(max_pos.x < 0) {
		max_pos.x = 0;
	}
	if(max_pos.y < 0) {
		max_pos.y = 0;
	}
	if(max_pos.x < min_pos.x) {
		max_pos.x = min_pos.x;
	}
	if(max_pos.y < min_pos.y) {
		max_pos.y = min_pos.y;
	}
		
	widget->min_x.px = min_pos.x;
	widget->min_y.px = min_pos.y;
	widget->min_x.which_one = 0;
	widget->min_x.which_one = 0;
	widget->max_x.px = max_pos.x;
	widget->max_y.px = max_pos.y;
	widget->max_x.which_one = 0;
	widget->max_y.which_one = 0;
}

void _Limit_Widget_Pos( LCUI_Widget *widget, char *x_str, char*y_str )
{
	
}

void _Limit_Widget_Size( LCUI_Widget *widget, char *w_str, char*h_str )
{
	
}

void Set_Widget_Border(LCUI_Widget *widget, LCUI_RGB color, LCUI_Border border)
/* 功能：设定部件的边框 */
{ 
	widget->border = border;
	widget->border_color = color;
	
	if(widget->border_style == BORDER_STYLE_NONE) {
		Set_Widget_Border_Style(widget, BORDER_STYLE_LINE_BORDER);
	}
	Draw_Widget(widget); 
	Add_Widget_Refresh_Area(widget, Get_Widget_Rect(widget));
}


void Set_Widget_Backcolor(LCUI_Widget *widget, LCUI_RGB color)
/* 功能：设定部件的背景色 */
{
	widget->back_color = color;
	Draw_Widget(widget);
	Refresh_Widget(widget);
}

int Set_Widget_Background_Image(LCUI_Widget *widget, LCUI_Graph *img, int flag)
/*
 * 功能：为部件填充背景图像
 */
{
	if(NULL == img) {
		Graph_Free(&widget->background_image);
	} else {
		widget->background_image_layout = flag;
		/* 填充背景图像 */
		Graph_Copy(&widget->background_image, img);
	}
	Draw_Widget(widget); 
	return 0;
}

int Mix_Widget_FontBitmap(	LCUI_Widget *widget, 
				int start_x, int start_y, 
				LCUI_WString *contents, 
				int rows, int space, 
				int linegap, int flag	)
/* 功能：混合部件内的字体位图，使字体能在部件上显示 */
{
	if(!Graph_Valid(&widget->graph)) {
		return -1; 
	}
	/* 如果文字行数不大于0 */ 
	if(rows <= 0) {
		return -2;
	}
	
	int i, j ,x = start_x, y = start_y, height = 13;
	//for(i = 0; i < rows; ++i) {
		//for(j=0; j < contents[i].size; ++j) {
			//if(contents[i].string[j].bitmap.height > 0) {
				//height = contents[i].string[j].bitmap.height;
				//break;
			//}
		//}
		//if(height > 0) {
			//break;
		//}
	//}
	
	Graph_Lock(&widget->graph, 1);
	for(i = 0; i < rows; ++i) {
		/* 如果已经超出部件的尺寸，就不需要绘制字体位图了 */
		if(y > widget->size.h) {
			break; 
		}
		for(j=0; j < contents[i].size; ++j) {
			if(x > widget->size.w) {
				break; 
			}
			/* 粘贴每个字 */
			FontBMP_Mix(
				&widget->graph,  Pos(x, y) , 
				&contents[i].string[j].bitmap , 
				contents[i].string[j].color, flag );
			
			/* 如果这一行文字需要更新 */
			if(contents[i].update == IS_TRUE) {
				contents[i].update = IS_FALSE;
				Add_Widget_Refresh_Area( widget, 
					Rect(	start_x, y-1, widget->size.w, 
					contents[i].string[0].bitmap.rows+2 )
				);
				/* 稍微增加点区域范围，所以y-1，height+2 */
			}
			/* 累计文字间距 */
			if( j < contents[i].size - 1) {
				x = x + space;  
			}
			/* 累计文字宽度 */
			x = x + contents[i].string[j].bitmap.width;
		}
		if(contents[i].size == 0) {
			y += height;
		} else {
			y += 13;//contents[i].string[0].bitmap.height; 
		}
		/* 累计文字行距 */
		if(i > 0 && i < rows - 1) {
			y += linegap; 
		}
		/* x归位 */
		x = start_x;
		//printf("Mix_Widget_FontBitmap():rows:%d/%d, size:%d\n", i, rows, contents[i].size);
	} 
	Graph_Unlock(&widget->graph);
	return 0;
}

void Enable_Widget(LCUI_Widget *widget)
/* 功能：启用部件 */
{
	widget->enabled = IS_TRUE; 
	Set_Widget_Status(widget, WIDGET_STATUS_NORMAL);
}

void Disable_Widget(LCUI_Widget *widget)
/* 功能：禁用部件 */
{
	widget->enabled = IS_FALSE; 
	Set_Widget_Status(widget, WIDGET_STATUS_DISABLE);
}

void Widget_Visible(LCUI_Widget *widget, int flag)
/* 功能：定义部件是否可见 */
{
	if(flag == IS_TRUE) {
		widget->visible = IS_TRUE; 
	} else {
		widget->visible = IS_FALSE; 
	}
}

void Set_Widget_Pos(LCUI_Widget *widget, LCUI_Pos pos)
/* 
 * 功能：设定部件的位置 
 * 说明：只修改坐标，不进行局部刷新
 * */
{
	widget->pos = pos;
}

void Set_Widget_PosType( LCUI_Widget *widget, POS_TYPE pos_type )
/* 设定部件的定位类型 */
{
	widget->pos_type = pos_type;
	Record_WidgetUpdate( widget, NULL, DATATYPE_POS_TYPE );
}

void Set_Widget_Alpha(LCUI_Widget *widget, unsigned char alpha)
/* 功能：设定部件的透明度 */
{
	if(widget->graph.alpha != alpha) {
		widget->graph.alpha = alpha; 
		Refresh_Widget(widget);
	}
}

void Exec_Move_Widget(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/
{
	LCUI_Pos t;
	LCUI_Rect old_rect;
	LCUI_Pos max_pos, min_pos;
	
	max_pos = Get_Widget_MaxPos( widget );
	min_pos = Get_Widget_MinPos( widget );
	/* 根据限制的移动范围，调整位置 */
	if(pos.x > max_pos.x) {
		pos.x = max_pos.x;
	}
	if(pos.y > max_pos.y) {
		pos.y = max_pos.y;
	}
	if(pos.x < min_pos.x) {
		pos.x = min_pos.x;
	}
	if(pos.y < min_pos.y) {
		pos.y = min_pos.y;
	} 
	t = widget->pos; /* 记录老位置 */
	widget->pos = pos;/* 记录新位置 */
	if( widget->visible ) {/* 如果该部件可见 */
		old_rect = Rect(t.x, t.y, widget->size.w, widget->size.h);
		Add_Widget_Refresh_Area(widget->parent, old_rect); /* 刷新老区域 */
		Refresh_Widget(widget); /* 刷新现在的区域 */
		/* 
		 * 不用担心老区域和新区域存在重叠区域而导致屏幕刷新效率下降的问题，因为LCUI会
		 * 在处理这些区域时，会将这些区域进行分割和删减，确保区域独立且不重叠。
		 *  */
	}
}

void Exec_Hide_Widget(LCUI_Widget *widget)
/* 功能：执行隐藏部件的操作 */
{
	if(widget == NULL || !widget->visible ) {
		return; 
	}
	
	void ( *func_hide ) (LCUI_Widget*);
	
	/* 获取隐藏部件需要调用的函数指针，并调用之 */
	func_hide = Get_WidgetFunc_By_ID( widget->type_id,
					FUNC_TYPE_HIDE );
	func_hide(widget);
	
	Widget_Visible(widget, IS_FALSE); 
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget)); 
}


void Exec_Show_Widget(LCUI_Widget *widget)
/* 功能：执行显示部件的任务 */
{ 

	if(widget == NULL || widget->visible ) {
		return; 
	}
	
	void ( *func_show ) (LCUI_Widget*);
	//Exec_Update_Widget(widget);/* 显示前，需要对部件图形数据进行一次更新 */
	Widget_Visible(widget, IS_TRUE); /* 部件可见 */
	
	/* 调用该部件在显示时需要用到的函数 */
	func_show = Get_WidgetFunc_By_ID(widget->type_id, FUNC_TYPE_SHOW); 
	func_show(widget); 
	
	Refresh_Widget(widget); /* 刷新部件所在区域的图形显示 */
}

void Auto_Resize_Widget(LCUI_Widget *widget)
/* 功能：自动调整部件大小，以适应其内容大小 */
{
	int i, total, temp;
	LCUI_Widget *child;
	LCUI_Queue point;
	LCUI_Size size;
	
	size.w = 0;
	size.h = 0;
	Queue_Init(&point, sizeof (LCUI_Pos), NULL);
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i) {/* 遍历每个子部件 */
		child = (LCUI_Widget *)Queue_Get(&widget->child, i);
		/* 保存各个子部件区域矩形的右上角顶点中，X轴坐标最大的 */
		temp = child->pos.x + child->size.w;
		if(temp > size.w) {
			size.w = temp;
		}
		/* 同上 */
		temp = child->pos.y + child->size.h;
		if(temp > size.h) {
			size.h = temp;
		}
	}
	size.w += 6;
	size.h += 6;
	//printf("Auto_Resize_Widget(): new size: %d,%d\n", size.w, size.h);
	//print_widget_info(widget);
	/* 得出适合的尺寸，调整之 */
	Resize_Widget(widget, size);
}

void Exec_Resize_Widget(LCUI_Widget *widget, LCUI_Size size)
/* 功能：执行改变部件尺寸的操作 */
{
	if(widget == NULL) {
		return;
	}
	void ( *func_resize ) (LCUI_Widget*);
	
	if(widget->size.w == size.w && widget->size.h == size.h) {
		return;
	}
	LCUI_Size max_size, min_size;
	max_size = Get_Widget_MaxSize( widget );
	min_size = Get_Widget_MinSize( widget );
	/* 根据指定的部件的尺寸范围，调整尺寸 */
	if(size.w > max_size.w) {
		size.w = max_size.w;
	}
	if(size.h > max_size.h) {
		size.h = max_size.h;
	}
	if(size.w < min_size.w) {
		size.w = min_size.w;
	}
	if(size.h < min_size.h) {
		size.h = min_size.h;
	}
	
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget));
	
	widget->size = size;
	
	Graph_Create(&widget->graph, size.w, size.h);
	/* 获取改变部件尺寸时需要调用的函数 */
	func_resize = Get_WidgetFunc_By_ID( widget->type_id, FUNC_TYPE_RESIZE );
	func_resize(widget); 
	Refresh_Widget(widget); 
	/* 更新子部件的位置及尺寸 */  
	Update_Child_Widget_Size( widget );
	if(widget->parent != NULL
	&& widget->parent->auto_size) {
	/* 如果需要让它的容器能够自动调整大小 */
		Auto_Resize_Widget(widget->parent); 
	}
}

void Enable_Widget_Auto_Size(LCUI_Widget *widget)
/* 功能：启用部件自动尺寸调整功能 */
{
	widget->auto_size = IS_TRUE;
}

void Disable_Widget_Auto_Size(LCUI_Widget *widget)
/* 功能：禁用部件自动尺寸调整功能 */
{
	widget->auto_size = IS_FALSE;
}

void Exec_Refresh_Widget(LCUI_Widget *widget)
/* 功能：执行刷新显示指定部件的整个区域图形的操作 */
{ 
	DEBUG_MSG("refresh widget: %d,%d,%d,%d\n", 
		Get_Widget_Rect(widget).x, Get_Widget_Rect(widget).y, 
		Get_Widget_Rect(widget).width, Get_Widget_Rect(widget).height
	);
	Add_Widget_Refresh_Area(widget->parent, Get_Widget_Rect(widget));
}

void Exec_Draw_Widget(LCUI_Widget *widget)
/* 功能：执行部件图形更新操作 */
{ 
	if(widget == NULL) {
		return;
	}
	
	void ( *func_update ) (LCUI_Widget*); 
	
	if(Graph_Valid(&widget->background_image)) {/* 如果有背景图 */
		/* alpha通道中的每个像素的透明值为255，整个部件的图形不透明 */
		Graph_Fill_Alpha(&widget->graph, 255); 
		Graph_Fill_Image(
				&widget->graph, 
				&widget->background_image, 
				widget->background_image_layout,
				widget->back_color
		); /* 填充背景色 */
	} else {/* 否则根据背景模式来处理 */
		switch(widget->bg_mode) {
		    case BG_MODE_FILL_BACKCOLOR: /* 填充背景色 */
			Graph_Fill_Alpha(&widget->graph, 255);
			Graph_Fill_Color(&widget->graph, widget->back_color); 
			break;
				
		    case BG_MODE_TRANSPARENT: /* 完全透明 */
			Graph_Fill_Alpha(&widget->graph, 0); 
			break;
		    default:break;
		}
	}
	
	/* 获取函数 */
	func_update = Get_WidgetFunc_By_ID( widget->type_id, FUNC_TYPE_UPDATE );
	func_update(widget);
	/* 绘制边框线 */
	if(widget->border_style != BORDER_STYLE_NONE) {
		Graph_Draw_Border( &widget->graph, widget->border_color, 
							widget->border); 
	}
}

void Move_Widget(LCUI_Widget *widget, LCUI_Pos new_pos)
/* 
 * 功能：移动部件位置
 * 说明：如果部件的布局为ALIGN_NONE，那么，就可以移动它的位置，否则，无法移动位置
 * */
{
	if(widget == NULL) {
		return; 
	}
	widget->x.px = new_pos.x;
	widget->y.px = new_pos.y;
	widget->x.which_one = 0;
	widget->y.which_one = 0;
	/* 记录部件的更新数据，等待进行更新 */
	Record_WidgetUpdate(widget, &new_pos, DATATYPE_POS);
}

void Update_Widget_Pos(LCUI_Widget *widget)
/* 功能：更新部件的位置 */
{ 
	Record_WidgetUpdate(widget, NULL, DATATYPE_POS);
}

void Exec_Update_Widget_Pos( LCUI_Widget *widget )
/* 更新部件的位置，以及位置变动范围 */
{
	LCUI_Pos pos; 
	pos = _Get_Widget_Pos( widget ); 
	widget->max_pos = _Get_Widget_MaxPos( widget );
	widget->min_pos = _Get_Widget_MinPos( widget );
	Exec_Move_Widget(widget, pos);
}

void Update_Widget_Size( LCUI_Widget *widget )
/* 部件尺寸更新 */
{
	Resize_Widget( widget, _Get_Widget_Size(widget) ); 
}

void Update_Child_Widget_Size(LCUI_Widget *widget)
/* 
 * 功能：更新指定部件的子部件的尺寸
 * 说明：当部件尺寸改变后，有的部件的尺寸以及位置是按百分比算的，需要重新计算。
 * */
{
	if(widget == NULL) {
		return;
	}
	
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&widget->child); 
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if( !child ) {
			continue;
		}
		Update_Widget_Size( child ); 
		Update_Widget_Pos( child );
	}
}

void Exec_Update_Widget_Size( LCUI_Widget *widget )
{ 
	widget->size = _Get_Widget_Size( widget ); 
	widget->w.px = widget->size.w;
	widget->h.px = widget->size.h;
}

void Update_Child_Widget_Pos(LCUI_Widget *widget)
/* 
 * 功能：更新指定部件的子部件的位置
 * 说明：当作为子部件的容器部件的尺寸改变后，有的部件的布局不为ALIGN_NONE，就需要重新
 * 调整位置。
 * */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		Update_Widget_Pos( child ); 
	}
	/* 更新该部件内定位类型为static的子部件的位置 */
	Update_StaticPosType_ChildWidget( widget );
}


void Set_Widget_Size( LCUI_Widget *widget, char *width, char *height )
/* 
 * 功能：设定部件的尺寸大小
 * 说明：如果设定了部件的停靠位置，并且该停靠类型默认限制了宽/高，那么部件的宽/高就不能被改变。
 * 用法示例：
 * Set_Widget_Size( widget, "100px", "50px" ); 部件尺寸最大为100x50像素
 * Set_Widget_Size( widget, "100%", "50px" ); 部件宽度等于容器宽度，高度为50像素
 * Set_Widget_Size( widget, "50", "50" ); 部件尺寸最大为50x50像素，px可以省略 
 * Set_Widget_Size( widget, NULL, "50%" ); 部件宽度保持原样，高度为容器高度的一半
 * */
{ 
	switch( widget->dock ) {
	    case DOCK_TYPE_TOP:
	    case DOCK_TYPE_BOTTOM: /* 只能改变高 */
		get_PX_P_t( height, &widget->h );
		break;
	    case DOCK_TYPE_LEFT:
	    case DOCK_TYPE_RIGHT:/* 只能改变宽 */
		get_PX_P_t( width, &widget->w );
		break;
	    case DOCK_TYPE_FILL:break;
	    case DOCK_TYPE_NONE: /* 可改变宽和高 */
		get_PX_P_t( width, &widget->w );
		get_PX_P_t( height, &widget->h );
		break;
	} 
	Update_Widget_Size( widget );
}

void Set_Widget_Dock( LCUI_Widget *widget, DOCK_TYPE dock )
/* 设定部件的停靠类型 */
{
	switch( dock ) {
	    case DOCK_TYPE_TOP:
		Set_Widget_Align( widget, ALIGN_TOP_CENTER, Pos(0,0) );
		Set_Widget_Size( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_BOTTOM:
		Set_Widget_Align( widget, ALIGN_BOTTOM_CENTER, Pos(0,0) );
		Set_Widget_Size( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_LEFT:
		Set_Widget_Align( widget, ALIGN_MIDDLE_LEFT, Pos(0,0) );
		Set_Widget_Size( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_RIGHT:
		Set_Widget_Align( widget, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
		Set_Widget_Size( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_FILL:
		Set_Widget_Align( widget, ALIGN_MIDDLE_CENTER, Pos(0,0) );
		Set_Widget_Size( widget, "100%", "100%" );
		break;
	    case DOCK_TYPE_NONE:
		Set_Widget_Align( widget, ALIGN_NONE, Pos(0,0) );
		break;
	}
	widget->dock = dock; 
}


void Offset_Widget_Pos(LCUI_Widget *widget, LCUI_Pos offset)
/* 功能：以部件原有的位置为基础，根据指定的偏移坐标偏移位置 */
{
	Move_Widget( widget, Pos_Add(widget->pos, offset) ); 
}

void Move_Widget_To_Pos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed)
/* 
 * 功能：将部件以指定的速度向指定位置移动 
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * */
{
	if(speed <= 0) {
		return;
	}
	int i, j;
	double w, h, l, n, x, y; 
	x = Get_Widget_Pos(widget).x;
	y = Get_Widget_Pos(widget).y;
	/* 求两点之间的距离 */
	w = des_pos.x-x;
	h = des_pos.y-y;
	l = sqrt(pow(w, 2) + pow(h, 2));
	
	n = l/speed;/* 求移动所需时间 */
	n = n*100;	/* 求移动次数，乘以100，是因为每隔0.01移动一次位置 */
	w = w/n;	/* 计算每次移动的x和y轴坐标的移动距离 */
	h = h/n;
	j = (int)(n>(int)n?n+1:n);
	for(i=0; i<j; i++) {
		x += w;
		y += h;
		Move_Widget(widget, Pos(x, y));
		usleep(10000);/* 停顿0.01秒 */
	}
}

void Refresh_Widget(LCUI_Widget *widget)
/* 功能：刷新显示指定部件的整个区域图形 */
{
	if(widget == NULL) {
		return; 
	}
	Record_WidgetUpdate(widget, NULL, DATATYPE_AREA);
}

void Resize_Widget(LCUI_Widget *widget, LCUI_Size new_size)
/* 功能：改变部件的尺寸 */
{
	if(widget == NULL) {
		return; 
	}
	widget->w.px = new_size.w;
	widget->h.px = new_size.h;
	Record_WidgetUpdate(widget, &new_size, DATATYPE_SIZE);
	if( widget->pos_type == POS_TYPE_STATIC
	 || widget->pos_type == POS_TYPE_RELATIVE ) {
		Record_WidgetUpdate(widget, NULL, DATATYPE_POS_TYPE);
	}
}

void Draw_Widget(LCUI_Widget *widget)
/* 功能：重新绘制部件 */
{
	if(widget == NULL) {
		return; 
	}
	Record_WidgetUpdate(widget, NULL, DATATYPE_GRAPH);
}


void Front_Widget(LCUI_Widget *widget)
/* 功能：将指定部件的显示位置移动到最前端 */
{
	LCUI_Queue *queue;
	/* 获取指向队列的指针 */
	if(widget->parent == NULL) {
		queue = &LCUI_Sys.widget_list;
	} else {
		queue = &widget->parent->child;
	}
	/* 将该部件移动至队列前端 */
	WidgetQueue_Move(queue, 0, widget);
} 

void Show_Widget(LCUI_Widget *widget)
/* 功能：显示部件 */
{
	if(widget == NULL) {
		return; 
	}
	Front_Widget(widget);/* 改变部件的排列位置 */
	Record_WidgetUpdate(widget, NULL, DATATYPE_SHOW);
}

void Hide_Widget(LCUI_Widget *widget)
/* 功能：隐藏部件 */
{
	if(widget == NULL) {
		return; 
	}
	Record_WidgetUpdate(widget, NULL, DATATYPE_HIDE); 
}

void Set_Widget_Status(LCUI_Widget *widget, int status)
/* 功能：设定部件的状态 */
{
	if(widget == NULL) {
		return; 
	}
	Record_WidgetUpdate(widget, &status, DATATYPE_STATUS); 
}
/************************* Widget End *********************************/



/**************************** Widget Update ***************************/
typedef struct _WidgetData
{
	void	*data;	/* 需要更新的数据 */
	int	type;	/* 这个数据的类型 */
}
WidgetData;

static void Destroy_WidgetData(void *arg)
/* 功能：释放WidgetData结构体中的指针 */
{
	WidgetData *wdata = (WidgetData*)arg;
	if(NULL == wdata) return;
	if(wdata->data != NULL) free(wdata->data);
}

static void WidgetData_Init(LCUI_Queue *queue)
/* 功能：初始化记录部件数据更新的队列 */
{
	Queue_Init(queue, sizeof(WidgetData), Destroy_WidgetData);
}

static int Find_WidgetData(LCUI_Widget *widget, const WidgetData *data)
/* 功能：查询指定数据类型是否在队列中 */
{
	WidgetData *temp;
	int i, total;
	total = Queue_Get_Total(&widget->data);	/* 获取成员总数 */ 
	for(i=0; i<total; ++i) {
		temp = (WidgetData*)Queue_Get(&widget->data, i);
		if(NULL == temp) { 
			break;
		} 
		if(temp->type == data->type) {
			return i; 
		}
	}
	return -1;
}

static int Record_WidgetUpdate(LCUI_Widget *widget, void *data, DATATYPE type)
/* 
 * 功能：记录需要进行数据更新的部件
 * 说明：将部件指针以及需更新的数据添加至队列，根据部件的显示顺序来排列队列
 * 返回值：出现问题则返回-1，正常返回不小于0的值
 *  */
{ 
	int pos, result = 0;
	size_t size = 0;
	WidgetData temp;
	/* 根据类型，来得知占用空间大小 */
	switch(type) {
	    case DATATYPE_POS	: size = sizeof(LCUI_Pos);break; /* 位置 */
	    case DATATYPE_SIZE	: size = sizeof(LCUI_Size);break; /* 尺寸 */
	    case DATATYPE_GRAPH	: size = sizeof(LCUI_Graph);break;/* 图形 */
	    case DATATYPE_STATUS: size = sizeof(int);break; /* 状态 */
	    case DATATYPE_AREA:
	    case DATATYPE_HIDE:
	    case DATATYPE_POS_TYPE:
	    case DATATYPE_SHOW:	 break;
	    default: return -1;
	}
	
	temp.type = type;			/* 保存类型 */
	if(data != NULL) { 
		temp.data = malloc(size);	/* 分配内存 */
		memcpy(temp.data, data, size);	/* 拷贝数据 */ 
	} else {
		temp.data = NULL;
	}
	
	pos = Find_WidgetData(widget, &temp);
	if( type == DATATYPE_AREA ) {
		DEBUG_MSG("search result: the recod at %d\n", pos);
	}
	if(pos >= 0) {	/* 如果已经存在，就覆盖 */ 
		result = Queue_Replace(&widget->data, pos, &temp); 
	} else {	/* 否则，追加至队列末尾 */
		result = Queue_Add(&widget->data, &temp);
		if( type == DATATYPE_AREA ) {
			DEBUG_MSG("queue add, the widget data at: %d\n", result);
		}
	}
	return result;
}


int Handle_WidgetUpdate(LCUI_Widget *widget)
/* 功能：处理部件的更新 */
{
	LCUI_Widget *child;
	WidgetData *temp;
	LCUI_Pos pos;
	int i, total;
	/* 处理部件中需要更新的数据 */
	//printf("Handle_WidgetUpdate(): enter\n");
	Queue_Lock(&widget->data);/* 锁定队列，其它线程暂时不能访问 */
	while( ! Queue_Empty(&widget->data) ) {
		temp = (WidgetData *)Queue_Get(&widget->data, 0);
		/* 根据不同的类型来进行处理 */
		switch(temp->type) {
		    case DATATYPE_SIZE	: 
			/* 部件尺寸更新，将更新部件的位置 */ 
			if( temp->data == NULL ) {
				Exec_Update_Widget_Size( widget );
			} else {
				Exec_Resize_Widget(widget, *((LCUI_Size*)temp->data));
			}
			Exec_Draw_Widget(widget);
			/* 需要更新位置，所以不用break */ 
		    case DATATYPE_POS	:
			/*
			 * 由于更新位置可能会是更新部件尺寸的附加操作，需要判断一下更新类型
			 * 是否为DATATYPE_POS 
			 * */
			if( temp->type == DATATYPE_POS 
			 && widget->align == ALIGN_NONE 
			 && temp->data != NULL ) {
				pos = *((LCUI_Pos*)temp->data);
				Exec_Move_Widget(widget, pos);
			} else {
				Exec_Update_Widget_Pos( widget );
			}
			break;
		    case DATATYPE_POS_TYPE:
			Update_StaticPosType_ChildWidget( widget->parent );
			break;
		    case DATATYPE_STATUS:
			widget->status = *(int*)temp->data;
				/* 改变部件状态后需要进行重绘，所以不用break */
		    case DATATYPE_GRAPH	:
			Exec_Draw_Widget(widget); 
			break;
		    case DATATYPE_HIDE:
			Exec_Hide_Widget(widget);
			break;
		    case DATATYPE_SHOW: 
			Exec_Show_Widget(widget); 
			break;
		    case DATATYPE_AREA:
			Exec_Refresh_Widget(widget);
			break;
		    default: break;
		} 
		Queue_Delete(&widget->data, 0);/* 移除该成员 */
	}
	Queue_UnLock(&widget->data);
	total = Queue_Get_Total(&widget->child);  
	for(i=total-1; i>=0; --i) {
		/* 从尾到首获取部件指针 */
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);  
		if(child != NULL) {/* 递归调用 */
			Handle_WidgetUpdate( child );
		}
	}
	//printf("Handle_WidgetUpdate(): quit\n");
	/* 解锁 */
	return 0;
}

void Handle_All_WidgetUpdate()
/* 功能：处理所有部件的更新 */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_Get_Total(&LCUI_Sys.widget_list); 
	//printf("Handle_All_WidgetUpdate():start\n");
	for(i=total-1; i>=0; --i) {
		/* 从尾到首获取部件指针 */
		child = (LCUI_Widget*)Queue_Get(&LCUI_Sys.widget_list, i); 
		//printf("child: ");print_widget_info(child);
		if(child != NULL) {/* 递归调用 */ 
			Handle_WidgetUpdate( child );
		}
	}
	//printf("Handle_All_WidgetUpdate():end\n");
}

/************************ Widget Update End ***************************/



/************************** Widget Library ****************************/

typedef struct _WidgetTypeData
{ 
	LCUI_String		type;	 /* 部件类型，用字符串描述 */
	LCUI_ID		type_id; /* 类型ID */
	LCUI_FuncQueue	func;	 /* 部件相关的函数 */
}WidgetTypeData;

int WidgetFunc_Add(
			const char *type, 
			void (*widget_func)(LCUI_Widget*), 
			FuncType func_type
		)
/*
 * 功能：为指定类型的部件添加相关类型的函数
 * 返回值：部件类型不存在，返回-1，其它错误返回-2
 **/
{
	WidgetTypeData *temp;
	LCUI_Func *temp_func, func_data;
	int total, i, found = 0;
	
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("WidgetFunc_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	//printf("WidgetFunc_Add(): widget type: %s, func type: %d\n", type, func_type); 
	
	total = Queue_Get_Total(&app->widget_lib); 
		/* 遍历数据，找到对应的位置 */
	for(i = 0; i < total; ++i) {
		temp = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(strcmp(temp->type.string, type) == 0) { 
			total = Queue_Get_Total(&temp->func); 
			for(i=0; i<total; i++) {
				temp_func = Queue_Get(&temp->func, i);
				if(temp_func->id == func_type) {
					found = 1;
					break;
				}
			}
			
			Get_FuncData(&func_data, widget_func, NULL, NULL);
			func_data.id = func_type; /* 保存类型ID */
			if(found == 1) {/* 如果已经存在，就覆盖 */
				//printf("WidgetFunc_Add(): the function is already registered. repalce\n");
				Queue_Replace(&temp->func, i, &func_data); 
			} else Queue_Add(&temp->func, &func_data); 
			return 0;
		}
	}
	//printf("WidgetFunc_Add(): warning: the widget type was never recorded\n");
	return -1;
}

int WidgetType_Add(char *type)
/*
 * 功能：添加一个新的部件类型至部件库
 * 返回值：如果添加的新部件类型已存在，返回-1，成功则返回0
 **/
{
	WidgetTypeData *wd, new_wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) {
		printf("WidgetType_Add():"APP_ERROR_UNRECORDED_APP);
		exit(-1);
	}
	
	int total, i; 
	//printf("WidgetType_Add(): add widget type: %s\n", type);
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0) { /* 如果类型一致 */
			//printf("WidgetType_Add(): the widget type is already registered\n");
			return -1;
		}
	} 
	
	/* 以下需要初始化 */
	FuncQueue_Init(&new_wd.func);
	new_wd.type_id = total; /* 保存类型ID */
	//printf("WidgetType_Add(): widget type id: %ld\n", new_wd.type_id); 
	String_Init(&new_wd.type);
	Strcpy(&new_wd.type, type);/* 保存部件类型 */
	Queue_Add(&app->widget_lib, &new_wd);/* 添加至队列 */
	return 0;
}

static void Destroy_WidgetType(void *arg)
/* 功能：移除部件类型数据 */
{
	WidgetTypeData *wd = (WidgetTypeData*)arg;
	Destroy_Queue(&wd->func);
}

void WidgetLib_Init(LCUI_Queue *w_lib)
/* 功能：初始化部件库 */
{
	Queue_Init(w_lib, sizeof(WidgetTypeData), Destroy_WidgetType);
}

int WidgetType_Delete(const char *type)
/* 功能：删除指定部件类型的相关数据 */
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) {
		return -2;
	}
	
	int total,  i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, type) == 0) {/* 如果类型一致 */
			return Queue_Delete(&app->widget_lib, i);
		}
	} 
	
	return -1;
}

void NULL_Widget_Func(LCUI_Widget *widget)
/*
 * 功能：空函数，不做任何操作
 * 说明：如果获取指定部件类型的函数指针失败，将返回这个函数的函数指针
 **/
{
	
}

LCUI_ID WidgetType_Get_ID(const char *widget_type)
/* 功能：获取指定类型部件的类型ID */
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0) { /* 如果类型一致 */
			return wd->type_id;
		}
	}
	
	return -1;
}

int Get_Widget_Type_By_ID(LCUI_ID id, char *widget_type)
/* 功能：获取指定类型ID的类型名称 */
{
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) return -2;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) { /* 如果类型一致 */
			strcpy(widget_type, wd->type.string); 
			return 0;
		}
	}
	
	return -1;
}

void ( *Get_WidgetFunc_By_ID(LCUI_ID id, FuncType func_type) ) (LCUI_Widget*)
/*
 * 功能：获取指定部件类型ID的函数的函数指针
 **/
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) 
		return NULL_Widget_Func;
	
	int total, i, found = 0; 
	//printf("Get_WidgetFunc_By_ID(): widget type id: %lu, func type: %d\n", id, func_type);
	total = Queue_Get_Total(&app->widget_lib); 
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(wd->type_id == id) { /* 如果类型一致 */  
			total = Queue_Get_Total(&wd->func); 
			for(i=0; i<total; i++) {
				f = Queue_Get(&wd->func, i); 
				if(f->id == func_type) {
					found = 1;
					break;
				}
			}
			/* 如果已经存在 */
			if(found == 1) {
				return f->func; 
			} else {
				//printf("Get_WidgetFunc_By_ID(): warning: widget func not found!\n");
				return NULL_Widget_Func;
			}
		}
	}
	
	return NULL_Widget_Func;
}

void ( *Get_WidgetFunc(const char *widget_type, FuncType func_type) ) (LCUI_Widget*)
/* 功能：获取指定类型部件的函数的函数指针 */
{
	LCUI_Func *f = NULL; 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	if(app == NULL) {
		return NULL_Widget_Func;
	}
	
	int total, i, found = 0; 
	
	total = Queue_Get_Total(&app->widget_lib);
	//printf("Get_WidgetFunc(): widget type: %s, func type: %d\n", widget_type, func_type);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0) { /* 如果类型一致 */ 
			total = Queue_Get_Total(&wd->func);
			for(i=0; i<total; i++) {
				f = Queue_Get(&wd->func, i);
				if(f->id == func_type) {
					found = 1;
					break;
				}
			}
			/* 如果已经存在 */
			if(found == 1) {
				return f->func; 
			} else {
				//printf("Get_WidgetFunc(): warning: widget func not found!\n");
				return NULL_Widget_Func; 
			}
		}
	}
	
	return NULL_Widget_Func;
}

int WidgetType_Valid(const char *widget_type)
/* 功能：检测指定部件类型是否有效 */
{ 
	WidgetTypeData *wd;
	LCUI_App *app = Get_Self_AppPointer();
	
	if(app == NULL) return 0;
	
	int total, i; 
	
	total = Queue_Get_Total(&app->widget_lib);
	for(i = 0; i < total; ++i) {
		wd = (WidgetTypeData*)Queue_Get(&app->widget_lib, i);
		if(Strcmp(&wd->type, widget_type) == 0)/* 如果类型一致 */ 
			return 1; 
	}
	
	return 0;
}


extern void Register_Window();
extern void Register_Label();
extern void Register_Button();
extern void Register_PictureBox();
extern void Register_ProgressBar();
extern void Register_Menu();
extern void Register_CheckBox();
extern void Register_RadioButton();
extern void Register_ActiveBox();
extern void Register_TextBox();

void Register_Default_Widget_Type()
/* 功能：为程序的部件库添加默认的部件类型 */
{
	WidgetType_Add(NULL); /* 添加一个NULL类型的部件 */
	Register_Window();/* 注册窗口部件 */
	Register_Label();/* 注册文本标签部件 */
	Register_Button();/* 注册按钮部件 */
	Register_PictureBox();/* 注册图片盒子部件 */
	Register_ProgressBar();
	Register_Menu();
	Register_CheckBox();
	Register_RadioButton();
	Register_ActiveBox();
	Register_TextBox();
}
/************************ Widget Library End **************************/


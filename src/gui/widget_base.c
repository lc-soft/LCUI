/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
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
 * widget_base.c -- 部件的基本操作集。
 *
 * 版权所有 (C) 2012-2013 归属于
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
#include LC_DISPLAY_H
#include LC_FONT_H
#include LC_ERROR_H
#include LC_CURSOR_H
#include LC_INPUT_H

#include <math.h>
#include <limits.h>

static LCUI_Widget root_widget;

/*************************** Container ********************************/
/* 将部件添加至作为容器的部件内 */
LCUI_API int
Widget_Container_Add( LCUI_Widget *ctnr, LCUI_Widget *widget )
{
	int pos;
	LCUI_GraphLayer *ctnr_glayer;
	LCUI_Queue *old_queue, *new_queue;

	//_DEBUG_MSG("ctnr widget: %p, widget: %p\n", ctnr, widget);
	if( !widget || ctnr == widget->parent ) {
		return -1;
	}
	new_queue = Widget_GetChildList( ctnr );
	ctnr_glayer = Widget_GetGraphLayer( ctnr );
	/* 如果部件有父部件，那就在父部件的子部件队列中 */
	if( widget->parent ) {
		old_queue = &widget->parent->child;
	} else {/* 否则没有部件，那么这个部件在创建时就储存至系统部件队列中 */
		old_queue = &root_widget.child;
	}
	/* 若部件已获得过焦点，则复位之前容器中的焦点 */
	if( Widget_GetFocus( widget ) ) {
		Reset_Focus( widget->parent );
	}

	/* 改变该部件的容器，需要将它从之前的容器中移除 */
	pos = Queue_Find( old_queue, widget );
	if(pos >= 0) {
		Queue_DeletePointer(old_queue, pos);
	}

	widget->parent = ctnr; /* 保存父部件指针 */
	Queue_AddPointer( new_queue, widget ); /* 添加至部件队列 */
	/* 将部件图层移动至新的父图层内 */
	GraphLayer_MoveChild( ctnr_glayer, widget->glayer );
	Widget_UpdatePos( widget );
	return 0;
}

LCUI_API int
_Widget_GetContainerWidth(LCUI_Widget *widget)
/* 通过计算得出指定部件的容器的宽度，单位为像素 */
{
	int width;
	widget = widget->parent;
	if( !widget ) {
		return LCUIScreen_GetWidth();
	}
	if( widget->w.which_one == 0 ) {
		width = widget->w.px;
	} else {
		width = _Widget_GetContainerWidth( widget );
		width *= widget->w.scale;
	}
	width -= widget->glayer->padding.left;
	width -= widget->glayer->padding.right;
	return width;
}

LCUI_API int
_Widget_GetContainerHeight(LCUI_Widget *widget)
/* 通过计算得出指定部件的容器的高度，单位为像素 */
{
	int height;
	widget = widget->parent;
	if( !widget ) {
		return LCUIScreen_GetHeight();
	}
	if( widget->h.which_one == 0 ) {
		height = widget->h.px;
	} else {
		height = _Widget_GetContainerHeight( widget );
		height *= widget->h.scale;
	}
	height -= widget->glayer->padding.top;
	height -= widget->glayer->padding.bottom;
	return height;
}

/* 获取容器的宽度 */
LCUI_API int
Widget_GetContainerWidth( LCUI_Widget *widget )
{
	int width;
	if( !widget ) {
		return LCUIScreen_GetWidth();
	}
	width = _Widget_GetWidth( widget );
	width -= (widget->glayer->padding.left + widget->glayer->padding.right);
	return width;
}

/* 获取容器的高度 */
LCUI_API int
Widget_GetContainerHeight( LCUI_Widget *widget )
{
	int height;
	if( !widget ) {
		return LCUIScreen_GetHeight();
	}
	height = _Widget_GetHeight( widget );
	height -= (widget->glayer->padding.top + widget->glayer->padding.bottom);
	return height;
}

/* 获取容器的尺寸 */
LCUI_API LCUI_Size
Widget_GetContainerSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = Widget_GetContainerWidth( widget );
	size.h = Widget_GetContainerHeight( widget );
	return size;
}
/************************* Container End ******************************/

/***************************** Widget *********************************/

/* 获取部件图层指针 */
LCUI_API LCUI_GraphLayer *
Widget_GetGraphLayer( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return root_widget.glayer;
	}
	return widget->glayer;
}

/* 获取部件的子部件队列 */
LCUI_API LCUI_Queue*
Widget_GetChildList( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return &root_widget.child;
	}
	return &widget->child;
}

/* 获取部件的矩形区域队列 */
LCUI_API LCUI_RectQueue*
Widget_GetInvalidAreaQueue( LCUI_Widget *widget )
{
	if( widget == NULL ) {
		return &root_widget.invalid_area;
	}
	return &widget->invalid_area;
}

/*-------------------------- Widget Pos ------------------------------*/
LCUI_API int
_Widget_GetX( LCUI_Widget *widget )
/* 通过计算得出部件的x轴坐标 */
{
	if(widget->x.which_one == 0) {
		return widget->x.px;
	}
	return widget->x.scale * _Widget_GetContainerWidth( widget );
}
LCUI_API int
_Widget_GetY( LCUI_Widget *widget )
/* 通过计算得出部件的y轴坐标 */
{
	if(widget->x.which_one == 0) {
		return widget->y.px;
	}
	return widget->y.scale * _Widget_GetContainerHeight( widget );
}

static int
_get_x_overlay_widget( LCUI_Queue *list, LCUI_Rect area, LCUI_Queue *out_data )
/* 获取在x轴上指定该区域重叠的部件 */
{
	int i, n, len;
	LCUI_Widget *wptr;

	if( Queue_GetTotal( out_data ) > 0 ) {
		Queue_Destroy( out_data );
	}
	len = Queue_GetTotal( list );
	for(i=0,n=0; i<len; ++i) {
		wptr = Queue_Get( list, i );
		/* 如果当前部件区域与area 在x轴上重叠，就添加至队列 */
		if( wptr->pos.x < area.x+area.width
		 && wptr->pos.x >= area.x ) {
			Queue_AddPointer( out_data, wptr );
			++n;
		}
	}
	return n;
}

static LCUI_Widget*
_get_max_y_widget( LCUI_Queue *widget_list )
/* 获取底边y轴坐标最大的部件 */
{
	int total, i, y, max_y;
	LCUI_Widget *widget, *tmp;

	total = Queue_GetTotal( widget_list );
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

static void
_move_widget( LCUI_Widget *widget, LCUI_Pos new_pos )
{
	static LCUI_Pos tmp_pos;
	if( widget->pos_type == POS_TYPE_RELATIVE ) {
		/* 如果是RELATIVE定位类型，需要加上偏移坐标 */
		tmp_pos = Pos_Add(new_pos, widget->offset);
		Widget_Move( widget, tmp_pos );
	} else {
		Widget_Move( widget, new_pos );
	}
}

static void
Widget_UpdateChildStaticPos( LCUI_Widget *widget )
/* 更新使用static定位类型的子部件 */
{
	//_DEBUG_MSG("enter\n");
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
	Queue_UsingPointer( &widget_list );
	Queue_UsingPointer( old_row );
	Queue_UsingPointer( cur_row );

	if( !widget ) {
		queue = &root_widget.child;
	} else {
		queue = &widget->child;
	}
	container_size = Widget_GetContainerSize( widget );
	//_DEBUG_MSG("container size: %d,%d\n", container_size.w, container_size.h);
	total = Queue_GetTotal( queue );
	//_DEBUG_MSG("queue total: %d\n", total);
	for(i=total-1,y=0,x=0; i>=0; --i) {
		wptr = Queue_Get( queue, i );
		/* 过滤掉定位类型不是static和relative的部件 */
		if(wptr->pos_type != POS_TYPE_STATIC
		&& wptr->pos_type != POS_TYPE_RELATIVE ) {
			continue;
		}
		//_DEBUG_MSG("widget: %p\n", wptr);
		if( new_pos.x == 0 && wptr->size.w > container_size.w ) {
			new_pos.x = x = 0;
			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("width > container width, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
			/* 更新y轴坐标 */
			new_pos.y += wptr->size.h;
		}
		//_DEBUG_MSG("0, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		if( y == 0 ) {/* 如果还在第一行，就直接记录部件 */
			//_DEBUG_MSG("1, %d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
			if(new_pos.x + wptr->size.w > container_size.w) {
				/* 如果超出容器范围，就开始下一行记录 */
				new_pos.x = x = 0;
				Queue_Destroy( old_row );
				tmp_row = old_row;
				old_row = cur_row;
				cur_row = tmp_row;
				tmp = Queue_Get( old_row, x );
				if( tmp ) {
					new_pos.y = tmp->y.px + tmp->size.h;
				}
				++y; ++i;
				/* 在下次循环里再次处理该部件 */
				//_DEBUG_MSG("1.1, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
				continue;
			}

			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("1.2, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
			/* 更新x轴坐标 */
			new_pos.x += wptr->size.w;
			continue;
		}
		//_DEBUG_MSG("2,%d + %d > %d\n", new_pos.x, wptr->size.w, container_size.w);
		/* 如果当前部件区块超出容器范围，y++，开始在下一行记录部件指针。*/
		if(new_pos.x + wptr->size.w > container_size.w) {
			new_pos.x = x = 0;
			Queue_Destroy( old_row );
			tmp_row = old_row;
			old_row = cur_row;
			cur_row = tmp_row;
			tmp = Queue_Get( old_row, x );
			if( tmp ) {
				new_pos.y = tmp->pos.y + tmp->size.h;
			}
			++y;
		}
		//_DEBUG_MSG("2, [%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		/* 保存在新位置的部件的区域 */
		area = Rect( new_pos.x, new_pos.y, wptr->size.w, wptr->size.h );
		/* 如果有上一行记录,获取上面几行与当前部件在x轴上重叠的部件列表 */
		n = _get_x_overlay_widget( cur_row, area, &widget_list );
		//printf("n: %d\n", n);
		if( n <= 0 ) { /* 如果上一行没有与之重叠的部件 */
			Queue_AddPointer( cur_row, wptr );
			//_DEBUG_MSG("3,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
			_move_widget( wptr, new_pos );
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
				Queue_AddPointer( cur_row, tmp2 );
				if(tmp2 == tmp) {
					break;
				}
			}
			new_pos.x = tmp->x.px + tmp->size.w;
			continue;
		}
		new_pos.y = tmp->y.px+tmp->size.h;
		Queue_AddPointer( cur_row, wptr );
		//_DEBUG_MSG("4,[%d], pos: %d,%d\n", i, new_pos.x, new_pos.y);
		_move_widget( wptr, new_pos );
		new_pos.x = tmp->x.px + wptr->size.w;
	}
	//_DEBUG_MSG("quit\n");
}

LCUI_API LCUI_Pos
_Widget_GetPos(LCUI_Widget *widget)
/* 功能：通过计算得出部件的位置，单位为像素 */
{
	LCUI_Pos pos;
	LCUI_Size size;
	pos.x = pos.y = 0;

	switch(widget->pos_type) {
	    case POS_TYPE_STATIC:
	    case POS_TYPE_RELATIVE:
		pos.x = _Widget_GetX( widget );
		pos.y = _Widget_GetY( widget );
		break;

	    default:
		if(widget->align == ALIGN_NONE) {
			pos.x = _Widget_GetX( widget );
			pos.y = _Widget_GetY( widget );
			break;
		}
		size = Widget_GetContainerSize( widget->parent );
		pos = GetPosByAlign(size, Widget_GetSize(widget), widget->align);
		/* 加上偏移距离 */
		pos = Pos_Add(pos, widget->offset);
		break;
	}
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetPos(LCUI_Widget *widget)
/*
 * 功能：获取部件相对于容器部件的位置
 * 说明：该位置相对于容器部件的左上角点，忽略容器部件的内边距。
 *  */
{
	return widget->pos;
}

LCUI_API LCUI_Pos
Widget_GetRelPos(LCUI_Widget *widget)
/*
 * 功能：获取部件的相对于所在容器区域的位置
 * 说明：部件所在容器区域并不一定等于容器部件的区域，因为容器区域大小受到
 * 容器部件的内边距的影响。
 *  */
{
	return Pos(widget->x.px, widget->x.px);
}

LCUI_API LCUI_Pos
Widget_ToRelPos(LCUI_Widget *widget, LCUI_Pos global_pos)
/*
 * 功能：全局坐标转换成相对坐标
 * 说明：传入的全局坐标，将根据传入的部件指针，转换成相对于该部件的坐标
 *  */
{
	if( widget == NULL || widget->parent == NULL) {
		return global_pos;
	}
	//widget = widget->parent;
	while( widget ) {
		global_pos.x -= widget->glayer->padding.left;
		global_pos.y -= widget->glayer->padding.top;
		global_pos.x -= widget->pos.x;
		global_pos.y -= widget->pos.y;
		widget = widget->parent;
	}
	return global_pos;
}

LCUI_API int
_Widget_GetMaxX( LCUI_Widget *widget )
{
	if(widget->max_x.which_one == 0) {
		return widget->max_x.px;
	}
	return widget->max_x.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMaxY( LCUI_Widget *widget )
{
	if(widget->max_y.which_one == 0) {
		return widget->max_y.px;
	}
	return widget->max_y.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Pos
_Widget_GetMaxPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Widget_GetMaxX( widget );
	pos.y = _Widget_GetMaxY( widget );
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetMaxPos(LCUI_Widget *widget)
{
	return widget->max_pos;
}

LCUI_API int
_Widget_GetMinX( LCUI_Widget *widget )
{
	if(widget->min_x.which_one == 0) {
		return widget->min_x.px;
	}
	return widget->min_x.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMinY( LCUI_Widget *widget )
{
	if(widget->min_y.which_one == 0) {
		return widget->min_y.px;
	}
	return widget->min_y.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Pos
_Widget_GetMinPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;
	pos.x = _Widget_GetMinX( widget );
	pos.y = _Widget_GetMinY( widget );
	return pos;
}

LCUI_API LCUI_Pos
Widget_GetMinPos(LCUI_Widget *widget)
{
	return widget->min_pos;
}
/*------------------------ END Widget Pos -----------------------------*/

/*------------------------- Widget Size -------------------------------*/
LCUI_API int
_Widget_GetMaxWidth( LCUI_Widget *widget )
{
	if(widget->max_w.which_one == 0) {
		return widget->max_w.px;
	}
	return widget->max_w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMaxHeight( LCUI_Widget *widget )
{
	if(widget->max_h.which_one == 0) {
		return widget->max_h.px;
	}
	return widget->max_h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API LCUI_Size
_Widget_GetMaxSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Widget_GetMaxWidth( widget );
	size.h = _Widget_GetMaxHeight( widget );
	return size;
}

LCUI_API LCUI_Size
Widget_GetMaxSize( LCUI_Widget *widget )
{
	return widget->max_size;
}

LCUI_API int
_Widget_GetMinWidth( LCUI_Widget *widget )
{
	if(widget->min_w.which_one == 0) {
		return widget->min_w.px;
	}
	return widget->min_w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
_Widget_GetMinHeight( LCUI_Widget *widget )
{
	if(widget->min_h.which_one == 0) {
		return widget->min_h.px;
	}
	return widget->min_h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API int
_Widget_GetHeight(LCUI_Widget *widget)
{
	if(widget->h.which_one == 0) {
		return widget->h.px;
	}
	return widget->h.scale * _Widget_GetContainerHeight( widget );
}

LCUI_API int
_Widget_GetWidth(LCUI_Widget *widget)
{
	if(widget->w.which_one == 0) {
		return widget->w.px;
	}
	return widget->w.scale * _Widget_GetContainerWidth( widget );
}

LCUI_API int
Widget_GetHeight(LCUI_Widget *widget)
{
	return widget->size.h;
}

LCUI_API int
Widget_GetWidth(LCUI_Widget *widget)
{
	return widget->size.w;
}

LCUI_API LCUI_Size
_Widget_GetMinSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size.w = _Widget_GetMinWidth( widget );
	size.h = _Widget_GetMinHeight( widget );
	return size;
}

LCUI_API LCUI_Size
Widget_GetMinSize(LCUI_Widget *widget)
{
	return widget->min_size;
}

LCUI_API LCUI_Size
Widget_GetSize(LCUI_Widget *widget)
/* 功能：获取部件的尺寸 */
{
	return widget->size;
}

LCUI_API LCUI_Size
_Widget_GetSize(LCUI_Widget *widget)
/* 功能：通过计算获取部件的尺寸 */
{
	LCUI_Size size;
	size.w = _Widget_GetWidth( widget );
	size.h = _Widget_GetHeight( widget );
	return size;
}

LCUI_API LCUI_Rect
Widget_GetRect(LCUI_Widget *widget)
/* 功能：获取部件的区域 */
{
	return Rect(widget->pos.x, widget->pos.y,
			widget->size.w, widget->size.h);
}

/*------------------------ END Widget Size ----------------------------*/

LCUI_API void*
Widget_GetPrivData(LCUI_Widget *widget)
/* 功能：获取部件的私有数据结构体的指针 */
{
	if( !widget ) {
		return NULL;
	}
	return widget->private_data;
}

LCUI_API LCUI_Widget*
Get_Widget_Parent(LCUI_Widget *widget)
/* 功能：获取部件的父部件 */
{
	return widget->parent;
}

LCUI_API int
Widget_PrintChildList( LCUI_Widget *widget )
{
	int i, n;
	LCUI_Queue *child_list;
	LCUI_Widget *child;
	child_list = Widget_GetChildList( widget );
	if(child_list == NULL) {
		return -1;
	}
	n = Queue_GetTotal( child_list );
	for(i=0; i<n; ++i) {
		child = (LCUI_Widget*)Queue_Get( child_list, i );
		if( child == NULL ) {
			continue;
		}
		printf("[%d] widget: %p, type: %s, visible: %s, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			i, child, child->type_name.string, child->visible?"TRUE":"FALSE",
			child->glayer->z_index, child->pos.x, child->pos.y,
			child->size.w, child->size.h);
	}
	return 0;
}

LCUI_API void
print_widget_info(LCUI_Widget *widget)
/*
 * 功能：打印widget的信息
 * 说明：在调试时需要用到它，用于确定widget是否有问题
 *  */
{
	int i, n;
	LCUI_Widget *child;
	LCUI_Queue *child_list;
	if( widget ) {
		if( widget->parent ) {
			child_list = &widget->parent->child;
		} else {
			child_list = &root_widget.child;
		}
		n = Queue_GetTotal( child_list );
		for(i=0; i<n; ++i) {
			child = Queue_Get( child_list, i );
			if( child == widget ) {
				break;
			}
		}
		printf("widget: %p, type: %s, visible: %d, show pos: %d, z-index: %d, pos: (%d,%d), size: (%d, %d)\n",
			widget, widget->type_name.string, widget->visible,
			i, widget->glayer->z_index, widget->pos.x, widget->pos.y,
			widget->size.w, widget->size.h);
	} else {
		printf("NULL widget\n");
	}
}

/* 在指定部件的内部区域内设定需要刷新的区域 */
LCUI_API int
Widget_InvalidArea( LCUI_Widget *widget, LCUI_Rect rect )
{
	if( !widget ) {
		return LCUIScreen_InvalidArea( rect );
	}
	if (rect.width <= 0 || rect.height <= 0) {
		return -1;
	}
	/* 加上内边距 */
	rect.x += widget->glayer->padding.left;
	rect.y += widget->glayer->padding.top;
	/* 根据部件所在容器的尺寸，调整矩形位置及尺寸 */
	rect = LCUIRect_ValidArea( Widget_GetSize(widget), rect );
	if( widget->visible ) {
		LCUIScreen_MarkSync();
	}
	/* 保存至队列中 */
	RectQueue_AddToValid( &widget->invalid_area, rect );
	return 0;
}

/*
 * global_pos: 当前部件的全局坐标
 * valid_area: 当前所在容器的有效显示区域
 * */
static void
__Widget_SyncInvalidArea(	LCUI_Widget *widget,
				LCUI_Pos global_pos,
				LCUI_Rect valid_area )
{
	int n;
	LCUI_Pos point;
	LCUI_Queue *widget_list;
	LCUI_Widget *child;
	LCUI_Rect rect;
	LCUI_RectQueue *rect_queue;

	rect_queue = Widget_GetInvalidAreaQueue( widget );
	point.x = valid_area.x + valid_area.width;
	point.y = valid_area.y + valid_area.height;
	RectQueue_Switch( rect_queue );
	while( RectQueue_GetFromCurrent( rect_queue, &rect ) ) {
		/* 加上当前部件的全局坐标 */
		rect.x += global_pos.x;
		rect.y += global_pos.y;
		/* 根据当前有效区域，对取出的区域进行调整 */
		if( rect.x < valid_area.x ) {
			rect.width -= (valid_area.x - rect.x);
			rect.x = valid_area.x;
		}
		if( rect.y < valid_area.y ) {
			rect.height -= (valid_area.y - rect.y);
			rect.y = valid_area.y;
		}
		if( rect.x + rect.width > point.x ) {
			rect.width = point.x - rect.x;
		}
		if( rect.y + rect.height > point.y ) {
			rect.height = point.y - rect.y;
		}
		/* 添加至屏幕无效区域队列 */
		LCUIScreen_InvalidArea( rect );
	}
	/* 根据当前部件的区域，计算有效显示区域 */
	if( global_pos.x > valid_area.x ) {
		valid_area.width -= (global_pos.x - valid_area.x);
		valid_area.x = global_pos.x;
	}
	if( global_pos.y > valid_area.y ) {
		valid_area.height -= (global_pos.y - valid_area.y);
		valid_area.y = global_pos.y;
	}
	if( global_pos.x + widget->size.w < point.x ) {
		valid_area.width = global_pos.x + widget->size.w - valid_area.x;
	}
	if( global_pos.y + widget->size.h < point.y ) {
		valid_area.height = global_pos.y + widget->size.h - valid_area.y;
	}
	widget_list = Widget_GetChildList( widget );
	n = Queue_GetTotal( widget_list );
	while(n--) {
		child = (LCUI_Widget*)Queue_Get( widget_list, n );
		if( !child || !child->visible ) {
			continue;
		}
		point.x = global_pos.x + child->pos.x;
		point.y = global_pos.y + child->pos.y;
		point.x += widget->glayer->padding.left;
		point.y += widget->glayer->padding.top;
		__Widget_SyncInvalidArea( child, point, valid_area );
	}
}

/* 将所有可见部件的无效区域 同步至 屏幕无效区域队列中 */
LCUI_API void
Widget_SyncInvalidArea( void )
{
	LCUI_Rect valid_area;
	valid_area.x = valid_area.y = 0;
	valid_area.width = LCUIScreen_GetWidth();
	valid_area.height = LCUIScreen_GetHeight();
	__Widget_SyncInvalidArea( &root_widget, Pos(0,0), valid_area );
}

/*
 * 功能：让指定部件响应部件状态的改变
 * 说明：部件创建时，默认是不响应状态改变的，因为每次状态改变后，都要调用函数重绘部件，
 * 这对于一些部件是多余的，没必要重绘，影响效率。如果想让部件能像按钮那样，鼠标移动到它
 * 上面时以及鼠标点击它时，都会改变按钮的图形样式，那就需要用这个函数设置一下。
 * 用法：
 * Widget_SetValidState( widget, WIDGET_STATE_NORMAL );
 * Widget_SetValidState( widget, WIDGET_STATE_OVERLAY | WIDGET_STATE_ACTIVE );
 * */
LCUI_API void
Widget_SetValidState( LCUI_Widget *widget, int state )
{
	widget->valid_state = state;
}

/*
 * 功能：获取部件的指定类型的父部件的指针
 * 说明：本函数会在部件关系链中往头部查找父部件指针，并判断这个父部件是否为制定类型
 * 返回值：没有符合要求的父级部件就返回NULL，否则返回部件指针
 **/
LCUI_API LCUI_Widget*
Widget_GetParent(LCUI_Widget *widget, char *widget_type)
{
	LCUI_Widget *temp;
	if( !widget ) {
		return NULL; /* 本身是NULL就退出函数 */
	}
	temp = widget;
	while( temp->parent ) {
		if(temp->parent
		  && _LCUIString_Cmp(&temp->parent->type_name, widget_type) == 0
		) {/* 如果指针有效，并且类型符合要求 */
			return temp->parent; /* 返回部件的指针 */
		}
		temp = temp->parent;/* 获取部件的父级部件指针 */
	}
	return NULL;
}

/* 获取指定ID的子部件 */
LCUI_API LCUI_Widget*
Widget_GetChildByID( LCUI_Widget *widget, LCUI_ID id )
{
	int i, n;
	LCUI_Widget *child;
	LCUI_Queue *child_list;

	if( widget ) {
		child_list = &widget->child;
	} else {
		child_list = &root_widget.child;
	}
	n = Queue_GetTotal( child_list );
	for(i=0; i<n; ++i) {
		child = Queue_Get( child_list, i );
		if( !child ) {
			continue;
		}
		if( child->self_id == id ) {
			return child;
		}
	}
	return NULL;
}

/* 销毁指定ID的程序的所有部件 */
LCUI_API void
LCUIApp_DestroyAllWidgets( LCUI_ID app_id )
{
	int i, total;
	LCUI_Widget *temp;
	
	Queue_Lock( &root_widget.child );
	total = Queue_GetTotal(&root_widget.child);
	for(i=0; i<total; i++) {
		temp = (LCUI_Widget*)Queue_Get(&root_widget.child,i);
		if( temp == NULL ) {
			continue;
		}
		if(temp->app_id != app_id) {
			continue;
		}
		/*
		 * 在Queue_Delete()函数将队列中的部件移除时，会调用初始化部件队列时指
		 * 定的Destroy_Widget()函数进行部件数据相关的清理。
		 * */
		Queue_Delete( &root_widget.child, i );
		/* 重新获取部件总数 */
		total = Queue_GetTotal(&root_widget.child);
		--i;/* 当前位置的部件已经移除，空位已由后面部件填补，所以，--i */
	}
	Queue_Unlock( &root_widget.child );
}


/* 检测指定部件是否处于焦点状态 */
LCUI_API LCUI_BOOL
Widget_GetFocus( LCUI_Widget *widget )
{
	LCUI_Widget *tmp;

	if( !widget ) {
		return FALSE;
	}
	tmp = Get_FocusWidget( widget->parent );
	if( tmp == widget ) {
		return TRUE;
	}
	return FALSE;
}

/* 检测指定部件是否获得全局焦点，也就是该部件以及上级所有父部件是否都得到了焦点 */
LCUI_API LCUI_BOOL
Widget_GetGlobalFocus( LCUI_Widget *widget )
{
	if( Widget_GetFocus(widget) ) {
		if( widget->parent == NULL
		 || widget->parent == &root_widget ) {
			return TRUE;
		}
		return Widget_GetFocus( widget->parent );
	}
	return FALSE;
}

/* 获取部件的风格名称 */
LCUI_API LCUI_String
Widget_GetStyleName( LCUI_Widget *widget )
{
	return widget->style_name;
}

/* 设定部件的风格名称 */
LCUI_API void
Widget_SetStyleName( LCUI_Widget *widget, const char *style_name )
{
	_LCUIString_Copy(&widget->style_name, style_name);
	Widget_Draw( widget );
}

/* 设定部件的风格ID */
LCUI_API void
Widget_SetStyleID( LCUI_Widget *widget, int style_id )
{
	widget->style_id = style_id;
	Widget_Draw( widget );
}

static LCUI_Widget *
__Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos )
{
	int i, total, temp;
	LCUI_Widget *child, *widget;
	LCUI_Queue *widget_list;
	LCUI_Pos tmp_pos;
	LCUI_RGBA pixel;
	LCUI_Graph *graph;

	if( !ctnr ) {
		return NULL;
	}
	widget_list = &ctnr->child;
	/* 判断 鼠标坐标对应部件图层中的像素点的透明度 是否符合要求，
		* 如果透明度小于/不小于clickable_area_alpha的值，那么，无视
		* 该部件。
		*  */
	graph = Widget_GetSelfGraph( ctnr );
	if( Graph_GetPixel( graph, pos, &pixel )) {
		DEBUG_MSG("%p, mode: %d, pixel alpha: %d, alpha: %d\n", ctnr, 
		ctnr->clickable_mode, pixel.alpha, ctnr->clickable_area_alpha );
		if( (ctnr->clickable_mode == 0
			&& pixel.alpha < ctnr->clickable_area_alpha )
			|| (ctnr->clickable_mode == 1
			&& pixel.alpha >= ctnr->clickable_area_alpha ) ) {
			//printf("Ignore widget\n");
			return NULL;
		}
	}/* else {
		printf("get graph pixel error\n");
	} */
	/* 减去内边距 */
	pos.x -= ctnr->glayer->padding.left;
	pos.y -= ctnr->glayer->padding.top;

	widget = ctnr;
	total = Queue_GetTotal( widget_list );
	for(i=0; i<total; ++i) {/* 从顶到底遍历子部件 */
		child = Queue_Get( widget_list, i );
		if( !child || !child->visible ) {
			continue;
		}
		temp = LCUIRect_IncludePoint( pos, Widget_GetRect(child) );
		/* 如果这个点没被包含在部件区域内 */
		if( !temp ) {
			continue;
		}
		/* 改变相对坐标 */
		tmp_pos.x = pos.x - child->pos.x;
		tmp_pos.y = pos.y - child->pos.y;
		widget = __Widget_At( child, tmp_pos );
		if( widget == NULL ) {
			widget = ctnr;
		}
		break;
	}
	return widget;
}

/* 获取指定坐标上的子部件，有则返回子部件指针，否则返回NULL */
LCUI_API LCUI_Widget*
Widget_At( LCUI_Widget *ctnr, LCUI_Pos pos )
{
	LCUI_Widget *widget;
	if( ctnr == NULL ) {
		ctnr = &root_widget;
	}
	widget = __Widget_At( ctnr, pos );
	if( widget == ctnr ) {
		return NULL;
	}
	return widget;

}

LCUI_API int
Widget_IsActive(LCUI_Widget *widget)
/* 功能：判断部件是否为活动状态 */
{
	if(widget->state != KILLED) {
		return 1;
	}
	return 0;
}

LCUI_API int
Empty_Widget(void)
/*
 * 功能：用于检测程序的部件列表是否为空
 * 返回值：
 *   1  程序的部件列表为空
 *   0  程序的部件列表不为空
 * */
{
	if(Queue_GetTotal(&root_widget.child) <= 0) {
		return 1;
	}
	return 0;
}

/* 功能：为部件私有结构体指针分配内存 */
LCUI_API void*
WidgetPrivData_New( LCUI_Widget *widget, size_t size )
{
	widget->private_data = malloc(size);
	return widget->private_data;
}

static void Widget_BackgroundInit( LCUI_Widget *widget )
{
	Graph_Init( &widget->background.image );
	widget->background.color = RGB(255,255,255);
	widget->background.transparent = TRUE;
	widget->background.layout = LAYOUT_NONE;
}


static void
Destroy_Widget( void *arg )
/*
 * 功能：销毁一个部件
 * 说明：如果这个部件有子部件，将对它进行销毁
 * */
{
	LCUI_Widget *widget;
	widget = (LCUI_Widget *)arg;
	widget->parent = NULL;

	/* 释放字符串 */
	LCUIString_Free(&widget->type_name);
	LCUIString_Free(&widget->style_name);
	/* 在移除图层前，先锁上图层树的互斥锁 */
	LCUIScreen_LockGraphLayerTree();
	/* 开始移除该部件的图层记录，并释放内存资源 */
	GraphLayer_Free( widget->glayer );
	/* 移除后，解除互斥锁 */
	LCUIScreen_UnlockGraphLayerTree();
	/* 销毁部件的队列 */
	Queue_Destroy(&widget->child);
	Queue_Destroy(&widget->event);
	Queue_Destroy(&widget->msg_buff);
	Queue_Destroy(&widget->msg_func);
	RectQueue_Destroy(&widget->invalid_area);

	widget->visible = FALSE;
	widget->enabled = TRUE;
	/* 调用回调函数销毁部件私有数据 */
	WidgetFunc_Call( widget, FUNC_TYPE_DESTROY );
	free( widget->private_data );
	widget->private_data = NULL;
}

/* 初始化部件队列 */
LCUI_API void
WidgetQueue_Init(LCUI_Queue *queue)
{
	Queue_Init(queue, sizeof(LCUI_Widget), Destroy_Widget);
}


LCUI_API LCUI_Queue *Widget_GetMsgFunc( LCUI_Widget *widget )
{
	if( widget != NULL ) {
		return &widget->msg_func;
	}
	return NULL;
}

LCUI_API LCUI_Queue *Widget_GetMsgBuff( LCUI_Widget *widget )
{
	if( widget != NULL ) {
		return &widget->msg_buff;
	}
	return &root_widget.msg_buff;
}

LCUI_API int Widget_Lock( LCUI_Widget *widget )
{
	return LCUIMutex_Lock( &widget->mutex );
}

LCUI_API int Widget_TryLock( LCUI_Widget *widget )
{
	return LCUIMutex_TryLock( &widget->mutex );
}

LCUI_API int Widget_Unlock( LCUI_Widget *widget )
{
	return LCUIMutex_Unlock( &widget->mutex );
}

/* 初始化部件属性 */
static void Widget_AttrInit( LCUI_Widget *widget )
{
	LCUI_App *app;
	app = LCUIApp_GetSelf();
	/*--------------- 初始化部件基本属性及数据 ------------------*/
	widget->auto_size		= FALSE;
	widget->auto_size_mode		= AUTOSIZE_MODE_GROW_AND_SHRINK;
	widget->type_id			= 0;
	widget->modal			= FALSE;
	widget->state			= WIDGET_STATE_NORMAL;
	widget->self_id			= 0;
	widget->app_id			= app?app->id:0;
	widget->parent			= NULL;
	widget->enabled			= TRUE;
	widget->visible			= FALSE;
	widget->focus			= TRUE;
	widget->focus_widget		= NULL;
	widget->pos			= Pos(0, 0);
	widget->size			= Size(0, 0);
	widget->min_size		= Size(0, 0);
	widget->max_size		= Size(INT_MAX, INT_MAX);
	widget->align			= ALIGN_NONE;
	widget->dock			= DOCK_TYPE_NONE;
	widget->offset			= Pos(0, 0);
	widget->pos_type		= POS_TYPE_ABSOLUTE;
	widget->color			= RGB(0,0,0);
	widget->private_data		= NULL;
	widget->valid_state		= 0;
	widget->clickable_mode		= 0;
	widget->clickable_area_alpha	= 0;
	/*------------------------- END --------------------------*/

	/*--------------- 初始化部件的附加属性 ------------------*/
	IntOrFloat_Init( &widget->x );
	IntOrFloat_Init( &widget->y );
	IntOrFloat_Init( &widget->max_x );
	IntOrFloat_Init( &widget->max_y );
	IntOrFloat_Init( &widget->min_x );
	IntOrFloat_Init( &widget->min_y );
	widget->max_x.px = INT_MAX;
	widget->max_y.px = INT_MAX;
	widget->min_x.px = INT_MIN;
	widget->min_y.px = INT_MIN;
	IntOrFloat_Init( &widget->w );
	IntOrFloat_Init( &widget->h );
	IntOrFloat_Init( &widget->max_w );
	IntOrFloat_Init( &widget->max_h );
	IntOrFloat_Init( &widget->min_w );
	IntOrFloat_Init( &widget->min_h );
	/*---------------------- END -----------------------*/

	/*------------- 函数指针初始化 ------------------*/
	widget->set_align = Widget_SetAlign;
	widget->set_alpha = Widget_SetAlpha;
	widget->set_border = Widget_SetBorder;
	widget->show = Widget_Show;
	widget->hide = Widget_Hide;
	widget->resize = Widget_Resize;
	widget->move = Widget_Move;
	widget->enable = Widget_Enable;
	widget->disable = Widget_Disable;
	/*----------------- END -----------------------*/

	/* 初始化边框数据 */
	Border_Init( &widget->border );
	Widget_BackgroundInit( widget );
	/* 为部件创建一个图层 */
	widget->glayer = GraphLayer_New();
	/* 继承主图层的透明度 */
	GraphLayer_InerntAlpha( widget->glayer, TRUE );
	/* 设定图层属性 */
	widget->glayer->graph.color_type = COLOR_TYPE_RGBA;
	//widget->glayer->graph.is_opaque = FALSE;
	//widget->glayer->graph.not_visible = TRUE;
	RectQueue_Init( &widget->invalid_area ); /* 初始化无效区域记录 */
	EventSlots_Init( &widget->event );	/* 初始化部件的事件数据队列 */
	WidgetQueue_Init( &widget->child );	/* 初始化子部件集 */
	WidgetMsgBuff_Init( widget );
	WidgetMsgFunc_Init( widget );
	LCUIString_Init( &widget->type_name );
	LCUIString_Init( &widget->style_name );
	LCUIMutex_Init( &widget->mutex );	/* 初始化互斥锁 */
}

/* 销毁部件 */
static void Widget_ExecDestroy( LCUI_Widget *widget )
{
	int i, total;
	LCUI_Queue *child_list;
	LCUI_Widget *tmp;

	if( !widget ) {
		return;
	}
	child_list = Widget_GetChildList( widget->parent );
	Queue_Lock( child_list );
	total = Queue_GetTotal(child_list);
	for(i=0; i<total; ++i) {
		tmp = Queue_Get(child_list, i);
		if(tmp == widget) {
			Queue_Delete(child_list, i);
			break;
		}
	}
	Queue_Unlock( child_list );
}

/* 初始化根部件 */
LCUI_API void RootWidget_Init(void)
{
	Widget_AttrInit( &root_widget );
	Widget_SetAlign( &root_widget, ALIGN_MIDDLE_CENTER, Pos(0,0) );
}

/* 销毁根部件 */
LCUI_API void RootWidget_Destroy(void)
{
	Widget_ExecDestroy( &root_widget );
}

/* 获取根部件图层指针 */
LCUI_API LCUI_GraphLayer *RootWidget_GetGraphLayer(void)
{
	return root_widget.glayer;
}

/* 获取根部件指针 */
LCUI_API LCUI_Widget *RootWidget_GetSelf(void)
{
	return &root_widget;
}


/*
 * 功能：创建指定类型的部件
 * 返回值：成功则部件的指针，失败则返回NULL
 */
LCUI_API LCUI_Widget*
Widget_New( const char *widget_type )
{
	LCUI_Widget *widget;

	if( !LCUI_Active() ) {
		return NULL;
	}
	widget = (LCUI_Widget *)malloc( sizeof(LCUI_Widget) );
	if( !widget ) {
		char str[256];
		sprintf( str, "%s ()", __FUNCTION__ );
		perror(str);
		return NULL;
	}
	Widget_AttrInit( widget );
	//_DEBUG_MSG()
	/* 作为根部件的子部件 */
	Widget_Container_Add( &root_widget, widget );
	if( !widget_type ) {
		return widget;
	}
	/* 验证部件类型是否有效 */
	if( !WidgetType_Valid(widget_type) ) {
		puts(WIDGET_ERROR_TYPE_NOT_FOUND);
		return NULL;
	}
	/* 保存部件类型 */
	_LCUIString_Copy( &widget->type_name, widget_type );
	widget->type_id = WidgetType_GetID( widget_type );
	/* 调用部件的回调函数，对部件私有数据进行初始化 */
	WidgetFunc_Call( widget, FUNC_TYPE_INIT );
	return widget;
}


/* 累计部件的位置坐标 */
static LCUI_Pos Widget_CountPos( LCUI_Widget *widget )
{
	LCUI_Pos pos;

	if( !widget->parent ) {
		return widget->pos;
	}
	pos = Widget_CountPos(widget->parent);
	pos.x += widget->parent->glayer->padding.left;
	pos.y += widget->parent->glayer->padding.top;
	pos = Pos_Add(pos, widget->pos);
	return pos;
}

/* 获取部件的全局坐标 */
LCUI_API LCUI_Pos
Widget_GetGlobalPos(LCUI_Widget *widget)
{
	return Widget_CountPos(widget);
}

LCUI_API void
Widget_SetClickableAlpha( LCUI_Widget *widget, uchar_t alpha, int mode )
/* 设定部件可被点击的区域的透明度 */
{
	if( mode == 0 ) {
		widget->clickable_mode = 0;
	} else {
		widget->clickable_mode = 1;
	}
	//printf("%p, set, mode: %d, alpha: %d\n", widget, widget->clickable_mode, alpha);
	widget->clickable_area_alpha = alpha;
}

LCUI_API void
Widget_SetAlign(LCUI_Widget *widget, ALIGN_TYPE align, LCUI_Pos offset)
/* 功能：设定部件的对齐方式以及偏移距离 */
{
	if( !widget ) {
		return;
	}
	if( widget->align == align
	 && offset.x == widget->offset.x
	 && offset.y == widget->offset.y ) {
		return;
	}
	widget->align = align;
	widget->offset = offset;
	Widget_UpdatePos(widget);/* 更新位置 */
}

LCUI_API int
Widget_SetMaxSize( LCUI_Widget *widget, char *width, char *height )
/*
 * 功能：设定部件的最大尺寸
 * 说明：当值为0时，部件的尺寸不受限制，用法示例可参考Set_Widget_Size()函数
 * */
{
	int n;
	n = GetIntOrFloat( width, &widget->max_w );
	n += GetIntOrFloat( width, &widget->max_h );
	return n;
}

LCUI_API int
Widget_SetMinSize( LCUI_Widget *widget, char *width, char *height )
/*
 * 功能：设定部件的最小尺寸
 * 说明：用法示例可参考Set_Widget_Size()函数
 * */
{
	int n;
	n = GetIntOrFloat( width, &widget->min_w );
	n += GetIntOrFloat( width, &widget->min_h );
	return n;
}

LCUI_API void
Widget_LimitSize(LCUI_Widget *widget, LCUI_Size min_size, LCUI_Size max_size)
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
	widget->min_size = min_size;
	widget->max_size = max_size;
}

LCUI_API void
Widget_LimitPos(LCUI_Widget *widget, LCUI_Pos min_pos, LCUI_Pos max_pos)
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
	Widget_UpdatePos( widget );
}

LCUI_API void
_Limit_Widget_Pos( LCUI_Widget *widget, char *x_str, char*y_str )
{

}

LCUI_API void
_Limit_Widget_Size( LCUI_Widget *widget, char *w_str, char*h_str )
{

}

/* 设定部件的边框 */
LCUI_API void
Widget_SetBorder( LCUI_Widget *widget, LCUI_Border border )
{
	widget->border = border;
	Widget_Draw( widget );
}

/* 设定部件边框的四个角的圆角半径 */
LCUI_API void
Widget_SetBorderRadius( LCUI_Widget *widget, unsigned int radius )
{
	Border_Radius( &widget->border, radius );
	Widget_Draw( widget );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* 设定部件的背景图像 */
LCUI_API void
Widget_SetBackgroundImage( LCUI_Widget *widget, LCUI_Graph *img )
{
	if(!widget) {
		return;
	}
	Widget_Draw( widget );
	if( !Graph_IsValid(img) ) {
		Graph_Init( &widget->background.image );
		return;
	}
	widget->background.image = *img;
}

/* 设定背景图的布局 */
LCUI_API void
Widget_SetBackgroundLayout( LCUI_Widget *widget, LAYOUT_TYPE layout )
{
	if(!widget) {
		return;
	}
	widget->background.layout = layout;
}

/* 设定部件的背景颜色 */
LCUI_API void
Widget_SetBackgroundColor( LCUI_Widget *widget, LCUI_RGB color )
{
	if(!widget) {
		return;
	}
	widget->background.color = color;
}

/* 设定部件背景是否透明 */
LCUI_API void
Widget_SetBackgroundTransparent( LCUI_Widget *widget, LCUI_BOOL flag )
{
	if(!widget) {
		return;
	}
	widget->background.transparent = flag;
}

LCUI_API void
Widget_Enable(LCUI_Widget *widget)
/* 功能：启用部件 */
{
	widget->enabled = TRUE;
	Widget_SetState(widget, WIDGET_STATE_NORMAL);
}

LCUI_API void
Widget_Disable(LCUI_Widget *widget)
/* 功能：禁用部件 */
{
	widget->enabled = FALSE;
	Widget_SetState( widget, WIDGET_STATE_DISABLE );
}

/* 指定部件是否可见 */
LCUI_API void
Widget_Visible( LCUI_Widget *widget, LCUI_BOOL flag )
{
	widget->visible = flag;
	if( flag ) {
		GraphLayer_Show( widget->glayer );
	} else {
		GraphLayer_Hide( widget->glayer );
	}
}

LCUI_API void
Widget_SetPos(LCUI_Widget *widget, LCUI_Pos pos)
/*
 * 功能：设定部件的位置
 * 说明：只修改坐标，不进行局部刷新
 * */
{
	widget->pos = pos;
}

/* 设置部件的内边距 */
LCUI_API void
Widget_SetPadding( LCUI_Widget *widget, LCUI_Padding padding )
{
	GraphLayer_SetPadding( widget->glayer, padding );
	/* 更新子部件的位置 */
	Widget_UpdateChildPos( widget );
}

LCUI_API void
Widget_SetPosType( LCUI_Widget *widget, POS_TYPE pos_type )
/* 设定部件的定位类型 */
{
	widget->pos_type = pos_type;
	Widget_UpdatePos( widget );
}

/* 设置部件的堆叠顺序 */
LCUI_API int
Widget_SetZIndex( LCUI_Widget *widget, int z_index )
{
	LCUI_GraphLayer *glayer;
	glayer = Widget_GetGraphLayer(widget);
	if( glayer == NULL ) {
		return -1;
	}
	/* 限制模态部件与非模态部件的z_index值 */
	if( !widget->modal ) {
		if( z_index > 9999 ) {
			z_index = 9999;
		}
	} else {
		if( z_index < 10000 ) {
			z_index = 10000;
		}
	}
	GraphLayer_SetZIndex( glayer, z_index );
	/* 向父部件添加消息，要求对子部件进行排序 */
	WidgetMsg_Post( widget->parent, WIDGET_SORT, NULL, TRUE, FALSE );
	return 0;
}

LCUI_API void
Widget_SetAlpha(LCUI_Widget *widget, unsigned char alpha)
/* 功能：设定部件的透明度 */
{
	if( GraphLayer_GetAlpha( widget->glayer ) != alpha) {
		GraphLayer_SetAlpha( widget->glayer, alpha );
		Widget_Refresh( widget );
	}
}

LCUI_API void
Widget_ExecMove( LCUI_Widget *widget, LCUI_Pos pos )
/*
 * 功能：执行移动部件位置的操作
 * 说明：更改部件位置，并添加局部刷新区域
 **/
{
	LCUI_Rect old_rect, new_rect;
	LCUI_Pos max_pos, min_pos;

	if( !widget ) {
		return;
	}
	max_pos = Widget_GetMaxPos( widget );
	min_pos = Widget_GetMinPos( widget );

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
	if( pos.x == widget->pos.x
	 && pos.y == widget->pos.y ) {
		return;
	}
	/* 如果图层是显示的，并且位置变动，那就需要添加无效区域 */
	if( widget->visible ) {
		old_rect = Widget_GetRect( widget );
		//_DEBUG_MSG("old:%d,%d,%d,%d\n",
		// rect.x, rect.y, rect.width, rect.height);
		widget->pos = pos;
		new_rect.x = pos.x;
		new_rect.y = pos.y;
		new_rect.width = old_rect.width;
		new_rect.height = old_rect.height;
		//_DEBUG_MSG("new:%d,%d,%d,%d\n",
		// rect.x, rect.y, rect.width, rect.height);
		Widget_InvalidArea( widget->parent, new_rect );
		Widget_InvalidArea( widget->parent, old_rect );
	} else {
		/* 否则，直接改坐标 */
		widget->pos = pos;
	}
	GraphLayer_SetPos( widget->glayer, pos.x, pos.y );
}

LCUI_API void
Widget_ExecHide(LCUI_Widget *widget)
/* 功能：执行隐藏部件的操作 */
{
	if( !widget || !widget->visible ) {
		return;
	}

	/* 获取隐藏部件需要调用的函数指针，并调用之 */
	WidgetFunc_Call( widget, FUNC_TYPE_HIDE );
	Widget_Visible( widget, FALSE );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* 为部件的子部件进行排序 */
static void
Widget_ExecSortChild( LCUI_Widget *widget )
{
	int i, j, total;
	LCUI_Widget *child_a, *child_b;
	LCUI_Queue *child_list;

	child_list = Widget_GetChildList( widget );
	Queue_Lock( child_list );
	total = Queue_GetTotal( child_list );
	/* 先将模态部件移动到队列前端 */
	for(j=i=0; i<total; ++i) {
		child_a = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child_a || !child_a->modal ) {
			continue;
		}
		/* 更新模态部件的z-index值 */
		Widget_SetZIndex( child_a, 0 );
		Queue_Move( child_list, 0, i );
		++j; /* j用于统计模态部件的数量 */
	}
	/* 忽略前面j个模态部件，从第j个部件开始 */
	for(i=j; i<total; ++i) {
		child_a = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child_a ) {
			continue;
		}
		for(j=i+1; j<total; ++j) {
			child_b = (LCUI_Widget*)Queue_Get( child_list, j );
			if( !child_b ) {
				continue;
			}
			if( child_b->glayer->z_index
			 > child_a->glayer->z_index ) {
				Queue_Swap( child_list, j, i );
			}
		}
	}
	Queue_Unlock( child_list );
	GraphLayer_Sort( Widget_GetGraphLayer(widget) );
}

/* 将部件显示在同等z-index值的部件的前端 */
LCUI_API int
Widget_Front( LCUI_Widget *widget )
{
	int i, src_pos, des_pos, total;
	LCUI_Widget *child;
	LCUI_Queue *child_list;

	if( widget == NULL || widget == &root_widget ) {
		return -1;
	}
	child_list = Widget_GetChildList( widget->parent );
	Queue_Lock( child_list );
	total = Queue_GetTotal( child_list );
	/* 先在队列中找到自己，以及z-index值小于或等于它的第一个部件 */
	for(i=0,src_pos=des_pos=-1; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get( child_list, i );
		if( !child ) {
			continue;
		}
		if( child == widget ) {
			src_pos = i;
			continue;
		}
		if( des_pos == -1 ) {
			if( child->glayer->z_index
			  <= widget->glayer->z_index ) {
				des_pos = i;
			}
		} else {
			if( src_pos != -1 ) {
				break;
			}
		}
	}
	/* 没有找到就退出 */
	if( des_pos == -1 || src_pos == -1 ) {
		Queue_Unlock( child_list );
		return -1;
	}
	if( src_pos+1 == des_pos ) {
		Queue_Unlock( child_list );
		return 1;
	}
	/* 找到的话就移动位置 */
	Queue_Move( child_list, des_pos, src_pos );
	Queue_Unlock( child_list );
	return GraphLayer_Front( Widget_GetGraphLayer(widget) );
}

LCUI_API void
Widget_ExecShow(LCUI_Widget *widget)
/* 功能：执行显示部件的任务 */
{
	if( !widget || widget->visible ) {
		return;
	}
	/* 调用该部件在显示时需要用到的函数 */
	WidgetFunc_Call(widget, FUNC_TYPE_SHOW);
	Widget_Visible( widget, TRUE ); /* 部件可见 */
	if( widget->focus ) {
		//Set_Focus( widget );	/* 将焦点给该部件 */
	}
	Widget_Front(widget); /* 改变部件的排列位置 */
	Widget_Refresh( widget ); /* 刷新部件所在区域的图形显示 */
}

/* 自动调整部件大小，以适应其内容大小 */
LCUI_API void
Widget_AutoResize(LCUI_Widget *widget)
{
	int i, total, temp;
	LCUI_Widget *child;
	LCUI_Queue point;
	LCUI_Size size;

	size.w = 0;
	size.h = 0;
	Queue_Init(&point, sizeof (LCUI_Pos), NULL);
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {/* 遍历每个子部件 */
		child = Queue_Get(&widget->child, i);
		if( !child ) {
			continue;
		}
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
	//size.w += 6;
	//size.h += 6;
	//printf("Widget_AutoResize(): %p, autosize: %d, new size: %d,%d\n",
	//widget, widget->auto_size, size.w, size.h);
	//print_widget_info(widget);
	/* 得出适合的尺寸，调整之 */
	Widget_Resize(widget, size);
}

/* 执行改变部件尺寸的操作 */
LCUI_API int
Widget_ExecResize(LCUI_Widget *widget, LCUI_Size size)
{
	LCUI_WidgetEvent event;

	if( !widget ) {
		return -1;
	}

	if( widget->size.w == size.w
	 && widget->size.h == size.h ) {
		return 1;
	}

	/* 记录事件数据 */
	event.type = EVENT_RESIZE;
	event.resize.new_size = size;
	event.resize.old_size = widget->size;

	if( widget->visible ) {
		LCUI_Rect rect;
		rect = Widget_GetRect( widget );
		Widget_InvalidArea( widget->parent, rect );
		widget->size = size;
		rect.width = size.w;
		rect.height = size.h;
		Widget_InvalidArea( widget->parent, rect );
	} else {
		widget->size = size;
	}
	//_DEBUG_MSG("size: %d, %d\n", size.w, size.h);
	GraphLayer_Resize( widget->glayer, size.w, size.h );
	WidgetFunc_Call( widget, FUNC_TYPE_RESIZE );
	//Widget_Refresh( widget );

	/* 更新子部件的位置及尺寸 */
	Widget_UpdateChildSize( widget );
	Widget_UpdateChildPos( widget );

	if( widget->parent && widget->parent->auto_size ) {
		/* 如果需要让它的容器能够自动调整大小 */
		Widget_AutoResize( widget->parent );
	}
	Widget_DispatchEvent( widget, &event ); /* 处理部件的RESIZE事件 */
	return 0;
}

/* 启用或禁用部件的自动尺寸调整功能 */
LCUI_API void
Widget_SetAutoSize(	LCUI_Widget *widget,
			LCUI_BOOL flag, AUTOSIZE_MODE mode )
{
	widget->auto_size = flag;
	widget->auto_size_mode = mode;
}

LCUI_API void
Widget_ExecRefresh(LCUI_Widget *widget)
/* 功能：执行刷新显示指定部件的整个区域图形的操作 */
{
	//_DEBUG_MSG("refresh widget: %d,%d,%d,%d\n",
	//	Widget_GetRect(widget).x, Widget_GetRect(widget).y,
	//	Widget_GetRect(widget).width, Widget_GetRect(widget).height );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* 执行部件的更新操作 */
LCUI_API void
Widget_ExecUpdate(LCUI_Widget *widget)
{
	/* 将样式库中的属性同步至该部件 */
	WidgetStyle_Sync( widget );
	/* 调用回调函数进更新 */
	WidgetFunc_Call( widget, FUNC_TYPE_UPDATE );
}

LCUI_API void
Widget_ExecDrawBackground( LCUI_Widget *widget )
{
	int fill_mode;
	LCUI_Graph *graph;
	LCUI_Background *bg;

	graph = Widget_GetSelfGraph( widget );
	bg = &widget->background;
	/* 如果背景透明，则使用覆盖模式将背景图绘制到部件上 */
	if( bg->transparent ) {
		fill_mode = GRAPH_MIX_FLAG_REPLACE;
	} else { /* 否则，使用叠加模式 */
		fill_mode = GRAPH_MIX_FLAG_OVERLAY;
	}
	fill_mode |= bg->layout;
	Graph_FillImage( graph, &bg->image, fill_mode, bg->color );
}

/* 执行部件图形更新操作 */
LCUI_API void
Widget_ExecDraw(LCUI_Widget *widget)
{
	LCUI_Graph *graph;
	LCUI_WidgetEvent event;

	if( !widget ) {
		return;
	}
	/* 先更新一次部件 */
	//Widget_ExecUpdate( widget );
	/* 然后根据部件样式，绘制背景图形 */
	Widget_ExecDrawBackground( widget );
	/* 调用该类型部件默认的函数进行处理 */
	WidgetFunc_Call( widget, FUNC_TYPE_DRAW );
	graph = Widget_GetSelfGraph( widget );
	/* 绘制边框线 */
	Graph_DrawBorder( graph, widget->border );

	event.type = EVENT_REDRAW;
	/* 处理部件的RESIZE事件 */
	Widget_DispatchEvent( widget, &event );
	Widget_InvalidArea( widget->parent, Widget_GetRect(widget) );
}

/* 获取指向部件自身图形数据的指针 */
LCUI_API LCUI_Graph*
Widget_GetSelfGraph( LCUI_Widget *widget )
{
	return GraphLayer_GetSelfGraph( widget->glayer );
}

/* 获取部件实际显示的图形 */
LCUI_API int
Widget_GetGraph(
	LCUI_Widget *widget,
	LCUI_Graph *graph_buff,
	LCUI_Rect rect )
{
	return GraphLayer_GetGraph( widget->glayer, graph_buff, rect );
}

LCUI_API LCUI_Pos
Widget_GetValidPos( LCUI_Widget *widget, LCUI_Pos pos )
/* 获取有效化后的坐标数据，其实就是将在限制范围外的坐标处理成在限制范围内的 */
{
	if( pos.x > widget->max_x.px ) {
		pos.x = widget->max_x.px;
	}
	if( pos.x < widget->min_x.px ) {
		pos.x = widget->min_x.px;
	}
	if( pos.y > widget->max_y.px ) {
		pos.y = widget->max_y.px;
	}
	if( pos.y < widget->min_y.px ) {
		pos.y = widget->min_y.px;
	}
	return pos;
}

LCUI_API void
Widget_Move(LCUI_Widget *widget, LCUI_Pos new_pos)
/*
 * 功能：移动部件位置
 * 说明：如果部件的布局为ALIGN_NONE，那么，就可以移动它的位置，否则，无法移动位置
 * */
{
	if( !widget ) {
		return;
	}
	widget->x.px = new_pos.x;
	widget->y.px = new_pos.y;
	WidgetMsg_Post( widget, WIDGET_MOVE, &new_pos, TRUE, FALSE );
}

LCUI_API void
Widget_UpdatePos(LCUI_Widget *widget)
/* 功能：更新部件的位置 */
{
	WidgetMsg_Post( widget, WIDGET_MOVE, NULL, TRUE, FALSE );
}

LCUI_API void
Widget_ExecUpdatePos( LCUI_Widget *widget )
/* 更新部件的位置，以及位置变动范围 */
{
	LCUI_Pos pos;
	pos = _Widget_GetPos( widget );
	widget->max_pos = _Widget_GetMaxPos( widget );
	widget->min_pos = _Widget_GetMinPos( widget );
	/* 只有在部件对齐方式不为ALIGN_NONE，或者计算的坐标不为(0,0)时改变位置 */
	if( widget->align != ALIGN_NONE || (pos.x != 0 && pos.y != 0)) {
		Widget_ExecMove( widget, pos );
	}
}

LCUI_API void
Widget_UpdateSize( LCUI_Widget *widget )
/* 部件尺寸更新 */
{
	LCUI_Size size;

	size = _Widget_GetSize(widget);
	Widget_Resize( widget, size );
}

LCUI_API void
Widget_UpdateChildSize(LCUI_Widget *widget)
/*
 * 功能：更新指定部件的子部件的尺寸
 * 说明：当部件尺寸改变后，有的部件的尺寸以及位置是按百分比算的，需要重新计算。
 * */
{
	int i, total;
	LCUI_Widget *child;

	if( !widget ) {
		return;
	}
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if( !child ) {
			continue;
		}
		/* 如果该子部件的尺寸是使用百分比的 */
		if( child->w.which_one == 1
		 || child->h.which_one == 1 ) {
			Widget_UpdatePos( child );
			Widget_UpdateSize( child );
		}
	}
}

LCUI_API int
Widget_ExecUpdateSize( LCUI_Widget *widget )
{
	LCUI_Size size;
	size = _Widget_GetSize( widget );
	if( size.w == widget->size.w
	 && size.h == widget->size.h ) {
		return 1;
	}
	widget->size = size;
	widget->w.px = size.w;
	widget->h.px = size.h;
	return 0;
}

LCUI_API void
Widget_UpdateChildPos(LCUI_Widget *widget)
/*
 * 功能：更新指定部件的子部件的位置
 * 说明：当作为子部件的容器部件的尺寸改变后，有的部件的布局不为ALIGN_NONE，就需要重新
 * 调整位置。
 * */
{
	LCUI_Widget *child;
	int i, total;
	total = Queue_GetTotal(&widget->child);
	for(i=0; i<total; ++i) {
		child = (LCUI_Widget*)Queue_Get(&widget->child, i);
		if( child == NULL ) {
			continue;
		}
		Widget_UpdatePos( child );
	}
	/* 更新该部件内定位类型为static的子部件的位置 */
	Widget_UpdateChildStaticPos( widget );
}

/* 设定部件的高度，单位为像素 */
LCUI_API void
Widget_SetHeight( LCUI_Widget *widget, int height )
{
	switch( widget->dock ) {
	    case DOCK_TYPE_NONE:
	    case DOCK_TYPE_TOP:
	    case DOCK_TYPE_BOTTOM:
		widget->h.which_one = 0;
		widget->h.px = height;
		break;
	    default: return;
	}
}

LCUI_API void
Widget_SetSize( LCUI_Widget *widget, char *width, char *height )
/*
 * 功能：设定部件的尺寸大小
 * 说明：如果设定了部件的停靠位置，并且该停靠类型默认限制了宽/高，那么部件的宽/高就不能被改变。
 * 用法示例：
 * Widget_SetSize( widget, "100px", "50px" ); 部件尺寸最大为100x50像素
 * Widget_SetSize( widget, "100%", "50px" ); 部件宽度等于容器宽度，高度为50像素
 * Widget_SetSize( widget, "50", "50" ); 部件尺寸最大为50x50像素，px可以省略
 * Widget_SetSize( widget, NULL, "50%" ); 部件宽度保持原样，高度为容器高度的一半
 * */
{
	switch( widget->dock ) {
	    case DOCK_TYPE_TOP:
	    case DOCK_TYPE_BOTTOM: /* 只能改变高 */
		GetIntOrFloat( height, &widget->h );
		break;
	    case DOCK_TYPE_LEFT:
	    case DOCK_TYPE_RIGHT:/* 只能改变宽 */
		GetIntOrFloat( width, &widget->w );
		break;
	    case DOCK_TYPE_FILL:break;
	    case DOCK_TYPE_NONE: /* 可改变宽和高 */
		GetIntOrFloat( width, &widget->w );
		GetIntOrFloat( height, &widget->h );
		break;
	}
	Widget_UpdateSize( widget );
}

/* 指定部件是否为模态部件 */
LCUI_API void
Widget_SetModal( LCUI_Widget *widget, LCUI_BOOL is_modal )
{
	if( !widget ) {
		return;
	}
	widget->modal = is_modal;
	Widget_SetZIndex( widget, 0 );
}

LCUI_API void
Widget_SetDock( LCUI_Widget *widget, DOCK_TYPE dock )
/* 设定部件的停靠类型 */
{
	switch( dock ) {
	    case DOCK_TYPE_TOP:
		Widget_SetAlign( widget, ALIGN_TOP_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_BOTTOM:
		Widget_SetAlign( widget, ALIGN_BOTTOM_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", NULL );
		break;
	    case DOCK_TYPE_LEFT:
		Widget_SetAlign( widget, ALIGN_MIDDLE_LEFT, Pos(0,0) );
		Widget_SetSize( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_RIGHT:
		Widget_SetAlign( widget, ALIGN_MIDDLE_RIGHT, Pos(0,0) );
		Widget_SetSize( widget, NULL, "100%" );
		break;
	    case DOCK_TYPE_FILL:
		Widget_SetAlign( widget, ALIGN_MIDDLE_CENTER, Pos(0,0) );
		Widget_SetSize( widget, "100%", "100%" );
		break;
	    case DOCK_TYPE_NONE:
		Widget_SetAlign( widget, ALIGN_NONE, Pos(0,0) );
		break;
	}
	widget->dock = dock;
}

LCUI_API void
Widget_OffsetPos(LCUI_Widget *widget, LCUI_Pos offset)
/* 功能：以部件原有的位置为基础，根据指定的偏移坐标偏移位置 */
{
	Widget_Move( widget, Pos_Add(widget->pos, offset) );
}

LCUI_API void
Widget_MoveToPos(LCUI_Widget *widget, LCUI_Pos des_pos, int speed)
/*
 * 功能：将部件以指定的速度向指定位置移动
 * 说明：des_pos是目标位置，speed是该部件的移动速度，单位为：像素/秒
 * */
{
	int i, j;
	double w, h, l, n, x, y;

	if(speed <= 0) {
		return;
	}
	x = Widget_GetPos(widget).x;
	y = Widget_GetPos(widget).y;
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
		Widget_Move(widget, Pos(x, y));
		LCUI_MSleep(10);/* 停顿0.01秒 */
	}
}

LCUI_API void
Widget_Refresh(LCUI_Widget *widget)
/* 功能：刷新显示指定部件的整个区域图形 */
{
	if( !widget ) {
		return;
	}
	WidgetMsg_Post( widget, WIDGET_REFRESH, NULL, TRUE, FALSE );
}

/* 调整部件的尺寸 */
LCUI_API void
Widget_Resize( LCUI_Widget *widget, LCUI_Size new_size )
{
	LCUI_Size max_size, min_size;
	
	if( widget == NULL || new_size.w < 0 || new_size.h < 0) {
		return;
	}
	max_size = Widget_GetMaxSize( widget );
	min_size = Widget_GetMinSize( widget );
	/* 根据指定的部件的尺寸范围，调整尺寸 */
	if(new_size.w > max_size.w) {
		new_size.w = max_size.w;
	}
	if(new_size.h > max_size.h) {
		new_size.h = max_size.h;
	}
	if(new_size.w < min_size.w) {
		new_size.w = min_size.w;
	}
	if(new_size.h < min_size.h) {
		new_size.h = min_size.h;
	}
	widget->w.px = new_size.w;
	widget->h.px = new_size.h;
	
	WidgetMsg_Post( widget, WIDGET_RESIZE, &new_size, TRUE, FALSE );
	if( widget->pos_type == POS_TYPE_STATIC
	 || widget->pos_type == POS_TYPE_RELATIVE ) {
		//WidgetMsg_Post( widget, NULL, DATATYPE_POS_TYPE, 0 );
	}
}

/* 重新绘制部件 */
LCUI_API void
Widget_Draw(LCUI_Widget *widget)
{
	if( !widget ) {
		return;
	}
	Widget_Update( widget );
	WidgetMsg_Post( widget, WIDGET_PAINT, NULL, TRUE, FALSE );
}

/* 销毁部件 */
LCUI_API void
Widget_Destroy( LCUI_Widget *widget )
{
	if( !widget ) {
		return;
	}
	Cancel_Focus( widget ); /* 取消焦点 */
	WidgetMsg_Post( widget, WIDGET_DESTROY, NULL, TRUE, FALSE );
}

LCUI_API void
Widget_Update(LCUI_Widget *widget)
/*
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：此记录会添加至队列，如果队列中有一条相同记录，则覆盖上条记录。
 * */
{
	if( !widget ) {
		return;
	}
	WidgetMsg_Post( widget, WIDGET_UPDATE, NULL, TRUE, FALSE );
}

LCUI_API void
__Widget_Update(LCUI_Widget *widget)
/*
 * 功能：让部件根据已设定的属性，进行相应数据的更新
 * 说明：与上个函数功能一样，但是，可以允许队列中有两条相同记录。
 * */
{
	if( !widget ) {
		return;
	}
	WidgetMsg_Post( widget, WIDGET_UPDATE, NULL, FALSE, FALSE );
}

/* 显示部件 */
LCUI_API void
Widget_Show(LCUI_Widget *widget)
{
	if( !widget ) {
		return;
	}
	WidgetMsg_Post( widget->parent, WIDGET_SORT, NULL, TRUE, FALSE );
	WidgetMsg_Post( widget, WIDGET_SHOW, NULL, TRUE, FALSE );
}

LCUI_API void
Widget_Hide(LCUI_Widget *widget)
/* 功能：隐藏部件 */
{
	if( !widget ) {
		return;
	}
	Cancel_Focus( widget ); /* 取消焦点 */
	WidgetMsg_Post( widget, WIDGET_HIDE, NULL, TRUE, FALSE );
}

/* 改变部件的状态 */
LCUI_API int
Widget_SetState( LCUI_Widget *widget, int state )
{
	if( !widget ) {
		return -1;
	}
	/* 如果该状态不被该部件支持 */
	if( (widget->valid_state & state) != state ) {
		return 1;
	}
	if( !widget->enabled ) {
		state = WIDGET_STATE_DISABLE;
	}
	WidgetMsg_Post( widget, WIDGET_CHGSTATE, &state, TRUE, FALSE );
	return 0;
}
/************************* Widget End *********************************/


LCUI_API LCUI_BOOL WidgetMsg_Dispatch( LCUI_Widget *widget, WidgetMsgData *data_ptr )
{
	int ret;

	if( data_ptr == NULL ) {
		return TRUE;
	}
	if( widget == NULL ) {
		widget = &root_widget;
	}
	/* 根据不同的类型来进行处理 */
	switch(data_ptr->msg_id) {
	case WIDGET_RESIZE:
		/* 部件尺寸更新，将更新部件的位置 */
		if( !data_ptr->valid ) {
			ret = Widget_ExecUpdateSize( widget );
		} else {
			ret = Widget_ExecResize( widget, data_ptr->data.size );
		}
		if( ret == 0 ) {
			Widget_Draw( widget );
			Widget_UpdatePos( widget );
		}
		break;
	case WIDGET_MOVE:
		if( widget->align == ALIGN_NONE
		 && data_ptr->valid ) {
			Widget_ExecMove( widget, data_ptr->data.pos );
		} else {
			Widget_ExecUpdatePos( widget );
		}
		break;
	case WIDGET_UPDATE:
		if( Widget_TryLock( widget ) != 0 ) {
			return FALSE;
		}
		Widget_ExecUpdate( widget );
		Widget_Unlock( widget );
		break;
	case WIDGET_CHGSTATE:
		/* 只有状态不一样才重绘部件 */
		if( widget->state == data_ptr->data.state ) {
			break;
		}
		widget->state = (WIDGET_STATE)data_ptr->data.state;
		Widget_Draw( widget );
		break;
	case WIDGET_PAINT:
		if( Widget_TryLock( widget ) != 0 ) {
			return FALSE;
		}
		Widget_ExecDraw( widget );
		Widget_Unlock( widget );
		break;
	case WIDGET_HIDE:
		Widget_ExecHide( data_ptr->target );
		break;
	case WIDGET_SORT:
		Widget_ExecSortChild( widget );
		break;
	case WIDGET_SHOW:
		Widget_ExecShow( data_ptr->target );
		/* 更新父部件中的STATIC定位类型的子部件的位置 */
		//Widget_UpdateChildStaticPos( widget->parent );
		break;
	case WIDGET_REFRESH:
		Widget_ExecRefresh( widget );
		break;
	case WIDGET_DESTROY:
		if( Widget_TryLock( data_ptr->target ) != 0 ) {
			return FALSE;
		}
		/* 添加刷新区域 */
		Widget_ExecRefresh( data_ptr->target );
		/* 开始销毁部件数据 */
		Widget_ExecDestroy( data_ptr->target );
		Widget_Unlock( data_ptr->target );
		break;
	default:
		WidgetMsg_AddToTask( widget, data_ptr );
		break;
	}
	return TRUE;
}

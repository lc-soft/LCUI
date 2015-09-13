/* ***************************************************************************
 * widget_paint.c -- LCUI widget paint module.
 *
 * Copyright (C) 2013-2015 by Liu Chao <lc-soft@live.cn>
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
 * ***************************************************************************/

/* ****************************************************************************
 * widget_paint.c -- LCUI部件绘制模块
 *
 * 版权所有 (C) 2013-2015 归属于 刘超 <lc-soft@live.cn>
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
 * ***************************************************************************/

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

/**
 * 根据所处框区域，调整矩形
 * @param[in] w		目标部件
 * @param[in,out] r	矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
static void Widget_AdjustArea(	LCUI_Widget w, LCUI_Rect *in_rect,
				LCUI_Rect *out_rect, int box_type )
{
	LCUI_Rect *box;
	switch( box_type ) {
	case SV_BORDER_BOX: box = &w->box.border; break;
	case SV_GRAPH_BOX: box = &w->box.graph; break;
	case SV_CONTENT_BOX:
	default: box = &w->box.content; break;
	}
	/* 如果为NULL，则视为使用整个部件区域 */
	if( !in_rect ) {
		out_rect->x = out_rect->y = 0;
		out_rect->w = box->w;
		out_rect->h = box->h;
	} else {
		*out_rect = *in_rect;
	}
	LCUIRect_ValidateArea( out_rect, Size(box->w, box->h) );
	/* 将坐标转换成相对于图像呈现区的坐标 */
	out_rect->x += (box->x - w->box.graph.x);
	out_rect->y += (box->y - w->box.graph.y);
}

/**
 * 标记部件内的一个区域为无效的，以使其重绘
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
void Widget_InvalidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type )
{
	LCUI_Rect rect;
	Widget_AdjustArea( w, r, &rect, box_type );
	DEBUG_MSG("[%s]: invalidRect:(%d,%d,%d,%d)\n", w->type, rect.x, rect.y, rect.width, rect.height);
	DirtyRectList_Add( &w->dirty_rects, &rect );
	while( w = w->parent, w && !w->has_dirty_child ) {
		w->has_dirty_child = TRUE;
	}
}

/**
 * 获取部件中的无效区域
 * @param[in] widget	目标部件
 * @param[out] area	无效区域
 */
int Widget_GetInvalidArea( LCUI_Widget widget, LCUI_Rect *area )
{
	LCUI_Rect *p_rect;
	if( LinkedList_GetTotal(&widget->dirty_rects) <= 0 ) {
		return -1;
	}
	LinkedList_Goto( &widget->dirty_rects, 0 );
	DEBUG_MSG("list: %p, used node num: %d, current: %p, pos: %d\n",
		&widget->dirty_rects, widget->dirty_rects.used_node_num,
		widget->dirty_rects.current_node, widget->dirty_rects.current_node_pos);
	p_rect = (LCUI_Rect*)LinkedList_Get( &widget->dirty_rects );
	if( !p_rect ) {
		return -2;
	}
	DEBUG_MSG("p_rect: %d,%d,%d,%d\n", p_rect->x, p_rect->y, p_rect->w, p_rect->h);
	*area = *p_rect;
	return 0;
}

/**
 * 标记部件内的一个区域为有效的
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
void Widget_ValidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type )
{
	LCUI_Rect rect;
	Widget_AdjustArea( w, r, &rect, box_type );
	DirtyRectList_Delete( &w->dirty_rects, &rect );
}

/** 当前部件的绘制函数 */
static void Widget_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	LCUI_WidgetClass *wc;
	LCUI_Rect box;

	box.x = box.y = 0;
	box.width = w->box.graph.width;
	box.height = w->box.graph.height;
	/* 如果是有位图缓存的话，则先清空缓存里的阴影区域 */
	if( Graph_IsValid(&w->graph) ) {
		Graph_ClearShadowArea( paint, &box, &w->computed_style.shadow );
	}
	Graph_DrawBoxShadow( paint, &box, &w->computed_style.shadow );
	box.x = w->box.border.x - w->box.graph.x;
	box.y = w->box.border.y - w->box.graph.y;
	box.width = w->box.border.width;
	box.height = w->box.border.height;
	Graph_DrawBackground( paint, &box, &w->computed_style.background );
	Graph_DrawBorder( paint, &box, &w->computed_style.border );
	wc = LCUIWidget_GetClass( w->type );
	wc && wc->methods.paint ? wc->methods.paint(w, paint):FALSE;
}

static int _Widget_ProcInvalidArea( LCUI_Widget w, int x, int y, 
				    LCUI_Rect *valid_box, 
				    LCUI_DirtyRectList *rlist )
{
	int count;
	LCUI_Widget child;
	LCUI_Rect rect, child_box, *r;

	count = LinkedList_GetTotal( &w->dirty_rects );
	/* 取出当前记录的脏矩形 */
	LinkedList_ForEach( r, 0, &w->dirty_rects ) {
		/* 若有独立位图缓存，则重绘脏矩形区域 */
		if( Graph_IsValid(&w->graph) ) {
			LCUI_PaintContextRec_ paint;
			paint.rect = *r;
			Graph_Quote( &paint.canvas, &w->graph, &paint.rect );
			DEBUG_MSG("[%s]: paint, rect:(%d,%d,%d,%d)\n", w->type, r->x, r->y, r->width, r->height);
			Widget_OnPaint( w, &paint );
		}
		/* 取出与容器内有效区域相交的区域 */
		if( LCUIRect_GetOverlayRect(r, valid_box, &rect) ) {
			/* 转换相对于根级部件的坐标 */
			rect.x += x;
			rect.y += y;
			DEBUG_MSG("[%s]: merge rect:(%d,%d,%d,%d)\n", w->type, rect.x, rect.y, rect.width, rect.height);
			DirtyRectList_Add( rlist, &rect );
		}
	}
	LinkedList_Destroy( &w->dirty_rects );
	/* 若子级部件没有脏矩形记录 */
	if( !w->has_dirty_child ) {
		return count;
	}
	/* 缩小有效区域到当前部件内容框内，若没有重叠区域，则不向子级部件递归 */
	if( !LCUIRect_GetOverlayRect(valid_box, &w->box.content, &child_box) ) {
		return count;
	}
	/* 转换有效区域的坐标，相对于当前部件的内容框 */
	child_box.x -= w->box.content.x;
	child_box.y -= w->box.content.y;
	/* 向子级部件递归 */
	LinkedList_ForEach( child, 0, &w->children ) {
		int child_x, child_y;

		if( !child->computed_style.visible ) {
			continue;
		}
		child_x = child->box.graph.x + x;
		child_x += w->box.content.x - w->box.graph.x;
		child_y = child->box.graph.y + y;
		child_y += w->box.content.y - w->box.graph.y;
		count += _Widget_ProcInvalidArea( child, child_x, child_y, 
						  &child_box, rlist );
	}
	w->has_dirty_child = FALSE;
	return count;
}

/**
 * 处理部件及其子级部件中的脏矩形，并合并至一个记录中
 * @param[in]	w	目标部件
 * @param[out]	rlist	合并后的脏矩形记录
 */
int Widget_ProcInvalidArea( LCUI_Widget w, LCUI_DirtyRectList *rlist )
{
	LCUI_Rect valid_box;
	valid_box.x = 0;
	valid_box.y = 0;
	valid_box.w = w->box.graph.w;
	valid_box.h = w->box.graph.h;
	return _Widget_ProcInvalidArea( w, 0, 0, &valid_box, rlist );
}

/**
 * 将转换部件中的矩形区域转换成指定范围框内有效的矩形区域
 * @param[in]	w		目标部件
 * @param[in]	in_rect		相对于部件呈现框的矩形区域
 * @param[out]	out_rect	转换后的区域
 * @param[in]	box_type	转换后的区域所处的范围框
 */
int Widget_ConvertArea( LCUI_Widget w, LCUI_Rect *in_rect,
			LCUI_Rect *out_rect, int box_type )
{
	LCUI_Rect rect;
	if( !in_rect ) {
		return -1;
	}
	switch( box_type ) {
	case SV_CONTENT_BOX:
		rect = w->box.content;
		break;
	case SV_PADDING_BOX:
		rect = w->box.content;
		rect.x -= w->padding.left;
		rect.y -= w->padding.top;
		rect.w += w->padding.left;
		rect.w += w->padding.right;
		rect.h += w->padding.top;
		rect.h += w->padding.bottom;
		break;
	case SV_BORDER_BOX:
		rect = w->box.border;
		break;
	case SV_GRAPH_BOX:
	default:
		return -2;
	}
	/* 转换成相对坐标 */
	rect.x -= w->box.graph.x;
	rect.y -= w->box.graph.y;
	out_rect->x = in_rect->x - rect.x;
	out_rect->y = in_rect->y - rect.y;
	out_rect->w = in_rect->w;
	out_rect->h = in_rect->h;
	/* 裁剪掉超出范围的区域 */
	if( out_rect->x < 0 ) {
		out_rect->w += out_rect->x;
		out_rect->x = 0;
	}
	if( out_rect->y < 0 ) {
		out_rect->h += out_rect->y;
		out_rect->y = 0;
	}
	if( out_rect->x + out_rect->w > rect.w ) {
		out_rect->w = rect.w - out_rect->x;
	}
	if( out_rect->y + out_rect->h > rect.h ) {
		out_rect->h = rect.h - out_rect->y;
	}
	return 0;
}

/**
 * 渲染指定部件呈现的图形内容
 * @param[in] w		部件
 * @param[in] paint 	进行绘制时所需的上下文
 */
void Widget_Render( LCUI_Widget w, LCUI_PaintContext paint )
{
	int content_left, content_top;
	LCUI_Widget child;
	LCUI_Rect child_rect, canvas_rect, content_rect;
	LCUI_PaintContextRec_ child_paint;
	LCUI_Graph content_graph, self_graph, layer_graph;
	LCUI_BOOL has_overlay, has_content_graph = FALSE,
		  has_self_graph = FALSE,has_layer_graph = FALSE,
		  is_cover_border = FALSE;
	
	Graph_Init( &layer_graph );
	Graph_Init( &self_graph );
	Graph_Init( &content_graph );
	/* 若部件本身是透明的 */
	if( w->computed_style.opacity < 1.0 ) {
		has_self_graph = TRUE;
		has_content_graph = TRUE;
		has_layer_graph = TRUE;
	} else {
		/* 若使用了圆角边框，则判断当前脏矩形区域是否在圆角边框内
		...
		if( ... ) {
			has_content_graph = TRUE;
			is_cover_border = TRUE;
		}
		*/
	}
	/* 如果需要缓存自身的位图 */
	if( has_self_graph ) {
		LCUI_PaintContextRec_ self_paint;
		/* 有位图缓存则直接截取出来，否则绘制一次 */
		if( Graph_IsValid(&w->graph) ) {
			Graph_Quote( &self_graph, &w->graph, &paint->rect );
		} else {
			Graph_Create( &self_graph, paint->rect.width,
				      paint->rect.height );
		}
		self_paint.canvas = self_graph;
		self_paint.rect = paint->rect;
		Widget_OnPaint( w, &self_paint );
	} else {
		/* 直接将部件绘制到目标位图缓存中 */
		if( Graph_IsValid(&w->graph) ) {
			Graph_Quote( &self_graph, &w->graph, &paint->rect );
			Graph_Mix( &paint->canvas, &self_graph, Pos(0,0) );
		} else {
			Widget_OnPaint( w, paint );
		}
	}
	/* 计算内容框相对于图层的坐标 */
	content_left = w->box.content.x - w->box.graph.x;
	content_top = w->box.content.y - w->box.graph.y;
	/* 获取内容框 */
	content_rect.x = content_left;
	content_rect.y = content_top;
	content_rect.width = w->box.content.width;
	content_rect.height = w->box.content.height;
	/* 获取内容框与脏矩形重叠的区域 */
	has_overlay = LCUIRect_GetOverlayRect(
		&content_rect, &paint->rect, &content_rect
	);
	/* 如果没有与内容框重叠，则跳过内容绘制 */
	if( !has_overlay ) {
		goto content_paint_done;
	}
	/* 转换重叠区域的坐标转换为相对于脏矩形的坐标 */
	content_rect.x -= paint->rect.x;
	content_rect.y -= paint->rect.y;
	/* 若需要部件内容区的位图缓存 */
	if( has_content_graph ) {
		content_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Create( &content_graph, content_rect.w, content_rect.h );
	} else {
		/* 引用该区域的位图，作为内容框的位图 */
		Graph_Quote( &content_graph, &paint->canvas, &content_rect );
	}
	/* 按照显示顺序，从底到顶，递归遍历子级部件 */
	LinkedList_ForEachReverse( child, 0, &w->children_show ) {
		if( !child->computed_style.visible ) {
			continue;
		}
		/* 将子部件的区域，由相对于内容框转换为相对于当前脏矩形 */
		child_rect = child->box.graph;
		child_rect.x += (content_left - paint->rect.x);
		child_rect.y += (content_top - paint->rect.y);
		/* 获取于内容框重叠的区域，作为子部件的绘制区域 */
		has_overlay = LCUIRect_GetOverlayRect(
			&content_rect, &child_rect, &child_paint.rect
		);
		/* 区域无效则不绘制 */
		if( !has_overlay ) {
			continue;
		}
		/* 将子部件绘制区域转换相对于当前部件内容框 */
		canvas_rect.x = child_paint.rect.x - content_rect.x;
		canvas_rect.y = child_paint.rect.y - content_rect.y;
		canvas_rect.width = child_paint.rect.width;
		canvas_rect.height = child_paint.rect.height;
		/* 将绘制区域转换为相对于子部件 */
		child_paint.rect.x -= child_rect.x;
		child_paint.rect.y -= child_rect.y;
		DEBUG_MSG("[%s]: canvas_rect:(%d,%d,%d,%d)\n", w->type, canvas_rect.left, canvas_rect.top, canvas_rect.w, canvas_rect.h);
		/* 在内容位图中引用所需的区域，作为子部件的画布 */
		Graph_Quote( &child_paint.canvas, &content_graph, &canvas_rect );
		Widget_Render( child, &child_paint );
	}
	/* 如果与圆角边框重叠，则裁剪掉边框外的内容 */
	if( is_cover_border ) {
		/* content_graph ... */
	}

content_paint_done:

	/* 若需要绘制的是当前部件图层，则先混合部件自身位图和内容位图，得出当
	 * 前部件的图层，然后将该图层混合到输出的位图中
	 */
	if( has_layer_graph ) {
		Graph_Init( &layer_graph );
		layer_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Copy( &layer_graph, &self_graph );
		Graph_Mix( &layer_graph, &content_graph,
			   Pos(content_rect.x, content_rect.y) );
		layer_graph.opacity = w->computed_style.opacity;
		Graph_Mix( &paint->canvas, &layer_graph, Pos(0,0) );
	}
	else if( has_content_graph ) {
		Graph_Mix( &paint->canvas, &content_graph,
			   Pos(content_rect.x, content_rect.y) );
	}
	Graph_Free( &layer_graph );
	Graph_Free( &self_graph );
	Graph_Free( &content_graph );
}

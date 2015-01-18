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
#include <LCUI/display.h>
#include <LCUI/widget_build.h>

static LCUI_BOOL painter_is_active = FALSE;
/** 记录需要进行绘制的部件 */
static LCUI_RBTree widget_paint_tree;

static int CompareWidget( void *data, const void *key )
{
	return data == key ? 0:(data > key ? 1:-1);
}

/** 初始化GUI部件绘制器 */
void LCUIWidgetPainter_Init(void)
{
	RBTree_Init( &widget_paint_tree );
	RBTree_SetDataNeedFree( &widget_paint_tree, FALSE );
	RBTree_OnJudge( &widget_paint_tree, CompareWidget );
	painter_is_active = TRUE;
}

/** 销毁GUI部件绘制器 */
void LCUIWidgetPainter_Destroy(void)
{
	painter_is_active = FALSE;
	RBTree_Destroy( &widget_paint_tree );
}

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
	case BORDER_BOX: box = &w->base.box.border; break;
	case GRAPH_BOX: box = &w->base.box.graph; break;
	case CONTENT_BOX: 
	default: box = &w->base.box.content; break;
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
	out_rect->x += (box->x - w->base.box.graph.x);
	out_rect->y += (box->y - w->base.box.graph.y);
}

/** 
 * 标记部件内的一个区域为无效的，以使其重绘
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
int Widget_InvalidateArea( LCUI_Widget w, LCUI_Rect *r, int box_type )
{
	LCUI_Rect rect;
	if( painter_is_active ) {
		return -1;
	}
	Widget_AdjustArea( w, r, &rect, box_type );
	/* 记录该部件，需要进行绘制 */
	RBTree_CustomInsert( &widget_paint_tree, w, w );
	return DirtyRectList_Add( &w->dirty_rects, &rect );
}

/** 
 * 获取部件中的无效区域
 * @param[in] widget	目标部件
 * @area[out] area	无效区域
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

/**  
 * 将部件的区域推送至屏幕
 * @param[in] w		目标部件
 * @param[in] r		矩形区域
 * @param[in] box_type	区域相对于何种框进行定位
 */
int Widget_PushAreaToScreen( LCUI_Widget w, LCUI_Rect *r, int box_type )
{
	LCUI_Rect rect;
	if( !w ) {
		return LCUIScreen_InvalidateArea( r );
	}
	Widget_AdjustArea( w, r, &rect, box_type );
	/* 向上级遍历，调整该矩形区域，直至根部件为止 */
	while( w && w != LCUIRootWidget ) {
		if( !w->style.visible ) {
			return 1;
		}
		rect.x += w->base.box.graph.x;
		rect.y += w->base.box.graph.y;
		w = w->parent;
		Widget_AdjustArea( w, &rect, &rect, CONTENT_BOX );
		if( rect.w <= 0 || rect.h <= 0 ) {
			return -2;
		}
	}
	return LCUIScreen_InvalidateArea( &rect );
}

static int Widget_DrawBackground( LCUI_Widget widget, LCUI_Rect *area )
{
	// ...
	return 0;
}

/** 
 * 执行重绘部件前的一些任务
 * @param[in] widget 需要重绘的部件
 * @param[out] area 需要进行重绘的区域
 * @returns 正常返回TRUE，没有无效区域则返回FALSE
 */
LCUI_BOOL Widget_BeginPaint( LCUI_Widget widget, LCUI_Rect *area )
{
	int ret;
	DEBUG_MSG("enter\n");
	ret = Widget_GetInvalidArea( widget, area );
	if( ret != 0 ) {
		DEBUG_MSG("quit1\n");
		return FALSE;
	}
	//Widget_DrawShadow( widget, area );
	//Widget_DrawBackground( widget, area );
	DEBUG_MSG("quit2\n");
	return TRUE;
}

/** 
 * 执行重绘部件后的一些任务
 * @param[in] widget	已被重绘的部件
 * @param[in] area	被重绘的区域
 */
void Widget_EndPaint( LCUI_Widget widget, LCUI_Rect *area )
{
	//Widget_DrawBorder( &graph, area );
	Widget_ValidateArea( widget, area, BORDER_BOX );
	Widget_PushAreaToScreen( widget, area, BORDER_BOX );
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
	case CONTENT_BOX:
		rect = w->base.box.content;
		break;
	case PADDING_BOX:
		rect = w->base.box.content;
		rect.x -= w->base.padding.left;
		rect.y -= w->base.padding.top;
		rect.w += w->base.padding.left;
		rect.w += w->base.padding.right;
		rect.h += w->base.padding.top;
		rect.h += w->base.padding.bottom;
		break;
	case BORDER_BOX:
		rect = w->base.box.border;
		break;
	case GRAPH_BOX:
	default:
		return -2;
	}
	/* 转换成相对坐标 */
	rect.x -= w->base.box.graph.x;
	rect.y -= w->base.box.graph.y;
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
 * 引用部件中指定区域内的图形
 * @param[in]	w		目标部件
 * @param[out]	graph		图形的引用
 * @param[in]	rect		被引用的区域
 * @param[in]	rect		实际引用的区域，相对于被引用的区域
 * @param[in]	box_type	该区域相对于何种范围框进行定位
 */
int Widget_QuoteGraph(	LCUI_Widget w, LCUI_Graph *graph, 
			LCUI_Rect *rect, int box_type )
{
	LCUI_Rect r;
	r = *rect;
	/* 从当前部件向上级部件遍历 */
	while( w ) {
		Widget_AdjustArea( w, &r, &r, box_type );
		switch( box_type ) {
		case CONTENT_BOX:
			r.x += (w->base.box.content.x - w->base.box.graph.x);
			r.y += (w->base.box.content.y - w->base.box.graph.y);
			break;
		case BORDER_BOX:
			r.x += (w->base.box.border.x - w->base.box.graph.x);
			r.y += (w->base.box.border.y - w->base.box.graph.y);
			break;
		case PADDING_BOX:
			r.x += (w->base.box.border.x - w->base.box.graph.x);
			r.y += (w->base.box.border.y - w->base.box.graph.y);
			r.x += w->base.padding.left;
			r.y += w->base.padding.top;
			break;
		case GRAPH_BOX:
		default: break;
		}
		/* 若这级部件有单独位图缓存，则用它的 */
		if( Graph_IsValid(&w->graph) ) {
			break;
		}
		/* 不可见就不处理 */
		if( !w->style.visible ) {
			return -1;
		}
		/* 该部件在父部件的内容框中可见，所以参照物改为内容框 */
		box_type = CONTENT_BOX;
		r.x += w->base.box.graph.x;
		r.y += w->base.box.graph.y;
		w = w->parent;
	}
	Graph_Quote( &w->graph, graph, r );
}

/** 默认的绘制函数 */
static void Widget_OnPaint( LCUI_Widget widget )
{
	LCUI_Rect rect;
	Widget_BeginPaint( widget, &rect );
	DEBUG_MSG("begin paint rect: %d,%d,%d,%d\n", rect.x, rect.y, rect.w, rect.h);
	Widget_EndPaint( widget, &rect );
}

/** 更新各个部件的无效区域中的内容 */
int LCUIWidget_ProcInvalidArea(void)
{
	LCUI_Widget w;
	LCUI_RBTreeNode *node;
	LCUI_WidgetClass *wc;

	if( !painter_is_active ) {
		return -1;
	}
	node = RBTree_First( &widget_paint_tree );
	DEBUG_MSG("tip1\n");
	/** 遍历当前需要更新位图缓存的部件 */
	while( node ) {
		w = (LCUI_Widget)node->data;
		/** 过滤掉不可见或无独立位图缓存的部件 */
		if( !w->style.visible || Graph_IsValid(&w->graph) ) {
			node = RBTree_Next( node );
			continue;
		}
		wc = LCUIWidget_GetClass( w->type_name );
		DEBUG_MSG("widget, %p, dirty rect num: %d\n", widget, widget->dirty_rects.used_node_num);
		/* 有多少个脏矩形就调用多少次部件的绘制函数 */
		while( w->dirty_rects.used_node_num > 0 ) {
			wc->methods.paint ? wc->methods.paint(w) : Widget_OnPaint(w);
		}
		node = RBTree_Next( node );
		RBTree_CustomErase( &widget_paint_tree, w );
	}
	DEBUG_MSG("tip2\n");
	return 0;
}

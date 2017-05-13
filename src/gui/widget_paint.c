/* ***************************************************************************
 * widget_paint.c -- LCUI widget paint module.
 *
 * Copyright (C) 2013-2016 by Liu Chao <lc-soft@live.cn>
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
 * 版权所有 (C) 2013-2016 归属于 刘超 <lc-soft@live.cn>
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

//#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/display.h>

typedef struct LCUI_RectGroupRec_ {
	LCUI_Widget widget;
	LinkedList rects;
}LCUI_RectGroupRec, *LCUI_RectGroup;

static struct LCUI_WidgetRenderModule {
	LCUI_BOOL active;
	RBTree groups;
	LinkedList rects;
} self = { 0 };

/** 判断部件是否有可绘制内容 */
static LCUI_BOOL Widget_IsPaintable( LCUI_Widget w )
{
	const LCUI_WidgetStyle *s = &w->computed_style;
	if( s->background.color.alpha > 0 ||
	    Graph_IsValid( &s->background.image ) ||
	    s->border.top.width > 0 || s->border.right.width > 0 ||
	    s->border.bottom.width > 0 || s->border.left.width > 0 ||
	    s->shadow.blur > 0 || s->shadow.spread > 0 ) {
		return TRUE;
	}
	return w->proto && w->proto->paint;
}

/**
 * 根据所处框区域，调整矩形
 * @param[in] w		目标部件
 * @param[in] in_rect	矩形
 * @param[out] out_rect	调整后的矩形
 * @param[in] box_type	区域相对于何种框进行定位
 */
static void Widget_AdjustArea(	LCUI_Widget w, LCUI_Rect *in_rect,
				LCUI_Rect *out_rect, int box_type )
{
	LCUI_RectF *box;
	switch( box_type ) {
	case SV_BORDER_BOX: box = &w->box.border; break;
	case SV_GRAPH_BOX: box = &w->box.graph; break;
	case SV_PADDING_BOX: box = &w->box.padding; break;
	case SV_CONTENT_BOX:
	default: box = &w->box.content; break;
	}
	/* 如果为NULL，则视为使用整个部件区域 */
	if( !in_rect ) {
		out_rect->x = out_rect->y = 0;
		out_rect->width = roundi( box->width );
		out_rect->height = roundi( box->height );
	} else {
		*out_rect = *in_rect;
		LCUIRect_ValidateArea( out_rect, roundi( box->width ),
				       roundi( box->height ) );
	}
	/* 将坐标转换成相对于图像呈现区的坐标 */
	out_rect->x += roundi( box->x - w->box.graph.x );
	out_rect->y += roundi( box->y - w->box.graph.y );
}

LCUI_BOOL Widget_InvalidateArea( LCUI_Widget widget,
				 LCUI_Rect *r, int box_type )
{
	int mode;
	LCUI_Rect rect;
	LCUI_RectF rectf;
	LCUI_Widget w = widget;
	LCUI_Widget root = LCUIWidget_GetRoot();
	LCUI_RectGroup group;

	if( !w ) {
		w = root;
	}
	mode = LCUIDisplay_GetMode();
	Widget_AdjustArea( w, r, &rect, box_type );
	rectf.x = rect.x + w->box.graph.x;
	rectf.y = rect.y + w->box.graph.y;
	rectf.width = (float)rect.width;
	rectf.height = (float)rect.height;
	while( w && w->parent ) {
		LCUIRectF_ValidateArea( &rectf, w->parent->box.padding.width,
					w->parent->box.padding.height );
		if( rectf.width < 0.5f || rectf.height < 0.5f ) {
			return FALSE;
		}
		if( mode != LCDM_SEAMLESS && w->parent == root ) {
			break;
		}
		w = w->parent;
		rectf.x += w->box.padding.x;
		rectf.y += w->box.padding.y;
	}
	RectF2Rect( rectf, rect );
	if( mode != LCDM_SEAMLESS ) {
		RectList_Add( &self.rects, &rect );
		return TRUE;
	}
	group = RBTree_CustomGetData( &self.groups, w );
	if( !group ) {
		group = NEW( LCUI_RectGroupRec, 1 );
		group->widget = w;
		LinkedList_Init( &group->rects );
		RBTree_CustomInsert( &self.groups, w, group );
	}
	return RectList_Add( &group->rects, &rect ) == 0;
}

size_t Widget_GetInvalidArea( LCUI_Widget w, LinkedList *rects )
{
	LCUI_RectGroup group;
	if( !w || w == LCUIWidget_GetRoot() ) {
		LinkedList_Concat( rects, &self.rects );
		return (size_t)rects->length;
	}
	group = RBTree_CustomGetData( &self.groups, w );
	if( group ) {
		LinkedList_Concat( rects, &group->rects );
	}
	return (size_t)rects->length;
}

static int OnCompareGroup( void *data, const void *keydata )
{
	LCUI_RectGroup group = data;
	return (int)((char*)group->widget - (char*)keydata);
}

static void OnDestroyGroup( void *data )
{
	LCUI_RectGroup group = data;
	RectList_Clear( &group->rects );
	group->widget = NULL;
}

void LCUIWidget_InitRenderer( void )
{
	RBTree_Init( &self.groups );
	RBTree_OnCompare( &self.groups, OnCompareGroup );
	RBTree_OnDestroy( &self.groups, OnDestroyGroup );
	LinkedList_Init( &self.rects );
	self.active = TRUE;
}

void LCUIWidget_ExitRenderer( void )
{
	self.active = FALSE;
	RectList_Clear( &self.rects );
	RBTree_Destroy( &self.groups );
}

/** 当前部件的绘制函数 */
static void Widget_OnPaint( LCUI_Widget w, LCUI_PaintContext paint )
{
	Widget_PaintBoxShadow( w, paint );
	Widget_PaintBakcground( w, paint );
	Widget_PaintBorder( w, paint );
	if( w->proto && w->proto->paint ) {
		w->proto->paint( w, paint );
	}
}

int Widget_ConvertArea( LCUI_Widget w, LCUI_Rect *in_rect,
			LCUI_Rect *out_rect, int box_type )
{
	LCUI_RectF rect;
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
		rect.width += w->padding.left;
		rect.width += w->padding.right;
		rect.height += w->padding.top;
		rect.height += w->padding.bottom;
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
	out_rect->x = in_rect->x - (int)rect.x;
	out_rect->y = in_rect->y - (int)rect.y;
	out_rect->width = in_rect->width;
	out_rect->height = in_rect->height;
	/* 裁剪掉超出范围的区域 */
	if( out_rect->x < 0 ) {
		out_rect->width += out_rect->x;
		out_rect->x = 0;
	}
	if( out_rect->y < 0 ) {
		out_rect->height += out_rect->y;
		out_rect->y = 0;
	}
	if( out_rect->x + out_rect->width > (int)rect.width ) {
		out_rect->width = (int)rect.width - out_rect->x;
	}
	if( out_rect->y + out_rect->height > (int)rect.height ) {
		out_rect->height = (int)rect.height - out_rect->y;
	}
	return 0;
}

#define ComputeActualPX(VAL) LCUIMetrics_ComputeActual( VAL, SVT_PX )

void Widget_Render( LCUI_Widget w, LCUI_PaintContext paint )
{
	LinkedListNode *node;
	float content_left, content_top;
	LCUI_PaintContextRec self_paint;
	LCUI_PaintContextRec child_paint;
	LCUI_Rect canvas_rect, content_rect;
	LCUI_Graph content_graph, self_graph, layer_graph;
	LCUI_BOOL has_overlay, has_content_graph = FALSE,
		has_self_graph = FALSE, has_layer_graph = FALSE,
		is_cover_border = FALSE, is_paintable;

	Graph_Init( &self_graph );
	Graph_Init( &layer_graph );
	Graph_Init( &content_graph );
	layer_graph.color_type = COLOR_TYPE_ARGB;
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
	is_paintable = Widget_IsPaintable( w );
	/* 如果部件有需要绘制的内容 */
	if( is_paintable ) {
		self_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Create( &self_graph, paint->rect.width,
				paint->rect.height );
		self_paint.canvas = self_graph;
		self_paint.rect = paint->rect;
		Widget_OnPaint( w, &self_paint );
		/* 若不需要缓存自身位图则直接绘制到画布上 */
		if( !has_self_graph ) {
			Graph_Mix( &paint->canvas, &self_graph,
				   0, 0, paint->with_alpha );
		}
	}
	/* 计算内容框相对于图层的坐标 */
	content_left = w->box.padding.x - w->box.graph.x;
	content_top = w->box.padding.y - w->box.graph.y;
	/* 获取内容框 */
	content_rect.x = ComputeActualPX( content_left );
	content_rect.y = ComputeActualPX( content_top );
	content_rect.width = ComputeActualPX( w->box.padding.width );
	content_rect.height = ComputeActualPX( w->box.padding.height );
	/* 获取内容框与脏矩形重叠的区域 */
	has_overlay = LCUIRect_GetOverlayRect(
		&content_rect, &paint->rect, &content_rect
	);
	/* 如果没有与内容框重叠，则跳过内容绘制 */
	if( !has_overlay ) {
		goto content_paint_done;
	}
	/* 将重叠区域的坐标转换为相对于脏矩形的坐标 */
	content_rect.x -= paint->rect.x;
	content_rect.y -= paint->rect.y;
	/* 若需要部件内容区的位图缓存 */
	if( has_content_graph ) {
		child_paint.with_alpha = TRUE;
		content_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Create( &content_graph, content_rect.width,
			      content_rect.height );
	} else {
		child_paint.with_alpha = paint->with_alpha;
		/* 引用该区域的位图，作为内容框的位图 */
		Graph_Quote( &content_graph, &paint->canvas, &content_rect );
	}
	/* 按照显示顺序，从底到顶，递归遍历子级部件 */
	LinkedList_ForEachReverse( node, &w->children_show ) {
		LCUI_RectF rect;
		LCUI_Rect child_rect;
		LCUI_Widget child = node->data;
		if( !child->computed_style.visible || 
		    child->state != WSTATE_NORMAL ) {
			continue;
		}
		/* 转换子部件区域，由相对于内容框转换为相对于当前脏矩形 */
		rect.x = child->box.graph.x + content_left;
		rect.y = child->box.graph.y + content_top;
		rect.width = child->box.graph.width;
		rect.height = child->box.graph.height;
		LCUIMetrics_ComputeRectActual( &child_rect, &rect );
		child_rect.x -= paint->rect.x;
		child_rect.y -= paint->rect.y;
		/* 获取与内容框重叠的区域，作为子部件的绘制区域 */
		has_overlay = LCUIRect_GetOverlayRect(
			&content_rect, &child_rect, &child_paint.rect
		);
		/* 区域无效则不绘制 */
		if( !has_overlay ) {
			continue;
		}
		/* 将子部件绘制区域转换成相对于当前部件内容框 */
		canvas_rect.x = child_paint.rect.x - content_rect.x;
		canvas_rect.y = child_paint.rect.y - content_rect.y;
		canvas_rect.width = child_paint.rect.width;
		canvas_rect.height = child_paint.rect.height;
		/* 将绘制区域转换为相对于子部件 */
		child_paint.rect.x -= child_rect.x;
		child_paint.rect.y -= child_rect.y;
		_DEBUG_MSG( "[%s]: canvas_rect:(%d,%d,%d,%d)\n", w->type,
			   canvas_rect.x, canvas_rect.y,
			   canvas_rect.width, canvas_rect.height );
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
		if( is_paintable ) {
			Graph_Copy( &layer_graph, &self_graph );
			Graph_Mix( &layer_graph, &content_graph,
				   content_rect.x, content_rect.y, TRUE );
		} else {
			Graph_Create( &layer_graph, paint->rect.width, 
				      paint->rect.height );
			Graph_Replace( &layer_graph, &content_graph, 
				       content_rect.x, content_rect.y );
		}
		layer_graph.opacity = w->computed_style.opacity;
		Graph_Mix( &paint->canvas, &layer_graph, 
			   0, 0, paint->with_alpha );
	}
	else if( has_content_graph ) {
		Graph_Mix( &paint->canvas, &content_graph,
			   content_rect.x, content_rect.y, TRUE );
	}
	Graph_Free( &layer_graph );
	Graph_Free( &self_graph );
	Graph_Free( &content_graph );
}

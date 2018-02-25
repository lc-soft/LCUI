/*
 * widget_paint.c -- LCUI widget paint module.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/display.h>

//#define DEBUG_FRAME_RENDER
#define ComputeActualPX(VAL) LCUIMetrics_ComputeActual( VAL, SVT_PX )

#ifdef DEBUG_FRAME_RENDER
#include <LCUI/image.h>
#endif

typedef struct LCUI_RectGroupRec_ {
	LCUI_Widget widget;
	LinkedList rects;
}LCUI_RectGroupRec, *LCUI_RectGroup;

typedef struct LCUI_WidgetRendererRec_ {
	float x, y;				/**< target widget position, it relative to root canvas */
	float content_top;			/**< content area top spacing, it relative to widget canvas */
	float content_left;			/**< content area left spacing, it relative to widget canvas */
	LCUI_Widget target;			/**< target widget */
	LCUI_PaintContext paint;		/**< current target widget paint context */
	LCUI_PaintContext root_paint;		/**< root paint context */
	LCUI_Graph content_graph;		/**< content canvas */
	LCUI_Graph self_graph;			/**< target widget canvas */
	LCUI_Graph layer_graph;			/**< layer canvas, used to mix content and self canvas with widget opacity */
	LCUI_Rect content_rect;			/**< actual paint rectangle in widget content rectangle, it relative to root canvas */
	LCUI_BOOL has_content_graph;
	LCUI_BOOL has_self_graph;
	LCUI_BOOL has_layer_graph;
	LCUI_BOOL is_cover_border;
	LCUI_BOOL can_render_self;
	LCUI_BOOL can_render_centent;
} LCUI_WidgetRendererRec, *LCUI_WidgetRenderer;

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
static void Widget_AdjustArea(	LCUI_Widget w, LCUI_RectF *in_rect,
				LCUI_RectF *out_rect, int box_type )
{
	LCUI_RectF *box;
	switch( box_type ) {
	case SV_BORDER_BOX: box = &w->box.border; break;
	case SV_GRAPH_BOX: box = &w->box.canvas; break;
	case SV_PADDING_BOX: box = &w->box.padding; break;
	case SV_CONTENT_BOX:
	default: box = &w->box.content; break;
	}
	/* 如果为NULL，则视为使用整个部件区域 */
	if( !in_rect ) {
		out_rect->x = out_rect->y = 0;
		out_rect->width = box->width;
		out_rect->height = box->height;
	} else {
		*out_rect = *in_rect;
		LCUIRectF_ValidateArea( out_rect, box->width, box->height );
	}
	/* 将坐标转换成相对于图像呈现区的坐标 */
	out_rect->x += box->x - w->box.canvas.x;
	out_rect->y += box->y - w->box.canvas.y;
}

void RectFToInvalidArea( const LCUI_RectF *rect, LCUI_Rect *area )
{
	LCUIMetrics_ComputeRectActual( area, rect );
}

void RectToInvalidArea( const LCUI_Rect *rect, LCUI_Rect *area )
{
	LCUI_RectF rectf;
	LCUIRect_ToRectF( rect, &rectf, 1.0f );
	LCUIMetrics_ComputeRectActual( area, &rectf );
}

LCUI_BOOL Widget_InvalidateArea( LCUI_Widget widget,
				 LCUI_RectF *in_rect, int box_type )
{
	int mode;
	LCUI_Rect area;
	LCUI_RectF rect;
	LCUI_Widget w = widget;
	LCUI_Widget root = LCUIWidget_GetRoot();
	LCUI_RectGroup group;

	if( !w ) {
		w = root;
	}
	mode = LCUIDisplay_GetMode();
	Widget_AdjustArea( w, in_rect, &rect, box_type );
	rect.x += w->box.canvas.x;
	rect.y += w->box.canvas.y;
	while( w && w->parent ) {
		LCUIRectF_ValidateArea( &rect, w->parent->box.padding.width,
					w->parent->box.padding.height );
		if( rect.width <= 0 || rect.height <= 0 ) {
			return FALSE;
		}
		if( mode != LCDM_SEAMLESS && w->parent == root ) {
			break;
		}
		w = w->parent;
		rect.x += w->box.padding.x;
		rect.y += w->box.padding.y;
	}
	RectFToInvalidArea( &rect, &area );
	if( mode != LCDM_SEAMLESS ) {
		RectList_Add( &self.rects, &area );
		return TRUE;
	}
	group = RBTree_CustomGetData( &self.groups, w );
	if( !group ) {
		group = NEW( LCUI_RectGroupRec, 1 );
		group->widget = w;
		LinkedList_Init( &group->rects );
		RBTree_CustomInsert( &self.groups, w, group );
	}
	return RectList_Add( &group->rects, &area ) == 0;
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

void LCUIWidget_FreeRenderer( void )
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
	rect.x -= w->box.canvas.x;
	rect.y -= w->box.canvas.y;
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

static LCUI_WidgetRenderer WidgetRenderer( LCUI_Widget w,
					   LCUI_PaintContext paint,
					   LCUI_WidgetRenderer parent )
{
	LCUI_RectF rect;
	LCUI_Rect paint_rect;
	ASSIGN( that, LCUI_WidgetRenderer );

	that->target = w;
	that->paint = paint;
	that->is_cover_border = FALSE;
	that->has_self_graph = FALSE;
	that->has_layer_graph = FALSE;
	that->has_content_graph = FALSE;
	if( parent ) {
		that->root_paint = parent->root_paint;
		that->x = parent->x + parent->content_left + w->box.canvas.x;
		that->y = parent->y + parent->content_top + w->box.canvas.y;
	} else {
		that->x = that->y = 0;
		that->root_paint = that->paint;
	}
	if( w->computed_style.opacity < 1.0 ) {
		that->has_self_graph = TRUE;
		that->has_content_graph = TRUE;
		that->has_layer_graph = TRUE;
	} else {
		/* if target has rounded corners border...
		...
		if( ... ) {
			that->has_content_graph = TRUE;
			that->is_cover_border = TRUE;
		}
		*/
	}
	Graph_Init( &that->self_graph );
	Graph_Init( &that->layer_graph );
	Graph_Init( &that->content_graph );
	that->layer_graph.color_type = COLOR_TYPE_ARGB;
	that->can_render_self = Widget_IsPaintable( w );
	if( that->can_render_self ) {
		that->self_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Create( &that->self_graph,
			      that->paint->rect.width,
			      that->paint->rect.height );
	}
	/* get content rectangle left spacing and top */
	that->content_left = w->box.padding.x - w->box.canvas.x;
	that->content_top = w->box.padding.y - w->box.canvas.y;
	/* get content rectangle, it relative to root canvas */
	rect.x = that->x + that->content_left;
	rect.y = that->y + that->content_top;
	rect.width = w->box.padding.width;
	rect.height = w->box.padding.height;
	LCUIMetrics_ComputeRectActual( &that->content_rect, &rect );
	/* convert position of paint rectangle to root canvas relative */
	rect.x = that->x;
	rect.y = that->y;
	LCUIMetrics_ComputeRectActual( &paint_rect, &rect );
	paint_rect.x += that->paint->rect.x;
	paint_rect.y += that->paint->rect.y;
	paint_rect.width = that->paint->rect.width;
	paint_rect.height = that->paint->rect.height;
	/* get actual paint rectangle in widget content rectangle */
	that->can_render_centent = LCUIRect_GetOverlayRect(
		&that->content_rect, &paint_rect, &that->content_rect
	);
	DEBUG_MSG( "[%s][%d/%d] content_rect: (%d,%d,%d,%d), "
		   "canvas_rect: (%d,%d,%d,%d)\n",
		   w->id, w->index,
		   w->parent ? w->parent->children_show.length : 1,
		   that->content_rect.x, that->content_rect.y,
		   that->content_rect.width, that->content_rect.height,
		   that->paint->canvas.quote.left,
		   that->paint->canvas.quote.top,
		   that->paint->canvas.width,
		   that->paint->canvas.height );
	if( !that->can_render_centent ) {
		return that;
	}
	if( that->has_content_graph ) {
		that->content_graph.color_type = COLOR_TYPE_ARGB;
		Graph_Create( &that->content_graph, that->content_rect.width,
			      that->content_rect.height );
	}
	return that;
}

static void WidgetRenderer_Delete( LCUI_WidgetRenderer renderer )
{
	Graph_Free( &renderer->layer_graph );
	Graph_Free( &renderer->self_graph );
	Graph_Free( &renderer->content_graph );
	free( renderer );
}

static size_t WidgetRenderer_Render( LCUI_WidgetRenderer renderer );

static size_t WidgetRenderer_RenderChildren( LCUI_WidgetRenderer that )
{
	size_t count = 0;
	LCUI_Widget child;
	LinkedListNode *node;
	LCUI_RectF rect;
	LCUI_PaintContextRec paint;
	LCUI_WidgetRenderer renderer;
	LCUI_Rect actual_rect, paint_rect;

	/* 按照显示顺序，从底到顶，递归遍历子级部件 */
	for( LinkedList_EachReverse( node, &that->target->children_show ) ) {
		child = node->data;
		if( !child->computed_style.visible ||
		    child->state != LCUI_WSTATE_NORMAL ) {
			continue;
		}
		rect.width = child->box.canvas.width;
		rect.height = child->box.canvas.height;
		rect.x = that->x + child->box.canvas.x + that->content_left;
		rect.y = that->y + child->box.canvas.y + that->content_top;
		/* 栅格化部件区域，即：转换为相对于根级部件的实际区域 */
		LCUIMetrics_ComputeRectActual( &actual_rect, &rect );
		if( !LCUIRect_GetOverlayRect( &that->content_rect,
					      &actual_rect,
					      &paint_rect ) ) {
			continue;
		}
		if( that->has_content_graph ) {
			paint.with_alpha = TRUE;
		} else {
			paint.with_alpha = that->paint->with_alpha;
		}
		paint.rect = paint_rect;
		/* 转换绘制区域坐标为相对于自身图层区域 */
		paint.rect.x -= actual_rect.x;
		paint.rect.y -= actual_rect.y;
		/* 转换绘制区域坐标为相对于部件内容区域，作为子部件的绘制区域 */
		paint_rect.x -= that->root_paint->rect.x;
		paint_rect.y -= that->root_paint->rect.y;
		Graph_Quote( &paint.canvas, &that->root_paint->canvas,
			     &paint_rect );
		renderer = WidgetRenderer( child, &paint, that );
		count += WidgetRenderer_Render( renderer );
		WidgetRenderer_Delete( renderer );
	}
	return count;
}

static size_t WidgetRenderer_Render( LCUI_WidgetRenderer renderer )
{
	size_t count = 0;
	LCUI_PaintContextRec self_paint;
	LCUI_WidgetRenderer that = renderer;
#ifdef DEBUG_FRAME_RENDER
	char filename[256];
	static size_t frame = 0;
#endif
	/* 如果部件有需要绘制的内容 */
	if( that->can_render_self ) {
		count += 1;
		self_paint = *that->paint;
		self_paint.canvas = that->self_graph;
		Widget_OnPaint( that->target, &self_paint );
#ifdef DEBUG_FRAME_RENDER
		sprintf( filename,
			 "frame-%lu-%s-self-paint-(%d,%d,%d,%d).png",
			 frame++, renderer->target->id,
			 self_paint.rect.x, self_paint.rect.y,
			 self_paint.rect.width,
			 self_paint.rect.height );
		LCUI_WritePNGFile( filename, &self_paint.canvas );
#endif
		/* 若不需要缓存自身位图则直接绘制到画布上 */
		if( !that->has_self_graph ) {
			Graph_Mix( &that->paint->canvas, &that->self_graph,
				   0, 0, that->paint->with_alpha );
#ifdef DEBUG_FRAME_RENDER
			sprintf( filename, "frame-%lu-%s-canvas.png",
				 frame++, renderer->target->id );
			LCUI_WritePNGFile( filename,
					   &that->root_paint->canvas );
#endif
		}
	}
	if( that->can_render_centent ) {
		count += WidgetRenderer_RenderChildren( that );
	}
	/* 如果与圆角边框重叠，则裁剪掉边框外的内容 */
	if( that->is_cover_border ) {
		/* content_graph ... */
	}
	if( !that->has_layer_graph ) {
		if( that->has_content_graph ) {
			Graph_Mix( &that->paint->canvas, &that->content_graph,
				   that->content_rect.x,
				   that->content_rect.y, TRUE );
		}
#ifdef DEBUG_FRAME_RENDER
		sprintf( filename, "frame-%lu-%s-canvas.png",
				frame++, renderer->target->id );
		LCUI_WritePNGFile( filename,
				   &that->root_paint->canvas );
#endif
		return count;
	}
	/* 若需要绘制的是当前部件图层，则先混合部件自身位图和内容位图，得出当
	 * 前部件的图层，然后将该图层混合到输出的位图中
	 */
	if( that->can_render_self ) {
		Graph_Copy( &that->layer_graph, &that->self_graph );
		Graph_Mix( &that->layer_graph, &that->content_graph,
			   that->content_rect.x,
			   that->content_rect.y, TRUE );
	} else {
		Graph_Create( &that->layer_graph, that->paint->rect.width,
			      that->paint->rect.height );
		Graph_Replace( &that->layer_graph, &that->content_graph,
			       that->content_rect.x,
			       that->content_rect.y );
	}
	that->layer_graph.opacity = that->target->computed_style.opacity;
	Graph_Mix( &that->paint->canvas, &that->layer_graph,
		   0, 0, that->paint->with_alpha );
#ifdef DEBUG_FRAME_RENDER
	sprintf( filename, "frame-%lu-%s-layer.png",
		 frame++, renderer->target->id );
	LCUI_WritePNGFile( filename, &that->layer_graph );
	sprintf( filename, "frame-%lu-%s-canvas.png",
		 frame++, renderer->target->id );
	LCUI_WritePNGFile( filename, &that->root_paint->canvas );
#endif
	return count;
}

size_t Widget_Render( LCUI_Widget w, LCUI_PaintContext paint )
{
	size_t count;
	LCUI_WidgetRenderer renderer;
	renderer = WidgetRenderer( w, paint, NULL );
	count = WidgetRenderer_Render( renderer );
	WidgetRenderer_Delete( renderer );
	return count;
}

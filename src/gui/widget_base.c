/* ***************************************************************************
 * widget_base.c -- the widget base operation set.
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
 * widget_base.c -- 部件的基本操作集。
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
#include <LCUI/widget_build.h>

#define NEW_ONE(TYPE) (TYPE*)malloc(sizeof(TYPE))

/**
 * $ 可以看成是 Self，指当前模块(Widget)、函数的操作对象，这样省的手工为每个函
 * 数名加 Widget_ 前缀了。
 */
#define $(FUNC_NAME) Widget_##FUNC_NAME

static struct LCUI_WidgetFull LCUIRootWidgetData;	/**< 根级部件 */
LCUI_Widget LCUIRootWidget = &LCUIRootWidgetData;	/**< 创建外部引用 */

/** 追加子部件 */
int $(Append)( LCUI_Widget container, LCUI_Widget widget )
{
	int i, n;
	LCUI_Widget old_container;
	LCUI_WidgetEvent e;
	_DEBUG_MSG("container: %p, widget: %p\n", container, widget);
	if( !container || !widget || container == widget->parent ) {
		return -1;
	}
	if( container == widget ) {
		return -2;
	}
	if( widget->parent ) {
		old_container = widget->parent;
	} else {
		goto remove_done;
	}

	/* 移除在之前的容器中的记录 */
	n = LinkedList_GetTotal( &old_container->children );
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &old_container->children, i );
		if( LinkedList_Get(&old_container->children) == widget ) {
			LinkedList_Delete( &old_container->children );
			break;
		}
	}
	/* 如果是从根级部件中移出，则触发 WET_REMOVE 事件 */
	if( i < n && old_container == LCUIRootWidget ) {
		e.type_name = "TopLevelWidget";
		e.target = widget;
		Widget_PostEvent( LCUIRootWidget, &e, (int*)WET_REMOVE );
	}
	n = LinkedList_GetTotal( &old_container->children_show );
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &old_container->children_show, i );
		if( LinkedList_Get(&old_container->children_show) == widget ) {
			LinkedList_Delete( &old_container->children_show );
			break;
		}
	}

remove_done:

	widget->parent = container;
	LinkedList_AddData( &container->children, widget );
	LinkedList_AddData( &container->children_show, widget );
	/* 如果是添加至根部件内，则触发 WET_ADD 事件 */
	if( container == LCUIRootWidget ) {
		int ret;
		e.type_name = "TopLevelWidget";
		e.target = widget;
		ret = Widget_PostEvent( LCUIRootWidget, &e, (int*)WET_ADD );
		_DEBUG_MSG("post done, ret = %d\n", ret);
	}
	Widget_UpdateTaskStatus( widget );
	_DEBUG_MSG("tip\n");
	return 0;
}

/** 前置显示 */
int $(Front)( LCUI_Widget widget )
{
	int i, n, src_pos = -1, des_pos = -1;
	LCUI_Widget parent, child;

	parent = widget->parent ? widget->parent:LCUIRootWidget;
	n = LinkedList_GetTotal( &parent->children_show );
	/* 先在队列中找到自己，以及z-index值小于或等于它的第一个部件 */
	for( i=0; i<n; ++i ) {
		LinkedList_Goto( &parent->children_show, i );
		child = (LCUI_Widget)LinkedList_Get( &parent->children_show );
		if( child == widget ) {
			src_pos = i;
			continue;
		}
		if( des_pos >= 0 ) {
			if( src_pos >= 0 ) {
				break;
			}
			continue;
		}
		/* 如果该位置的图层的z-index值不大于自己 */
		if( child->style.z_index <= widget->style.z_index ) {
			/* 如果未找到自己的源位置 */
			if( src_pos == -1 ) {
				des_pos = i;
				continue;
			}
			/* 否则，退出循环，因为已经在前排了 */
			break;
		}
	}
	/* 没有找到就退出 */
	if( des_pos == -1 || src_pos == -1 ) {
		return -1;
	}
	/* 找到的话就移动位置 */
	LinkedList_Goto( &parent->children_show, src_pos );
	LinkedList_MoveTo( &parent->children_show, des_pos );
	// XXX
	return 0;
}

/** 部件析构函数 */
static void $(OnDestroy)( void *arg )
{
	LCUI_WidgetEvent e;
	LCUI_Widget widget = (LCUI_Widget)arg;

	Widget_DestroyTaskBox( widget );
	LCUIEventBox_Destroy( widget->event );
	widget->event = NULL;
	LinkedList_Destroy( &widget->children );
	LinkedList_Destroy( &widget->children_show );
	DirtyRectList_Destroy( &widget->dirty_rects );
	/* 如果是从根级部件中移出，则触发 WET_REMOVE 事件 */
	if( widget->parent == LCUIRootWidget ) {
		e.type_name = "TopLevelWidget";
		e.target = widget;
		Widget_PostEvent( LCUIRootWidget, &e, (int*)WET_REMOVE );
	}
}

/** 构造函数 */
static void $(Init)( LCUI_Widget widget )
{
	widget->style.z_index = 0;
	widget->style.x.type = SVT_NONE;
	widget->style.y.type = SVT_NONE;
	widget->style.width.type = SVT_AUTO;
	widget->style.height.type = SVT_AUTO;
	widget->style.box_sizing = CONTENT_BOX;
	widget->style.opacity = 1.0;
	widget->style.margin.top.px = 0;
	widget->style.margin.right.px = 0;
	widget->style.margin.bottom.px = 0;
	widget->style.margin.left.px = 0;
	widget->style.margin.top.type = SVT_PX;
	widget->style.margin.right.type = SVT_PX;
	widget->style.margin.bottom.type = SVT_PX;
	widget->style.margin.left.type = SVT_PX;
	widget->style.padding.top.px = 0;
	widget->style.padding.right.px = 0;
	widget->style.padding.bottom.px = 0;
	widget->style.padding.left.px = 0;
	widget->style.padding.top.type = SVT_PX;
	widget->style.padding.right.type = SVT_PX;
	widget->style.padding.bottom.type = SVT_PX;
	widget->style.padding.left.type = SVT_PX;
	widget->event = LCUIEventBox_Create();
	memset( &widget->base, 0, sizeof(widget->base));
	widget->parent = NULL;
	widget->title = NULL;
	widget->event = LCUIEventBox_Create();
	Widget_InitTaskBox( widget );
	Background_Init( &widget->style.background );
	BoxShadow_Init( &widget->style.shadow );
	Border_Init( &widget->style.border );
	LinkedList_Init( &widget->children, sizeof(struct LCUI_WidgetFull) );
	LinkedList_Init( &widget->children_show, 0 );
	LinkedList_SetDestroyFunc( &widget->children, $(OnDestroy) );
	LinkedList_SetDataNeedFree( &widget->children, TRUE );
	LinkedList_SetDataNeedFree( &widget->children_show, FALSE );
	DirtyRectList_Init( &widget->dirty_rects );
	Graph_Init( &widget->graph );
}

/** 新建一个GUI部件 */
LCUI_Widget $(New)( const char *type_name )
{
	LCUI_Widget widget = NEW_ONE(struct LCUI_WidgetFull);
	$(Init)(widget);
	return widget;
}

/** 获取当前点命中的最上层可见部件 */
LCUI_Widget $(At)( LCUI_Widget widget, int x, int y )
{
	int i, n;
	LCUI_Widget target = NULL;

	do {
		n = LinkedList_GetTotal( &widget->children_show );
		for( i=0; i<n; ++i ) {
			LinkedList_Goto( &widget->children_show, i );
			target = (LCUI_Widget)
			LinkedList_Get( &widget->children_show );
			if( !target->style.visible ) {
				continue;
			}
			if( target->style.x.px <= x && target->style.y.px
			 && target->style.x.px + x < target->style.w.px
			 && target->style.y.px + x < target->style.h.px ) {
				widget = target;
				break;
			 }
		}
	} while( i >= n );

	return target;
}

/** 设置部件为顶级部件 */
int $(Top)( LCUI_Widget w )
{
	_DEBUG_MSG("tip\n");
	return $(Append)( LCUIRootWidget, w );
}

/** 设置部件标题 */
void $(SetTitleW)( LCUI_Widget w, const wchar_t *title )
{
	int len;
	LCUI_WidgetTask t;

	if( w->title ) {
		free( w->title );
		w->title = NULL;
	}
	len = wcslen(title) + 1;
	w->title = (wchar_t*)malloc(sizeof(wchar_t)*len);
	wcsncpy( w->title, title, len );
	Widget_AddTask( w, (t.type = WTT_TITLE, &t) );
}

/** 获取内边距框占用的矩形区域 */
void $(GetPaddingRect)( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取内容框占用的矩形区域 */
void $(GetContentRect)( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取边框盒占用的矩形区域 */
void $(GetBorderRect)( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取部件当前占用的矩形区域（包括阴影区域） */
void $(GetOuterRect)( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取部件当前可见的矩形区域 */
void $(GetValidRect)( LCUI_Widget widget, LCUI_Rect *rect )
{
	LCUI_Rect container_rect;
	$(GetRect)( widget, rect );
	while( widget ) {
		$(GetRect)( widget->parent, &container_rect );
		if( rect->x < 0 ) {
			rect->w = rect->w + rect->x;
			rect->x = 0;
		}
		if( rect->y < 0 ) {
			rect->h = rect->h + rect->y;
			rect->y = 0;
		}
		if( rect->x + rect->w > container_rect.w ) {
			rect->w = container_rect.w - rect->x;
		}
		if( rect->y + rect->h > container_rect.h ) {
			rect->h = container_rect.h - rect->y;
		}
		/** 参照物改变，区域的坐标需要加上父部件的坐标 */
		rect->x += container_rect.x;
		rect->y += container_rect.y;
		/** 切换至上一级部件 */
		widget = widget->parent;
	}
}

/** 计算坐标 */
void $(ComputeCoord)( LCUI_Widget w )
{
	// 需要考虑到其它定位相关的属性
	// code ...

	switch( w->style.x.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->base.x =  w->parent->base.box.content.width;
		w->base.x *= w->style.x.scale;
		break;
	case SVT_PX:
		w->base.x = w->style.x.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->base.x = 0;
		break;
	}
	switch( w->style.y.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->base.y = w->parent->base.box.content.height;
		w->base.y *= w->style.y.scale;
		break;
	case SVT_PX:
		w->base.y = w->style.y.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->base.y = 0;
		break;
	}
	/* 以x、y为基础 */
	w->base.box.border.x = w->base.x;
	w->base.box.border.y = w->base.y;
	w->base.box.content.x = w->base.x;
	w->base.box.content.y = w->base.y;
	w->base.box.outer.x = w->base.x;
	w->base.box.outer.y = w->base.y;
	w->base.box.graph.x = w->base.x;
	w->base.box.graph.y = w->base.y;
	/* 计算各个框的坐标 */
	w->base.box.content.x += w->style.border.left.width + w->base.padding.left;
	w->base.box.content.y += w->style.border.top.width + w->base.padding.top;
	w->base.box.outer.x -= w->base.margin.left;
	w->base.box.outer.y -= w->base.margin.top;
	w->base.box.graph.x -= BoxShadow_GetBoxX(&w->style.shadow);
	w->base.box.graph.y -= BoxShadow_GetBoxY(&w->style.shadow);
}

/** 更新位图尺寸 */
void $(UpdateGraphBox)( LCUI_Widget w )
{
	LCUI_Rect *rb = &w->base.box.border;
	LCUI_Rect *rg = &w->base.box.graph;
	rg->x = w->base.x - BoxShadow_GetBoxX( &w->style.shadow );
	rg->y = w->base.y - BoxShadow_GetBoxY( &w->style.shadow );
	rg->width = BoxShadow_GetWidth( &w->style.shadow, rb->width );
	rg->height = BoxShadow_GetHeight( &w->style.shadow, rb->height );
	Graph_Create( &w->graph, rg->width, rg->height );
}

/** 计算尺寸 */
void $(ComputeSize)( LCUI_Widget w )
{
	switch( w->style.width.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->base.width = w->parent->base.box.content.width;
		w->base.width *= w->style.width.scale;
		break;
	case SVT_PX:
		w->base.width = w->style.width.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->base.width = 0;
		break;
	}
	switch( w->style.height.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->base.height = w->parent->base.box.content.height;
		w->base.height *= w->style.height.scale;
		break;
	case SVT_PX:
		w->base.height = w->style.height.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->base.height = 0;
		break;
	}

	w->base.box.border.width = w->base.width;
	w->base.box.border.height = w->base.height;
	w->base.box.content.width = w->base.width;
	w->base.box.content.height = w->base.height;
	/* 如果是以边框盒作为尺寸调整对象，则需根据边框盒计算内容框尺寸 */
	if( w->style.box_sizing == BORDER_BOX ) {
		/* 名字太长了，都放一行里代码会太长，只好分解成多行了 */
		w->base.box.content.width -= w->style.border.left.width;
		w->base.box.content.width -= w->style.border.right.width;
		w->base.box.content.width -= w->base.padding.left;
		w->base.box.content.width -= w->base.padding.right;
		w->base.box.content.height -= w->style.border.top.width;
		w->base.box.content.height -= w->style.border.bottom.width;
		w->base.box.content.height -= w->base.padding.top;
		w->base.box.content.height -= w->base.padding.bottom;
	} else {
		/* 否则是以内容框作为尺寸调整对象，需计算边框盒的尺寸 */
		w->base.box.border.width += w->style.border.left.width;
		w->base.box.border.width += w->style.border.right.width;
		w->base.box.border.width += w->base.padding.left;
		w->base.box.border.width += w->base.padding.right;
		w->base.box.border.height += w->style.border.top.width;
		w->base.box.border.height += w->style.border.bottom.width;
		w->base.box.border.height += w->base.padding.top;
		w->base.box.border.height += w->base.padding.bottom;
	}
}

/** 计算内边距 */
void $(ComputePadding)( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_StyleVar *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->style.padding.top,
			&w->base.padding.top,
			w->parent->base.box.content.height
		}, {
			&w->style.padding.right,
			&w->base.padding.right,
			w->parent->base.box.content.width
		}, {
			&w->style.padding.bottom,
			&w->base.padding.bottom,
			w->parent->base.box.content.height
		}, {
			&w->style.padding.left,
			&w->base.padding.left,
			w->parent->base.box.content.width
		}
	};
	for( i=0; i<4; ++i ) {
		switch( map[i].value->type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			result = map[i].size * map[i].value->scale / 100;
			*map[i].buffer = (int)result;
			break;
		case SVT_PX:
			*map[i].buffer = map[i].value->px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			// ...
			break;
		}
	}
}

/** 计算内边距 */
void $(ComputeMargin)( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_StyleVar *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->style.margin.top,
			&w->base.margin.top,
			w->parent->base.box.content.height
		}, {
			&w->style.margin.right,
			&w->base.margin.right,
			w->parent->base.box.content.width
		}, {
			&w->style.margin.bottom,
			&w->base.margin.bottom,
			w->parent->base.box.content.height
		}, {
			&w->style.margin.left,
			&w->base.margin.left,
			w->parent->base.box.content.width
		}
	};
	for( i=0; i<4; ++i ) {
		switch( map[i].value->type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			result = map[i].size * map[i].value->scale / 100;
			*map[i].buffer = (int)result;
			break;
		case SVT_PX:
			*map[i].buffer = map[i].value->px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			// ...
			break;
		}
	}
}

/** 设置内边距 */
void $(SetPadding)( LCUI_Widget w, int top, int right, int bottom, int left )
{
	LCUI_WidgetTask t;
	w->style.padding.top.px = top;
	w->style.padding.right.px = left;
	w->style.padding.bottom.px = left;
	w->style.padding.left.px = left;
	w->style.padding.top.type = SVT_PX;
	w->style.padding.right.type = SVT_PX;
	w->style.padding.bottom.type = SVT_PX;
	w->style.padding.left.type = SVT_PX;
	$(ComputeMargin)( w );
	$(ComputeSize)( w );
	$(ComputeCoord)( w );
	Widget_AddTask( w, (t.type = WTT_AUTO_SIZE, &t) );
	Widget_AddTask( w, (t.type = WTT_AUTO_LAYOUT, &t) );
}

void $(SetPaddingS)(
	LCUI_Widget w,
	const char *top,
	const char *right,
	const char *bottom,
	const char *left
)
{

}

/** 设置外边距 */
void $(SetMargin)( LCUI_Widget w, int top, int right, int bottom, int left )
{
	LCUI_WidgetTask t;
	w->style.margin.top.px = top;
	w->style.margin.right.px = left;
	w->style.margin.bottom.px = left;
	w->style.margin.left.px = left;
	w->style.margin.top.type = SVT_PX;
	w->style.margin.right.type = SVT_PX;
	w->style.margin.bottom.type = SVT_PX;
	w->style.margin.left.type = SVT_PX;
	$(ComputeMargin)( w );
	$(ComputeSize)( w );
	$(ComputeCoord)( w );
	Widget_AddTask( w, (t.type = WTT_AUTO_SIZE, &t) );
	Widget_AddTask( w, (t.type = WTT_AUTO_LAYOUT, &t) );
}

/** 设置左边距 */
void $(SetLeft)( LCUI_Widget w, const char *value )
{

}

/** 设置顶边距 */
void $(SetTop)( LCUI_Widget w, const char *value )
{

}

/** 移动部件位置 */
void $(Move)( LCUI_Widget w, int top, int left )
{
	LCUI_WidgetTask t;
	t.type = WTT_MOVE;
	/* 记录当前的图形呈现框的坐标 */
	t.move.x = w->base.box.graph.x;
	t.move.y = w->base.box.graph.y;
	w->style.y.px = top;
	w->style.x.px = left;
	w->style.y.type = SVT_PX;
	w->style.x.type = SVT_PX;
	// 重新计算各个区域的坐标
	$(ComputeCoord)( w );
	Widget_AddTask( w, &t );
}

/** 调整部件尺寸 */
void $(Resize)( LCUI_Widget w, int width, int height )
{
	LCUI_WidgetTask t;

	t.type = WTT_RESIZE;
	t.resize.w = w->base.width;
	t.resize.h = w->base.height;
	w->style.width.px = width;
	w->style.height.px = height;
	w->style.width.type = SVT_PX;
	w->style.height.type = SVT_PX;
	$(ComputeSize)( w );
	Widget_AddTask( w, &t );
	Widget_AddTask( w, (t.type = WTT_AUTO_LAYOUT, &t) );
}

void $(SetWidth)( LCUI_Widget w, const char *value )
{

}

void $(SetHeight)( LCUI_Widget w, const char *value )
{

}

void $(Show)( LCUI_Widget w )
{
	LCUI_WidgetTask t;
	t.type = WTT_SHOW;
	t.visible = w->style.visible;
	w->style.visible = TRUE;
	_DEBUG_MSG("tip, type: %d, %d\n", t.type, WTT_SHOW);
	Widget_AddTask( w, &t );
}

void $(Hide)( LCUI_Widget w )
{
	LCUI_WidgetTask t;
	t.type = WTT_SHOW;
	t.visible = w->style.visible;
	w->style.visible = FALSE;
	Widget_AddTask( w, &t );
}

void $(SetBackgroundColor)( LCUI_Widget w, LCUI_Color color )
{
	w->style.background.color = color;
}

#define PULL(WIDGET, STYLE) WIDGET##->base.style.STYLE## = WIDGET##->style.##STYLE
#define PUSH(WIDGET, STYLE) WIDGET##->style.STYLE## = WIDGET##->base.style.##STYLE

/** 拉取现有样式至缓存区 */
void $(PullStyle)( LCUI_Widget w, int style )
{
	if(!(style & WSS_POSITION) ) goto PULL_WSS_BOX;
	PULL(w, position);
	PULL(w, x);
	PULL(w, y);
	PULL(w, z_index);
	PULL(w, top);
	PULL(w, right);
	PULL(w, bottom);
	PULL(w, left);
PULL_WSS_BOX:
	if( !(style & WSS_BOX) ) goto PULL_WSS_BACKGROUND;
	PULL(w, box_sizing);
	PULL(w, width);
	PULL(w, height);
	PULL(w, margin);
	PULL(w, padding);
PULL_WSS_BACKGROUND:
	if( !(style & WSS_BACKGROUND) ) goto PULL_WSS_BORDER;
	PULL(w, background);
PULL_WSS_BORDER:
	if( !(style & WSS_BORDER) ) goto PULL_WSS_SHADOW;
	PULL(w, border);
PULL_WSS_SHADOW:
	if( !(style & WSS_SHADOW) ) goto PULL_DONE;
	PULL(w, shadow);
PULL_DONE:
	return;
}

/** 推送缓存区中的样式，以让部件应用新样式 */
void $(PushStyle)( LCUI_Widget w, int style )
{
	LCUI_WidgetTask t;
	if(!(style & WSS_POSITION) ) goto PUSH_WSS_BOX;
	PUSH(w, position);
	PUSH(w, x);
	PUSH(w, y);
	PUSH(w, z_index);
	PUSH(w, top);
	PUSH(w, right);
	PUSH(w, bottom);
	PUSH( w, left );
	Widget_AddTask( w, (t.type = WTT_MOVE, &t) );
PUSH_WSS_BOX:
	if( !(style & WSS_BOX) ) goto PUSH_WSS_BACKGROUND;
	PUSH(w, box_sizing);
	PUSH(w, width);
	PUSH(w, height);
	PUSH(w, margin);
	PUSH( w, padding );
	Widget_AddTask( w, (t.type = WTT_MARGIN, &t) );
	Widget_AddTask( w, (t.type = WTT_PADDING, &t) );
PUSH_WSS_BACKGROUND:
	if( !(style & WSS_BACKGROUND) ) goto PUSH_WSS_BORDER;
	PUSH( w, background );
	Widget_AddTask( w, (t.type = WTT_BODY, &t) );
PUSH_WSS_BORDER:
	if( !(style & WSS_BORDER) ) goto PUSH_WSS_SHADOW;
	PUSH( w, border );
	Widget_AddTask( w, (t.type = WTT_BODY, &t) );
PUSH_WSS_SHADOW:
	if( !(style & WSS_SHADOW) ) goto PUSH_DONE;
	PUSH( w, shadow );
	Widget_AddTask( w, (t.type = WTT_SHADOW, &t) );
PUSH_DONE:
	return;
}

void LCUIModule_Widget_Init(void)
{
	$(Init)(LCUIRootWidget);
	Widget_SetTitleW( LCUIRootWidget, L"LCUI's widget container" );
	LCUIWidget_Task_Init();
}

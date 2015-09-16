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
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/widget/button.h>
#include <LCUI/gui/widget/sidebar.h>

#define NEW_ONE(TYPE) (TYPE*)malloc(sizeof(TYPE))

LCUI_Widget LCUIRootWidget = NULL;

/** 获取根级部件 */
LCUI_Widget LCUIWidget_GetRoot(void)
{
	return LCUIRootWidget;
}

/** 追加子部件 */
int Widget_Append( LCUI_Widget container, LCUI_Widget widget )
{
	LCUI_Widget w, old_container;
	LCUI_WidgetEvent e;

	DEBUG_MSG("container: %p, widget: %p\n", container, widget);
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
	LinkedList_ForEach( w, 0, &old_container->children ) {
		if( w != widget ) {
			continue;
		}
		LinkedList_Delete( &old_container->children );
		/* 如果是从根级部件中移出，则触发 WET_REMOVE 事件 */
		if( old_container == LCUIRootWidget ) {
			e.type_name = "TopLevelWidget";
			e.target = widget;
			Widget_PostEvent( LCUIRootWidget, &e, (int*)WET_REMOVE );
		}
		break;
	}
	LinkedList_ForEach( w, 0, &old_container->children_show ) {
		if( w == widget ) {
			LinkedList_Delete( &old_container->children_show );
			break;
		}
	}

remove_done:

	widget->parent = container;
	LinkedList_Append( &container->children, widget );
	LinkedList_Goto( &container->children_show, 0 );
	LinkedList_Insert( &container->children_show, widget );
	/* 如果是添加至根部件内，则触发 WET_ADD 事件 */
	if( container == LCUIRootWidget ) {
		e.type_name = "TopLevelWidget";
		e.target = widget;
		Widget_PostEvent( LCUIRootWidget, &e, (int*)WET_ADD );
	}
	Widget_AddTaskToSpread( widget, WTT_REFRESH_STYLE );
	Widget_UpdateTaskStatus( widget );
	DEBUG_MSG("tip\n");
	return 0;
}

/** 前置显示 */
int Widget_Front( LCUI_Widget widget )
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
		if( child->computed_style.z_index <= widget->computed_style.z_index ) {
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
static void Widget_OnDestroy( void *arg )
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
static void Widget_Init( LCUI_Widget widget )
{
	memset( widget, 0, sizeof(struct LCUI_WidgetRec_));
	widget->custom_style = StyleSheet();
	widget->cached_style = StyleSheet();
	widget->style = StyleSheet();
	widget->computed_style.position = SV_STATIC;
	widget->computed_style.display = SV_BLOCK;
	widget->inherited_style = StyleSheet();
	widget->computed_style.width.type = SVT_AUTO;
	widget->computed_style.height.type = SVT_AUTO;
	widget->computed_style.box_sizing = SV_CONTENT_BOX;
	widget->computed_style.opacity = 1.0;
	widget->computed_style.margin.top.type = SVT_PX;
	widget->computed_style.margin.right.type = SVT_PX;
	widget->computed_style.margin.bottom.type = SVT_PX;
	widget->computed_style.margin.left.type = SVT_PX;
	widget->computed_style.padding.top.type = SVT_PX;
	widget->computed_style.padding.right.type = SVT_PX;
	widget->computed_style.padding.bottom.type = SVT_PX;
	widget->computed_style.padding.left.type = SVT_PX;
	widget->event = LCUIEventBox_Create();
	widget->event = LCUIEventBox_Create();
	Widget_InitTaskBox( widget );
	Background_Init( &widget->computed_style.background );
	BoxShadow_Init( &widget->computed_style.shadow );
	Border_Init( &widget->computed_style.border );
	LinkedList_Init( &widget->children, sizeof(struct LCUI_WidgetRec_) );
	LinkedList_Init( &widget->children_show, 0 );
	LinkedList_SetDestroyFunc( &widget->children, Widget_OnDestroy );
	LinkedList_SetDataNeedFree( &widget->children, TRUE );
	LinkedList_SetDataNeedFree( &widget->children_show, FALSE );
	DirtyRectList_Init( &widget->dirty_rects );
	Graph_Init( &widget->graph );
	LCUIMutex_Init( &widget->mutex );
}

/** 
 * 获取与选择器相匹配的第一个部件
 * 该方法主要用于精确搜索某个部件
 * @param[in] selector 选择器
 * @return  如果能找到匹配的部件，则返回该部件，否则返回 NULL
 */
LCUI_Widget LCUIWidget_Get( const char *selector )
{
	return NULL;
}

/** 
 * 查询与选择器相匹配的部件列表
 * @param[in]	selector 选择器
 * @param[out]	list 部件列表
 * @return 匹配到的部件总数
 */
int LCUIWidget_Find( const char *selector, LinkedList *list )
{
	return 0;
}

/** 新建一个GUI部件 */
LCUI_Widget LCUIWidget_New( const char *type_name )
{
	LCUI_Widget widget;
	LCUI_WidgetClass *wc;

	widget = NEW_ONE(struct LCUI_WidgetRec_);
	Widget_Init( widget );
	if( type_name ) {
		widget->type = strdup( type_name );
		wc = LCUIWidget_GetClass( type_name );
		if( wc ) {
			wc->methods.init( widget );
		}
	}
	Widget_AddTask( widget, WTT_REFRESH_STYLE );
	return widget;
}

/** 获取当前点命中的最上层可见部件 */
LCUI_Widget Widget_At( LCUI_Widget widget, int x, int y )
{
	LCUI_BOOL is_hit;
	LCUI_Widget target = widget, c = NULL;
	do {
		is_hit = FALSE;
		LinkedList_ForEach( c, 0, &target->children_show ) {
			if( !c->computed_style.visible ) {
				continue;
			}
			if( LCUIRect_HasPoint(&c->box.border, x, y) ) {
				target = c;
				x -= c->box.content.x;
				y -= c->box.content.y;
				is_hit = TRUE;
				break;
			 }
		}
	} while( is_hit );
	return (target == widget) ? NULL:target;
}

/** 设置部件为顶级部件 */
int Widget_Top( LCUI_Widget w )
{
	DEBUG_MSG("tip\n");
	return Widget_Append( LCUIRootWidget, w );
}

/** 设置部件标题 */
void Widget_SetTitleW( LCUI_Widget w, const wchar_t *title )
{
	int len;
	wchar_t *new_title, *old_title;

	len = wcslen(title) + 1;
	new_title = (wchar_t*)malloc(sizeof(wchar_t)*len);
	if( !new_title ) {
		return;
	}
	wcsncpy( new_title, title, len );
	old_title = w->title;
	w->title = new_title;
	if( old_title ) {
		free( old_title );
	}
	Widget_AddTask( w, WTT_TITLE );
}

/** 获取内边距框占用的矩形区域 */
void Widget_GetPaddingRect( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取内容框占用的矩形区域 */
void Widget_GetContentRect( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取边框盒占用的矩形区域 */
void Widget_GetBorderRect( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取部件当前占用的矩形区域（包括阴影区域） */
void Widget_GetOuterRect( LCUI_Widget widget, LCUI_Rect *rect )
{
	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

/** 获取部件当前可见的矩形区域 */
void Widget_GetValidRect( LCUI_Widget widget, LCUI_Rect *rect )
{
	LCUI_Rect container_rect;
	Widget_GetRect( widget, rect );
	while( widget ) {
		Widget_GetRect( widget->parent, &container_rect );
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
void Widget_ComputePosition( LCUI_Widget w )
{
	/* 如果有指定定位方式为绝对定位，则以样式表中指定的位置为准 */
	if( w->computed_style.position == SV_ABSOLUTE ) {
		switch( w->cached_style[key_left].type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			w->x = w->parent->box.content.width;
			w->x *= w->cached_style[key_left].value_scale;
			break;
		case SVT_PX:
			w->x = w->cached_style[key_left].value_px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			w->x = 0;
			break;
		}
		switch( w->cached_style[key_top].type ) {
		case SVT_SCALE:
			if( !w->parent ) {
				break;
			}
			w->y = w->parent->box.content.height;
			w->y *= w->cached_style[key_top].value_scale;
			break;
		case SVT_PX:
			w->y = w->cached_style[key_top].value_px;
			break;
		case SVT_NONE:
		case SVT_AUTO:
		default:
			w->y = 0;
			break;
		}
	}
	/* 以x、y为基础 */
	w->box.border.x = w->x;
	w->box.border.y = w->y;
	w->box.content.x = w->x;
	w->box.content.y = w->y;
	w->box.outer.x = w->x;
	w->box.outer.y = w->y;
	w->box.graph.x = w->x;
	w->box.graph.y = w->y;
	/* 计算各个框的坐标 */
	w->box.content.x += w->computed_style.border.left.width + w->padding.left;
	w->box.content.y += w->computed_style.border.top.width + w->padding.top;
	w->box.outer.x -= w->margin.left;
	w->box.outer.y -= w->margin.top;
	w->box.graph.x -= BoxShadow_GetBoxX(&w->computed_style.shadow);
	w->box.graph.y -= BoxShadow_GetBoxY(&w->computed_style.shadow);
}

/** 更新位图尺寸 */
void Widget_UpdateGraphBox( LCUI_Widget w )
{
	LCUI_Rect *rb = &w->box.border;
	LCUI_Rect *rg = &w->box.graph;
	rg->x = w->x - BoxShadow_GetBoxX( &w->computed_style.shadow );
	rg->y = w->y - BoxShadow_GetBoxY( &w->computed_style.shadow );
	rg->width = BoxShadow_GetWidth( &w->computed_style.shadow, rb->width );
	rg->height = BoxShadow_GetHeight( &w->computed_style.shadow, rb->height );
	/* 如果有会产生透明效果的样式 */
	if( w->computed_style.border.bottom_left_radius > 0
	 || w->computed_style.border.bottom_right_radius > 0
	 || w->computed_style.border.top_left_radius > 0
	 || w->computed_style.border.top_right_radius > 0
	 || w->computed_style.background.color.alpha < 255
	 || w->computed_style.shadow.blur > 0 ) {
		w->graph.color_type = COLOR_TYPE_ARGB;
	} else {
		w->graph.color_type = COLOR_TYPE_RGB;
	}
	Graph_Create( &w->graph, rg->width, rg->height );
}

/** 获取合适的内容框大小 */
void Widget_GetContentSize( LCUI_Widget w, int *width, int *height )
{
	int n;
	LCUI_Widget child;

	*width = 0;
	*height = 0;
	LinkedList_ForEach( child, 0, &w->children_show ) {
		if( !child->computed_style.visible ) {
			continue;
		}
		n = child->box.outer.x + child->box.outer.width;
		if( n > *width ) {
			*width = n;
		}
		n = child->box.outer.y + child->box.outer.height;
		if( n > *height ) {
			*height = n;
		}
	}
}

/** 计算尺寸 */
void Widget_ComputeSize( LCUI_Widget w )
{
	switch( w->computed_style.width.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->width = w->parent->box.content.width;
		w->width *= w->computed_style.width.scale;
		break;
	case SVT_PX:
		w->width = w->computed_style.width.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->width = 0;
		break;
	}
	switch( w->computed_style.height.type ) {
	case SVT_SCALE:
		if( !w->parent ) {
			break;
		 }
		w->height = w->parent->box.content.height;
		w->height *= w->computed_style.height.scale;
		break;
	case SVT_PX:
		w->height = w->computed_style.height.px;
		break;
	case SVT_NONE:
	case SVT_AUTO:
	default:
		w->height = 0;
		break;
	}
	if( w->computed_style.height.type == SVT_AUTO
	 || w->computed_style.width.type == SVT_AUTO ) {
		LCUI_WidgetClass *wc;
		int width, height;

		wc = LCUIWidget_GetClass( w->type );
		if( wc && wc->methods.autosize ) {
			wc->methods.autosize( w, &width, &height );
		} else {
			Widget_GetContentSize( w, &width, &height );
		}
		if( w->computed_style.width.type == SVT_AUTO ) {
			w->width = width;
		}
		if( w->computed_style.height.type == SVT_AUTO ) {
			w->height = height;
		}
	}
	w->box.border.width = w->width;
	w->box.border.height = w->height;
	w->box.content.width = w->width;
	w->box.content.height = w->height;
	/* 如果是以边框盒作为尺寸调整对象，则需根据边框盒计算内容框尺寸 */
	if( w->computed_style.box_sizing == SV_BORDER_BOX ) {
		/* 名字太长了，都放一行里代码会太长，只好分解成多行了 */
		w->box.content.width -= w->computed_style.border.left.width;
		w->box.content.width -= w->computed_style.border.right.width;
		w->box.content.width -= w->padding.left;
		w->box.content.width -= w->padding.right;
		w->box.content.height -= w->computed_style.border.top.width;
		w->box.content.height -= w->computed_style.border.bottom.width;
		w->box.content.height -= w->padding.top;
		w->box.content.height -= w->padding.bottom;
	} else {
		/* 否则是以内容框作为尺寸调整对象，需计算边框盒的尺寸 */
		w->box.border.width += w->computed_style.border.left.width;
		w->box.border.width += w->computed_style.border.right.width;
		w->box.border.width += w->padding.left;
		w->box.border.width += w->padding.right;
		w->box.border.height += w->computed_style.border.top.width;
		w->box.border.height += w->computed_style.border.bottom.width;
		w->box.border.height += w->padding.top;
		w->box.border.height += w->padding.bottom;
	}
	/* 先暂时用边框盒区域作为外部区域，等加入外边距设置后再改 */
	w->box.outer = w->box.border;
}

/** 计算内边距 */
void Widget_ComputePadding( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_StyleVar *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->computed_style.padding.top,
			&w->padding.top,
			w->parent->box.content.height
		}, {
			&w->computed_style.padding.right,
			&w->padding.right,
			w->parent->box.content.width
		}, {
			&w->computed_style.padding.bottom,
			&w->padding.bottom,
			w->parent->box.content.height
		}, {
			&w->computed_style.padding.left,
			&w->padding.left,
			w->parent->box.content.width
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
void Widget_ComputeMargin( LCUI_Widget w )
{
	int i;
	double result;
	struct {
		LCUI_StyleVar *value;
		int *buffer;
		int size;
	} map[4] = {
		{
			&w->computed_style.margin.top,
			&w->margin.top,
			w->parent->box.content.height
		}, {
			&w->computed_style.margin.right,
			&w->margin.right,
			w->parent->box.content.width
		}, {
			&w->computed_style.margin.bottom,
			&w->margin.bottom,
			w->parent->box.content.height
		}, {
			&w->computed_style.margin.left,
			&w->margin.left,
			w->parent->box.content.width
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
void Widget_SetPadding( LCUI_Widget w, int top, int right, int bottom, int left )
{

}

void Widget_SetPaddingS(
	LCUI_Widget w,
	const char *top,
	const char *right,
	const char *bottom,
	const char *left
)
{

}

/** 设置外边距 */
void Widget_SetMargin( LCUI_Widget w, int top, int right, int bottom, int left )
{

}

/** 设置左边距 */
void Widget_SetLeft( LCUI_Widget w, const char *value )
{

}

/** 设置顶边距 */
void Widget_SetTop( LCUI_Widget w, const char *value )
{

}

/** 移动部件位置 */
void Widget_Move( LCUI_Widget w, int left, int top )
{
	SetStyle( w->custom_style, key_top, top, px );
	SetStyle( w->custom_style, key_left, left, px );
	DEBUG_MSG("top = %d, left = %d\n", top, left);
	Widget_Update( w, FALSE );
}

/** 调整部件尺寸 */
void Widget_Resize( LCUI_Widget w, int width, int height )
{
	SetStyle( w->custom_style, key_width, width, px );
	SetStyle( w->custom_style, key_height, height, px );
	Widget_Update( w, FALSE );
}

void Widget_Show( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, TRUE, boolean );
	Widget_Update( w, FALSE );
}

void Widget_Hide( LCUI_Widget w )
{
	SetStyle( w->custom_style, key_visible, FALSE, boolean );
	Widget_Update( w, FALSE );
}

void Widget_SetBackgroundColor( LCUI_Widget w, LCUI_Color color )
{
	w->computed_style.background.color = color;
}

void Widget_Lock( LCUI_Widget w )
{
	LCUIMutex_Lock( &w->mutex );
}

void Widget_Unlock( LCUI_Widget w )
{
	LCUIMutex_Unlock( &w->mutex );
}

int strlist_add_str( char ***strlist, const char *str )
{
	int i, len;
	char **newlist;

	if( *strlist ) {
		for( i = 0; (*strlist)[i]; ++i ) {
			if( strcmp((*strlist)[i], str) == 0 ) {
				return 0;
			}
		}
		len = i + 2;
		newlist = (char**)realloc( *strlist, len * sizeof(char*) );
	} else {
		i = 0;
		newlist = (char**)malloc( sizeof(char*) * 2 );
	}
	if( newlist == NULL ) {
		return -1;
	}
	newlist[i] = strdup(str);
	newlist[i+1] = NULL;
	*strlist = newlist;
	return 1;
}

LCUI_BOOL strlist_has_str( char **strlist, const char *str )
{
	int i;
	if( !strlist ) {
		return FALSE;
	}
	for( i = 0; strlist[i]; ++i ) {
		if( strcmp(strlist[i], str) == 0 ) {
			return TRUE;
		}
	}
	return FALSE;
}

int strlist_remove_str( char ***strlist, const char *str )
{
	int i, pos, len;
	char **newlist;

	if( !*strlist ) {
		return 0;
	}
	for( pos = 0,i = 0; (*strlist)[i] != NULL; ++i ) {
		if( strcmp((*strlist)[i], str) == 0 ) {
			pos = i;
		}
	}
	if( pos == 0 && i < 2 ) {
		free( *strlist );
		*strlist = NULL;
		return 1;
	}
	len = i - 1;
	newlist = (char**)malloc( i * sizeof(char*) );
	for( i = 0; i < pos; ++i ) {
		newlist[i] = (*strlist)[i];
	}
	for( i = pos; i < len; ++i ) {
		newlist[i] = (*strlist)[i+1];
	}
	newlist[i] = NULL;
	free( (*strlist)[pos] );
	free( *strlist );
	*strlist = newlist;
	return 1;
}

/** 为部件添加一个类 */
int Widget_AddClass( LCUI_Widget w, const char *class_name )
{
	if( strlist_add_str( &w->classes, class_name ) != 1 ) {
		return 0;
	}
	// 标记需要更新该部件及子级部件的样式表
	Widget_AddTaskToSpread( w, WTT_UPDATE_STYLE );
	return 1;
}

/** 判断部件是否包含指定的类 */
LCUI_BOOL Widget_HasClass( LCUI_Widget w, const char *class_name )
{
	return strlist_has_str( w->classes, class_name );
}

/** 从部件中移除一个类 */
int Widget_RemoveClass( LCUI_Widget w, const char *class_name )
{
	if( strlist_remove_str( &w->classes, class_name ) != 1 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w, WTT_REFRESH_STYLE );
	return 1;
}

/** 为部件添加一个状态 */
int Widget_AddStatus( LCUI_Widget w, const char *status_name )
{
	if( strlist_add_str( &w->pseudo_classes, status_name ) != 1 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w,  WTT_REFRESH_STYLE );
	return 1;
}

/** 判断部件是否包含指定的状态 */
LCUI_BOOL Widget_HasStatus( LCUI_Widget w, const char *status_name )
{
	return strlist_has_str( w->pseudo_classes, status_name );
}

/** 从部件中移除一个状态 */
int Widget_RemoveStatus( LCUI_Widget w, const char *status_name )
{
	if( strlist_remove_str( &w->pseudo_classes, status_name ) != 1 ) {
		return 0;
	}
	Widget_AddTask( w, WTT_REFRESH_STYLE );
	Widget_AddTaskToSpread( w,  WTT_REFRESH_STYLE );
	return 1;
}

/** 更新子部件的布局 */
void Widget_UpdateLayout( LCUI_Widget w )
{
	struct {
		int x, y;
		int line_height;
		LCUI_Widget prev;
		int prev_display;
		int max_width;
	} ctx = { 0 };
	LCUI_Widget child;

	ctx.max_width = 256;
	for( child = w; child; child = child->parent ) {
		if( child->computed_style.width.type != SVT_AUTO ) {
			ctx.max_width = child->box.content.width;
			break;
		}
	}
	LinkedList_ForEach( child, 0, &w->children ) {
		if( child->computed_style.position != SV_STATIC ) {
			continue;
		}
		switch( child->computed_style.display ) {
		case SV_NONE: continue;
		case SV_BLOCK:
			child->x = ctx.x;
			child->y = ctx.y;
			ctx.x = 0;
			ctx.line_height = 0;
			ctx.y += child->box.outer.height;
			break;
		case SV_INLINE_BLOCK:
			if( child->box.outer.height > ctx.line_height ) {
				ctx.line_height = child->box.outer.height;
			}
			if( ctx.prev && ctx.prev_display != SV_INLINE_BLOCK ) {
				ctx.x = 0;
				ctx.y += ctx.line_height;
				break;
			}
			ctx.x += child->box.outer.width;
			break;
		default: continue;
		}
		Widget_AddTask( child, WTT_POSITION );
		ctx.prev = child;
		ctx.prev_display = child->computed_style.display;
	}
	if( w->computed_style.height.type == SVT_AUTO
	 || w->computed_style.width.type == SVT_AUTO ) {
		Widget_AddTask( w, WTT_RESIZE );
	}
}

void LCUI_InitWidget(void)
{
	LCUIWidget_InitTask();
	LCUIWidget_InitEvent();
	LCUIWidget_InitLibrary();
	LCUIWidget_InitStyle();
	LCUIWidget_AddTextView();
	LCUIWidget_AddButton();
	LCUIWidget_AddSideBar();
	LCUIRootWidget = LCUIWidget_New("root");
	Widget_SetTitleW( LCUIRootWidget, L"LCUI Display" );
}

void LCUI_ExitWidget(void)
{

}

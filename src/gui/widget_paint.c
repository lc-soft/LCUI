
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/display.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/widget_build.h>

static LCUI_BOOL painter_is_active = FALSE;
/** 记录需要进行绘制的部件 */
static LCUI_RBTree widget_paint_tree;

/** 初始化GUI部件绘制器 */
void LCUIWidgetPainter_Init(void)
{
	RBTree_Init( &widget_paint_tree );
	painter_is_active = TRUE;
}

/** 销毁GUI部件绘制器 */
void LCUIWidgetPainter_Destroy(void)
{
	painter_is_active = FALSE;
	RBTree_Destroy( &widget_paint_tree );
}

/* 设置部件内无效区域，该无效区域将被重绘 */
int Widget_InvalidateArea( LCUI_Widget widget, LCUI_Rect *r )
{
	LCUI_Rect rect;
	/** 如果为NULL，那将整个部件区域需要刷新 */
	if( !r ) {
		rect.x = rect.y = 0;
		rect.w = widget->base.outerWidth;
		rect.h = widget->base.outerHeight;
		r = &rect;
	}
	LCUIRect_ValidateArea( r, Size(widget->base.outerWidth, widget->base.outerHeight) );
	DEBUG_MSG("widget:%p, rect: %d,%d,%d,%d\n", widget, r->x, r->y, r->w, r->h);
	if( painter_is_active ) {
		/* 记录该部件，需要进行绘制 */
		RBTree_Insert( &widget_paint_tree, (int)widget, NULL );
		return DirtyRectList_Add( &widget->dirty_rects, r );
	}
	return -1;
}

/** 获取部件中的无效区域 */
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

/** 使部件中的一块区域有效化 */
void Widget_ValidateArea( LCUI_Widget widget, LCUI_Rect *area )
{
	LCUI_Rect rect;
	if( !area ) {
		rect.x = rect.y = 0;
		rect.w = widget->base.outerWidth;
		rect.h = widget->base.outerHeight;
		area= &rect;
	}
	DirtyRectList_Delete( &widget->dirty_rects, area );
}

/** 将部件的区域推送至屏幕 */
int Widget_PushAreaToScreen( LCUI_Widget widget, LCUI_Rect *area )
{
	int n;
	LCUI_Rect rect;
	LCUI_Widget root;

	if( !widget ) {
		return LCUIScreen_InvalidateArea( area );
	}

	if( !area ) {
		rect.x = rect.y = 0;
		rect.w = widget->base.outerWidth;
		rect.h = widget->base.outerHeight;
		area = &rect;
	}
	root = LCUIRootWidget;
	while( widget && widget != root ) {
		if( !widget->base.isVisible ) {
			return 1;
		}
		if( area->x < 0 ) {
			area->w += area->x;
			area->x = 0;
		}
		if( area->y < 0 ) {
			area->h += area->y;
			area->y = 0;
		}
		if( area->x + area->w > widget->base.outerWidth ) {
			area->w = widget->base.outerWidth - area->x;
		}
		if( area->y + area->h > widget->base.outerHeight ) {
			area->h = widget->base.outerHeight - area->y;
		}
		/* 加上所在部件的坐标 */
		area->x += widget->base.x;
		area->y += widget->base.y;
		/* 加上父级部件的内边距 */
		if( !widget->parent ) {
			break;
		}
		
		/* 切换至父级部件 */
		widget = widget->parent;
		area->x += widget->style.padding.left.px;
		area->y += widget->style.padding.top.px;
		/* 计算父部件的内边距框，然后再调整矩形区域 */
		n = widget->base.outerWidth - widget->style.padding.right.px;
		if( area->x + area->w > n ) {
			area->w = n - area->x;
		}
		n = widget->base.outerHeight - widget->style.padding.bottom.px;
		if( area->y + area->h > n ) {
			area->h = n - area->y;
		}

		if( area->w <= 0 || area->h <= 0 ) {
			return -1;
		}
	}
	return LCUIScreen_InvalidateArea( area );
}

static int Widget_DrawBackground( LCUI_Widget widget, LCUI_Rect area )
{
	LCUI_Graph graph;
	Widget_QuoteInnerGraph( widget, &graph, &area );
	// ...
	return 0;
}

/* 获取部件内部区域的位图 */
int Widget_QuoteInnerGraph( LCUI_Widget widget, LCUI_Graph *graph, LCUI_Rect *r )
{
	LCUI_Rect rect;
	if( !r ) {
		rect.x = 0;
		rect.y = 0;
		rect.w = widget->base.outerWidth;
		rect.h = widget->base.outerHeight;
	} else {
		rect.x = r->x;
		rect.y = r->y;
		rect.w = r->w;
		rect.h = r->h;
		LCUIRect_ValidateArea( &rect, Size(widget->base.outerWidth, widget->base.outerHeight) );
	}

	//rect.x += BoxShadow_GetBoxX( &widget->style.shadow );
	//rect.y += BoxShadow_GetBoxY( &widget->style.shadow );
	return Graph_Quote( graph, &widget->glayer.graph, rect );
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
	Widget_DrawBackground( widget, *area );
	DEBUG_MSG("quit2\n");
	return TRUE;
}

/** 执行重绘部件后的一些任务 */
void Widget_EndPaint( LCUI_Widget widget, LCUI_Rect *area )
{
	LCUI_Graph graph;
	Widget_QuoteInnerGraph( widget, &graph, area );
	//Graph_DrawBorderEx( &graph, widget->border, *area );
	Widget_ValidateArea( widget, area );
	Widget_PushAreaToScreen( widget, area );
}

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
	int count = 0, old_num;
	LCUI_Widget widget;
	LCUI_RBTreeNode *node;
	LCUI_WidgetClass *wc;

	if( !painter_is_active ) {
		return -1;
	}
	node = RBTree_First( &widget_paint_tree );
	if( !node ) {
		return 0;
	}
	DEBUG_MSG("tip1\n");
	while( node ) {
		widget = (LCUI_Widget)node->key;
		wc = LCUIWidget_GetClass( widget->type_name );
		DEBUG_MSG("widget, %p, dirty rect num: %d\n", widget, widget->dirty_rects.used_node_num);
		old_num = widget->dirty_rects.used_node_num;
		/* 有多少个脏矩形就调用多少次部件的绘制函数 */
		while( widget->dirty_rects.used_node_num > 0 ) {
			wc->methods.paint ? wc->methods.paint(widget):FALSE;
			if( widget->dirty_rects.used_node_num >= old_num ) {
				++count;
				if( count > 10 ) {
					break;
				}
			} else {
				count = 0;
				widget->dirty_rects.used_node_num;
			}
		}
		if( count > 0 ) {
			_DEBUG_MSG("warning: widget(%s): "
				"the dirty-rect of does not reduce.\n",
				widget->type_name);
		}
		node = RBTree_Next( node );
		RBTree_Erase( &widget_paint_tree, (int)widget );
	}
	DEBUG_MSG("tip2\n");
	return 1;
}

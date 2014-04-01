#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H
#include LC_DISPLAY_H

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
LCUI_API int Widget_InvalidateArea( LCUI_Widget *widget, LCUI_Rect *r )
{
	LCUI_Rect rect;
	/** 如果为NULL，那将整个部件区域需要刷新 */
	if( !r ) {
		rect.x = rect.y = 0;
		rect.w = widget->size.w;
		rect.h = widget->size.h;
		r = &rect;
	}
	if( painter_is_active ) {
		/* 记录该部件，需要进行绘制 */
		RBTree_Insert( &widget_paint_tree, (int)widget, NULL );
		return DirtyRectList_Add( &widget->dirty_rect, r );
	}
	return -1;
}

/** 获取部件中的无效区域 */
LCUI_API int Widget_GetInvalidArea( LCUI_Widget *widget, LCUI_Rect *area )
{
	LCUI_Rect *p_rect;
	LinkedList_Goto( &widget->dirty_rect, 0 );
	p_rect = (LCUI_Rect*)LinkedList_Get( &widget->dirty_rect );
	if( !p_rect ) {
		return -1;
	}
	*area = *p_rect;
	return 0;
}

/** 使部件中的一块区域有效化 */
LCUI_API void Widget_ValidateArea( LCUI_Widget *widget, LCUI_Rect *area )
{
	LCUI_Rect rect;
	if( !area ) {
		rect.x = rect.y = 0;
		rect.w = widget->size.w;
		rect.h = widget->size.h;
		area= &rect;
	}
	DirtyRectList_Delete( &widget->dirty_rect, area );
}

/** 将部件的区域推送至屏幕 */
LCUI_API int Widget_PushAreaToScreen( LCUI_Widget *widget, LCUI_Rect *area )
{
	int n;
	LCUI_Rect rect;
	LCUI_Widget *root;

	if( !widget ) {
		return LCUIScreen_InvalidateArea( area );
	}

	if( !area ) {
		rect.x = rect.y = 0;
		rect.w = widget->size.w;
		rect.h = widget->size.h;
		area = &rect;
	}
	if( area->x < 0 ) {
		area->w += area->x;
		area->x = 0;
	}
	if( area->y < 0 ) {
		area->h += area->y;
		area->y = 0;
	}
	if( area->x + area->w > widget->size.w ) {
		area-> w = widget->size.w - area->x;
	}
	if( area->y + area->h > widget->size.h ) {
		area-> h = widget->size.h - area->y;
	}
	if( area->w <= 0 || area->h <= 0 ) {
		return -1;
	}

	root = RootWidget_GetSelf();
	while( widget->parent && widget->parent != root ) {
		/* 加上所在部件的坐标 */
		area->x += widget->pos.x;
		area->y += widget->pos.y;
		/* 加上父级部件的内边距 */
		area->x += widget->parent->glayer->padding.left;
		area->y += widget->parent->glayer->padding.top;
		/* 计算父部件的内边距框，然后再调整矩形区域 */
		n = widget->parent->glayer->padding.left;
		n += widget->parent->glayer->padding.right;
		n = widget->size.w - n;
		if( area->x + area->w > n  ) {
			area->w = n - area->w;
		}
		n = widget->parent->glayer->padding.top;
		n += widget->parent->glayer->padding.bottom;
		n = widget->size.h - n;
		if( area->y + area->h > n  ) {
			area->h = n - area->h;
		}
		/* 切换至父级部件 */
		widget = widget->parent;
	}
	return LCUIScreen_InvalidateArea( area );
}

static int Widget_DrawBackground( LCUI_Widget *widget, LCUI_Rect area )
{
	LCUI_Graph *graph;
	LCUI_Background *bg;

	graph = Widget_GetSelfGraph( widget );
	bg = &widget->background;
	/* 如果背景透明，则使用覆盖模式将背景图绘制到部件上 */
	if( bg->transparent ) {
		return Graph_FillImageEx( graph, &bg->image, bg->layout, area );
	}
	/* 否则，使用叠加模式 */
	return Graph_FillImageWithColorEx( graph, &bg->image, bg->layout,
							bg->color, area );
}

/** 
 * 执行重绘部件前的一些任务
 * @param[in] widget 需要重绘的部件
 * @param[out] area 需要进行重绘的区域
 * @returns 正常返回TRUE，没有无效区域则返回FALSE
 */
LCUI_API LCUI_BOOL Widget_BeginPaint( LCUI_Widget *widget, LCUI_Rect *area )
{
	int ret;
	ret = Widget_GetInvalidArea( widget, area );
	if( ret != 0 ) {
		return FALSE;
	}
	Widget_DrawBackground( widget, *area );
	return TRUE;
}

/** 执行重绘部件后的一些任务 */
LCUI_API void Widget_EndPaint( LCUI_Widget *widget, LCUI_Rect *area )
{
	LCUI_Graph *graph;
	graph = Widget_GetSelfGraph( widget );
	Graph_DrawBorderEx( graph, widget->border, *area );
	Widget_ValidateArea( widget, area );
	Widget_PushAreaToScreen( widget, area );
}

/** 更新各个部件的无效区域中的内容 */
LCUI_API int LCUIWidget_ProcInvalidArea(void)
{
	int count = 0, old_num;
	LCUI_Widget *widget;
	LCUI_RBTreeNode *node;

	if( !painter_is_active ) {
		return -1;
	}
	node = RBTree_First( &widget_paint_tree );
	if( !node ) {
		return 0;
	}
	while( node ) {
		widget = (LCUI_Widget*)node->key;
		old_num = widget->dirty_rect.used_node_num;
		/* 有多少个脏矩形就地阿姨多少次部件的绘制函数 */
		while( widget->dirty_rect.used_node_num > 0 ) {
			WidgetFunc_Call( widget, FUNC_TYPE_PAINT );
			if( widget->dirty_rect.used_node_num >= old_num ) {
				++count;
				if( count > 10 ) {
					break;
				}
			} else {
				count = 0;
				widget->dirty_rect.used_node_num;
			}
		}
		if( count > 0 ) {
			_DEBUG_MSG("warning: widget(%s): "
				"the dirty-rect of does not reduce.\n",
				widget->type_name.string ? 
				widget->type_name.string:"unknown");
		}
		node = RBTree_Next( node );
	}
	return 1;
}

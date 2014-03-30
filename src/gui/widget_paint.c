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

/* 在指定部件的内部区域内设定需要刷新的区域 */
LCUI_API int Widget_InvalidateArea( LCUI_Widget *widget, LCUI_Rect rect )
{
	/* 记录该部件，需要进行绘制 */
	RBTree_Insert( &widget_paint_tree, (int)widget, NULL );
	return DirtyRectList_Add( &widget->dirty_rect, &rect );
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
LCUI_API void Widget_ValidateArea( LCUI_Widget *widget, LCUI_Rect area )
{
	DirtyRectList_Delete( &widget->dirty_rect, &area );
}

/** 将部件的区域推送至屏幕 */
LCUI_API int Widget_PushAreaToScreen( LCUI_Widget *widget, LCUI_Rect area )
{
	int n;
	LCUI_Widget *root;

	if( area.x < 0 ) {
		area.w += area.x;
		area.x = 0;
	}
	if( area.y < 0 ) {
		area.h += area.y;
		area.y = 0;
	}
	if( area.x + area.w > widget->size.w ) {
		area. w = widget->size.w - area.x;
	}
	if( area.y + area.h > widget->size.h ) {
		area. h = widget->size.h - area.y;
	}
	if( area.w <= 0 || area.h <= 0 ) {
		return -1;
	}

	root = RootWidget_GetSelf();
	while( widget->parent && widget->parent != root ) {
		/* 加上所在部件的坐标 */
		area.x += widget->pos.x;
		area.y += widget->pos.y;
		/* 加上父级部件的内边距 */
		area.x += widget->parent->glayer->padding.left;
		area.y += widget->parent->glayer->padding.top;
		/* 计算父部件的内边距框，然后再调整矩形区域 */
		n = widget->parent->glayer->padding.left;
		n += widget->parent->glayer->padding.right;
		n = widget->size.w - n;
		if( area.x + area.w > n  ) {
			area.w = n - area.w;
		}
		n = widget->parent->glayer->padding.top;
		n += widget->parent->glayer->padding.bottom;
		n = widget->size.h - n;
		if( area.y + area.h > n  ) {
			area.h = n - area.h;
		}
		/* 切换至父级部件 */
		widget = widget->parent;
	}
	return LCUIScreen_InvalidateArea( area );
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
	while( node ) {
		old_num = widget->dirty_rect.used_node_num;
		widget = (LCUI_Widget*)node->key;
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
	return 0;
}

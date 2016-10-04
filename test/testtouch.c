/** testtouch.c -- test touch support */

#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>

/** 触点绑定记录 */
typedef struct TouchPointBindingRec_ {
	int point_id;			/**< 触点 ID */
	LCUI_Widget widget;		/**< 部件 */
	LinkedListNode node;		/**< 在链表中的结点 */
	LCUI_BOOL is_valid;		/**< 是否有效 */
} TouchPointBindingRec, *TouchPointBinding;

/** 触点绑定记录列表 */
static LinkedList touch_bindings;

static void OnTouchWidget( LCUI_Widget w, LCUI_WidgetEvent e, void *arg )
{
	LCUI_TouchPoint point;
	TouchPointBinding binding;
	if( e->touch.n_points == 0 ) {
		return;
	}
	binding = e->data;
	point = & e->touch.points[0];
	switch( point->state ) {
	case WET_TOUCHMOVE:
		Widget_Move( binding->widget, point->x - 32, point->y - 32 );
		break;
	case WET_TOUCHUP:
		if( !binding->is_valid ) {
			break;
		}
		/* 当触点释放后销毁部件及绑定记录 */
		Widget_ReleaseTouchCapture( binding->widget, -1 );
		LinkedList_Unlink( &touch_bindings, &binding->node );
		binding->is_valid = FALSE;
		Widget_Destroy( w );
		free( binding );
		break;
	case WET_TOUCHDOWN:
	default: break;
	}
}

static void OnTouch( LCUI_SysEvent e, void *arg )
{
	int i;
	LinkedListNode *node;
	LCUI_StyleSheet sheet;
	LCUI_TouchPoint point;
	for( i = 0; i < e->touch.n_points; ++i ) {
		TouchPointBinding binding;
		LCUI_BOOL is_existed = FALSE;
		point = &e->touch.points[i];
		/* 检查该触点是否已经被绑定 */
		LinkedList_ForEach( node, &touch_bindings ) {
			binding = node->data;
			if( binding->point_id == point->id ) {
				is_existed = TRUE;
			}
		}
		if( is_existed ) {
			continue;
		}
		/* 新建绑定记录 */
		binding = NEW( TouchPointBindingRec, 1 );
		binding->widget = LCUIWidget_New( NULL );
		binding->point_id = point->id;
		binding->is_valid = TRUE;
		binding->node.data = binding;
		Widget_Resize( binding->widget, 64, 64 );
		Widget_Move( binding->widget, point->x - 32, point->y - 32 );
		/* 设置让该部件捕获当前触点 */
		Widget_SetTouchCapture( binding->widget, binding->point_id );
		Widget_BindEvent( binding->widget, "touch", OnTouchWidget, binding, NULL );
		sheet = binding->widget->custom_style;
		SetStyle( sheet, key_position, SV_ABSOLUTE,  style );
		SetStyle( sheet, key_background_color, RGB( 255, 0, 0 ), color );
		LinkedList_AppendNode( &touch_bindings, &binding->node );
		Widget_Top( binding->widget );
	}
}

int main( int argc, char **argv )
{
	LCUI_Init();
	LinkedList_Init( &touch_bindings );
	LCUI_BindEvent( LCUI_TOUCH, OnTouch, NULL, NULL );
	return LCUI_Main();
}

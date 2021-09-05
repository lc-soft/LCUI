/** test_touch.c -- test touch support */

#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/display.h>
#include <LCUI/ui.h>

/** 触点绑定记录 */
typedef struct TouchPointBindingRec_ {
	int point_id;        /**< 触点 ID */
	ui_widget_t* widget;  /**< 部件 */
	LinkedListNode node; /**< 在链表中的结点 */
	LCUI_BOOL is_valid;  /**< 是否有效 */
} TouchPointBindingRec, *TouchPointBinding;

/** 触点绑定记录列表 */
static LinkedList touch_bindings;

static void OnTouchWidget(ui_widget_t* w, ui_event_t* e, void *arg)
{
	LCUI_TouchPoint point;
	TouchPointBinding binding;

	if (e->touch.n_points == 0) {
		return;
	}
	binding = e->data;
	point = &e->touch.points[0];
	switch (point->state) {
	case UI_EVENT_TOUCHMOVE:
		ui_widget_move(w, point->x - 32.0f, point->y - 32.0f);
		break;
	case UI_EVENT_TOUCHUP:
		if (!binding->is_valid) {
			break;
		}
		/* 当触点释放后销毁部件及绑定记录 */
		ui_widget_release_touch_capture(w, -1);
		LinkedList_Unlink(&touch_bindings, &binding->node);
		binding->is_valid = FALSE;
		ui_widget_remove(w);
		free(binding);
		break;
	case UI_EVENT_TOUCHDOWN:
	default:
		break;
	}
}

static void OnTouch(LCUI_SysEvent e, void *arg)
{
	int i;
	ui_widget_t* w;
	LinkedListNode *node;
	LCUI_TouchPoint point;
	LCUI_Color bgcolor = RGB(255, 0, 0);

	for (i = 0; i < e->touch.n_points; ++i) {
		TouchPointBinding binding;
		LCUI_BOOL is_existed = FALSE;
		point = &e->touch.points[i];
		_DEBUG_MSG("point: %d\n", point->id);
		/* 检查该触点是否已经被绑定 */
		for (LinkedList_Each(node, &touch_bindings)) {
			binding = node->data;
			if (binding->point_id == point->id) {
				is_existed = TRUE;
			}
		}
		if (is_existed) {
			continue;
		}
		w = ui_create_widget(NULL);
		/* 新建绑定记录 */
		binding = NEW(TouchPointBindingRec, 1);
		binding->point_id = point->id;
		binding->node.data = binding;
		binding->is_valid = TRUE;
		binding->widget = w;
		ui_widget_resize(w, 64, 64);
		ui_widget_move(w, point->x - 32.0f, point->y - 32.0f);
		/* 设置让该部件捕获当前触点 */
		ui_widget_set_touch_capture(w, binding->point_id);
		ui_widget_on(w, "touch", OnTouchWidget, binding, NULL);
		ui_widget_set_style(w, key_position, SV_ABSOLUTE, style);
		ui_widget_set_style(w, key_background_color, bgcolor, color);
		LinkedList_AppendNode(&touch_bindings, &binding->node);
		ui_root_append(w);
	}
}

int main(int argc, char **argv)
{
	LCUI_Init();
	LinkedList_Init(&touch_bindings);
	LCUI_BindEvent(LCUI_TOUCH, OnTouch, NULL, NULL);
	return LCUI_Main();
}

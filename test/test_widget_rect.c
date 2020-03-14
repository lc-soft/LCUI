#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include "libtest.h"

void test_widget_rect(void)
{
	LCUI_Widget root;
	LCUI_Widget parent, child;
	LCUI_SysEventRec ev;
	LCUI_Rect *rect;
	LCUI_Rect expected_rect;
	LinkedList rects;

	LCUI_Init();
	root = LCUIWidget_GetRoot();
	parent = LCUIWidget_New("button");
	child = LCUIWidget_New("textview");

	Widget_SetStyle(parent, key_box_sizing, SV_BORDER_BOX, style);
	Widget_Resize(root, 200, 200);
	Widget_Resize(parent, 100, 100);
	Widget_Resize(child, 50, 50);
	Widget_Append(parent, child);
	Widget_Append(root, parent);
	LCUIWidget_Update();

	LinkedList_Init(&rects);
	Widget_GetInvalidArea(root, &rects);
	LinkedList_Clear(&rects, free);

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 150, y: 150}), "
	     "root.getInvalidArea().length == 0",
	     rects.length == 0, TRUE);

	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	rect = rects.head.next->data;
	it_b("app.trigger({ type: 'mousemove', x: 80, y: 80 }), "
	     "root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);

	expected_rect.x = 0;
	expected_rect.y = 0;
	expected_rect.width = 100;
	expected_rect.height = 100;
	it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	LinkedList_Clear(&rects, free);

	ev.motion.x = 40;
	ev.motion.y = 40;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 40, y: 40 }), "
	     "root.getInvalidArea().length == 0",
	     rects.length == 0, TRUE);

	ev.type = LCUI_MOUSEDOWN;
	ev.button.x = 40;
	ev.button.y = 40;
	ev.button.button = LCUI_KEY_LEFTBUTTON;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("app.trigger({ type: 'mousedown', x: 40, y: 40 }), "
	     "root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = rects.head.next->data;
		it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	}
	LinkedList_Clear(&rects, free);

	ev.type = LCUI_MOUSEUP;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("app.trigger({ type: 'mouseup', x: 40, y: 40 }), "
	     "root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = rects.head.next->data;
		it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	}
	LinkedList_Clear(&rects, free);

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 80, y: 80 }), "
	     "root.getInvalidArea().length == 0",
	     rects.length == 0, TRUE);

	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);

	it_b("app.trigger({ type: 'mousemove', x: 150, y: 150 }), "
	     "root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = rects.head.next->data;
		it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	}
	LinkedList_Clear(&rects, free);

	expected_rect.x = 21;
	expected_rect.y = 11;
	expected_rect.width = 50;
	expected_rect.height = 50;
	Widget_Destroy(child);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("child.destroy(), root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = rects.head.next->data;
		it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	}
	LinkedList_Clear(&rects, free);

	expected_rect.x = 0;
	expected_rect.y = 0;
	expected_rect.width = 100;
	expected_rect.height = 100;
	Widget_Destroy(parent);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	it_b("parent.destroy(), root.getInvalidArea().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = rects.head.next->data;
		it_rect("root.getInvalidArea()[0]", rect, &expected_rect);
	}
	LinkedList_Clear(&rects, free);

	LCUI_Destroy();
}

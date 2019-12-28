#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include "test.h"

int test_widget_rect(void)
{
	int ret = 0;
	LCUI_Widget root;
	LCUI_Widget parent, child;
	LCUI_SysEventRec ev;
	LCUI_Rect *rect;
	LinkedList rects;

	LCUI_Init();
	root = LCUIWidget_GetRoot();
	parent = LCUIWidget_New("button");
	child = LCUIWidget_New("textview");

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
	CHECK_WITH_TEXT("moving the mouse should not create dirty rectangle",
			rects.length == 0);

	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	rect = rects.head.next->data;
	CHECK_WITH_TEXT("moving the mouse over the button should create a "
			"dirty rectangle: (0, 0, 144, 124)",
			rects.length == 1 && rect->x == 0 && rect->y == 0 &&
			    rect->width == 144 && rect->height == 124);
	LinkedList_Clear(&rects, free);

	ev.motion.x = 40;
	ev.motion.y = 40;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	CHECK_WITH_TEXT("moving the mouse over the textview should not create "
			"dirty rectangle",
			rects.length == 0);

	ev.type = LCUI_MOUSEDOWN;
	ev.button.x = 40;
	ev.button.y = 40;
	ev.button.button = LCUI_KEY_LEFTBUTTON;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	rect = rects.head.next->data;
	CHECK_WITH_TEXT("pressing the mouse button should create a dirty "
			"rectangle: (0, 0, 144, 124)",
			rects.length == 1 && rect->x == 0 && rect->y == 0 &&
			    rect->width == 144 && rect->height == 124);
	LinkedList_Clear(&rects, free);

	ev.type = LCUI_MOUSEUP;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	rect = rects.head.next->data;
	CHECK_WITH_TEXT("releasing the mouse button should create a dirty "
			"rectangle: (0, 0, 144, 124)",
			rects.length == 1 && rect->x == 0 && rect->y == 0 &&
			    rect->width == 144 && rect->height == 124);
	LinkedList_Clear(&rects, free);

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	CHECK_WITH_TEXT("moving the mouse over the button should not create "
			"dirty rectangle",
			rects.length == 0);

	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	LCUIWidget_Update();
	Widget_GetInvalidArea(root, &rects);
	rect = rects.head.next->data;
	CHECK_WITH_TEXT("moving the mouse outside the button should create "
			"a dirty rectangle",
			rects.length == 1 && rect->x == 0 && rect->y == 0 &&
			    rect->width == 144 && rect->height == 124);
	LinkedList_Clear(&rects, free);

	LCUI_Destroy();
	return ret;
}

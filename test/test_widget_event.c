#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include "test.h"

int test_widget_mouse_event(void)
{
	int ret = 0;
	LCUI_Widget root;
	LCUI_Widget parent, child;
	LCUI_SysEventRec ev;

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

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousemove(150, 150): root.hasStatus('hover') == true",
			Widget_HasStatus(root, "hover"));
	CHECK_WITH_TEXT("mousemove(150, 150): parent.hasStatus('hover') == false",
			!Widget_HasStatus(parent, "hover"));
	CHECK_WITH_TEXT("mousemove(150, 150): child.hasStatus('hover') == false",
			!Widget_HasStatus(child, "hover"));

	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousemove(80, 80): root.hasStatus('hover') == true",
			Widget_HasStatus(root, "hover"));
	CHECK_WITH_TEXT("mousemove(80, 80): parent.hasStatus('hover') == true",
			Widget_HasStatus(parent, "hover"));
	CHECK_WITH_TEXT("mousemove(80, 80): child.hasStatus('hover') == false",
			!Widget_HasStatus(child, "hover"));

	ev.motion.x = 40;
	ev.motion.y = 40;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousemove(40, 40): root.hasStatus('hover') == true",
			Widget_HasStatus(root, "hover"));
	CHECK_WITH_TEXT("mousemove(40, 40): parent.hasStatus('hover') == true",
			Widget_HasStatus(parent, "hover"));
	CHECK_WITH_TEXT("mousemove(40, 40): child.hasStatus('hover') == true",
			Widget_HasStatus(child, "hover"));

	ev.type = LCUI_MOUSEDOWN;
	ev.button.x = 40;
	ev.button.y = 40;
	ev.button.button = LCUI_KEY_LEFTBUTTON;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousedown(40, 40): root.hasStatus('active') == true",
			Widget_HasStatus(root, "active"));
	CHECK_WITH_TEXT("mousedown(40, 40): parent.hasStatus('active') == true",
			Widget_HasStatus(parent, "active"));
	CHECK_WITH_TEXT("mousedown(40, 40): child.hasStatus('active') == true",
			Widget_HasStatus(child, "active"));

	ev.type = LCUI_MOUSEUP;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mouseup(40, 40): root.hasStatus('active') == false",
			!Widget_HasStatus(root, "active"));
	CHECK_WITH_TEXT("mouseup(40, 40): parent.hasStatus('active') == false",
			!Widget_HasStatus(parent, "active"));
	CHECK_WITH_TEXT("mouseup(40, 40): child.hasStatus('active') == false",
			!Widget_HasStatus(child, "active"));

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousemove(80, 80): root.hasStatus('hover') == true",
			Widget_HasStatus(root, "hover"));
	CHECK_WITH_TEXT("mousemove(80, 80): parent.hasStatus('hover') == true",
			Widget_HasStatus(parent, "hover"));
	CHECK_WITH_TEXT("mousemove(80, 80): child.hasStatus('hover') == false",
			!Widget_HasStatus(child, "hover"));

	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	CHECK_WITH_TEXT("mousemove(150, 150): root.hasStatus('hover') == true",
			Widget_HasStatus(root, "hover"));
	CHECK_WITH_TEXT("mousemove(150, 150): parent.hasStatus('hover') == false",
			!Widget_HasStatus(parent, "hover"));
	CHECK_WITH_TEXT("mousemove(150, 150): child.hasStatus('hover') == false",
			!Widget_HasStatus(child, "hover"));

	LCUI_Destroy();
	return ret;
}

int test_widget_event(void)
{
	return test_widget_mouse_event();
}

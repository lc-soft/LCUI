#include <stdio.h>
#include <LCUI.h>
#include <LCUI/input.h>
#include <LCUI/gui/widget.h>
#include "test.h"

void test_widget_mouse_event(void)
{
	ui_widget_t* root;
	ui_widget_t* parent, child;
	LCUI_SysEventRec ev;

	LCUI_Init();
	root = ui_root();
	parent = ui_create_widget("button");
	child = ui_create_widget("textview");

	ui_widget_resize(root, 200, 200);
	ui_widget_resize(parent, 100, 100);
	ui_widget_resize(child, 50, 50);
	ui_widget_append(parent, child);
	ui_widget_append(root, parent);
	ui_update();

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	it_b("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), FALSE);
	it_b("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	it_b("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	it_b("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.motion.x = 40;
	ev.motion.y = 40;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousemove(40, 40): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	it_b("mousemove(40, 40): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	it_b("mousemove(40, 40): child.hasStatus('hover') == true",
	     ui_widget_has_status(child, "hover"), TRUE);

	ev.type = LCUI_MOUSEDOWN;
	ev.button.x = 40;
	ev.button.y = 40;
	ev.button.button = LCUI_KEY_LEFTBUTTON;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousedown(40, 40): root.hasStatus('active') == true",
	     ui_widget_has_status(root, "active"), TRUE);
	it_b("mousedown(40, 40): parent.hasStatus('active') == true",
	     ui_widget_has_status(parent, "active"), TRUE);
	it_b("mousedown(40, 40): child.hasStatus('active') == true",
	     ui_widget_has_status(child, "active"), TRUE);

	ev.type = LCUI_MOUSEUP;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mouseup(40, 40): root.hasStatus('active') == false",
	     ui_widget_has_status(root, "active"), FALSE);
	it_b("mouseup(40, 40): parent.hasStatus('active') == false",
	     ui_widget_has_status(parent, "active"), FALSE);
	it_b("mouseup(40, 40): child.hasStatus('active') == false",
	     ui_widget_has_status(child, "active"), FALSE);

	ev.type = LCUI_MOUSEMOVE;
	ev.motion.x = 80;
	ev.motion.y = 80;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	it_b("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	it_b("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.motion.x = 150;
	ev.motion.y = 150;
	ev.motion.xrel = 0;
	ev.motion.yrel = 0;
	LCUI_TriggerEvent(&ev, NULL);
	it_b("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	it_b("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), FALSE);
	it_b("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	LCUI_Destroy();
}

void test_widget_event(void)
{
	describe("test widget mouse event", test_widget_mouse_event);
}

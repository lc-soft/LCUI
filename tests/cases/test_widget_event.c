#include <stdio.h>
#include <LCUI.h>
#include <ui.h>
#include <ctest-custom.h>

void test_widget_mouse_event(void)
{
	ui_widget_t *root;
	ui_widget_t *parent, *child;
	ui_event_t ev;

	lcui_init();
	root = ui_root();
	parent = ui_create_widget("button");
	child = ui_create_widget("text");

	ui_widget_resize(root, 200, 200);
	ui_widget_resize(parent, 100, 100);
	ui_widget_resize(child, 50, 50);
	ui_widget_append(parent, child);
	ui_widget_append(root, parent);
	ui_update();

	ev.type = UI_EVENT_MOUSEMOVE;
	ev.mouse.x = 150;
	ev.mouse.y = 150;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	ctest_equal_bool("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), FALSE);
	ctest_equal_bool("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.mouse.x = 80;
	ev.mouse.y = 80;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	ctest_equal_bool("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	ctest_equal_bool("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.mouse.x = 40;
	ev.mouse.y = 40;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(40, 40): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	ctest_equal_bool("mousemove(40, 40): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	ctest_equal_bool("mousemove(40, 40): child.hasStatus('hover') == true",
	     ui_widget_has_status(child, "hover"), TRUE);

	ev.type = UI_EVENT_MOUSEDOWN;
	ev.mouse.x = 40;
	ev.mouse.y = 40;
	ev.mouse.button = MOUSE_BUTTON_LEFT;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousedown(40, 40): root.hasStatus('active') == true",
	     ui_widget_has_status(root, "active"), TRUE);
	ctest_equal_bool("mousedown(40, 40): parent.hasStatus('active') == true",
	     ui_widget_has_status(parent, "active"), TRUE);
	ctest_equal_bool("mousedown(40, 40): child.hasStatus('active') == true",
	     ui_widget_has_status(child, "active"), TRUE);

	ev.type = UI_EVENT_MOUSEUP;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mouseup(40, 40): root.hasStatus('active') == false",
	     ui_widget_has_status(root, "active"), FALSE);
	ctest_equal_bool("mouseup(40, 40): parent.hasStatus('active') == false",
	     ui_widget_has_status(parent, "active"), FALSE);
	ctest_equal_bool("mouseup(40, 40): child.hasStatus('active') == false",
	     ui_widget_has_status(child, "active"), FALSE);

	ev.type = UI_EVENT_MOUSEMOVE;
	ev.mouse.x = 80;
	ev.mouse.y = 80;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(80, 80): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	ctest_equal_bool("mousemove(80, 80): parent.hasStatus('hover') == true",
	     ui_widget_has_status(parent, "hover"), TRUE);
	ctest_equal_bool("mousemove(80, 80): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	ev.mouse.x = 150;
	ev.mouse.y = 150;
	ui_dispatch_event(&ev);
	ui_update();
	ctest_equal_bool("mousemove(150, 150): root.hasStatus('hover') == true",
	     ui_widget_has_status(root, "hover"), TRUE);
	ctest_equal_bool("mousemove(150, 150): parent.hasStatus('hover') == false",
	     ui_widget_has_status(parent, "hover"), FALSE);
	ctest_equal_bool("mousemove(150, 150): child.hasStatus('hover') == false",
	     ui_widget_has_status(child, "hover"), FALSE);

	lcui_quit();
	lcui_main();
}

void test_widget_event(void)
{
	ctest_describe("test widget mouse event", test_widget_mouse_event);
}

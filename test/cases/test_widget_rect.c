#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI.h>
#include <LCUI/ui.h>
#include "ctest.h"

void test_widget_rect(void)
{
	ui_widget_t *root;
	ui_widget_t *parent, *child;
	ui_event_t e = { 0 };
	pd_rect_t rect = { 0 };
	pd_rect_t expected_rect;
	list_t rects;

	lcui_init();
	root = ui_root();
	parent = ui_create_widget("button");
	child = ui_create_widget("textview");

	ui_widget_set_style(parent, key_box_sizing, SV_BORDER_BOX, style);
	ui_widget_resize(root, 200, 200);
	ui_widget_resize(parent, 100, 100);
	ui_widget_resize(child, 50, 50);
	ui_widget_append(parent, child);
	ui_widget_append(root, parent);
	ui_update();

	list_create(&rects);
	ui_widget_get_dirty_rects(root, &rects);
	list_destroy(&rects, free);

	e.type = UI_EVENT_MOUSEMOVE;
	e.mouse.x = 150;
	e.mouse.y = 150;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 150, y: 150}), "
	     "root.getDirtyRects().length == 0",
	     rects.length == 0, TRUE);

	e.mouse.x = 80;
	e.mouse.y = 80;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 80, y: 80 }), "
	     "root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);

	expected_rect.x = 0;
	expected_rect.y = 0;
	expected_rect.width = 100;
	expected_rect.height = 100;
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	e.mouse.x = 40;
	e.mouse.y = 40;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 40, y: 40 }), "
	     "root.getDirtyRects().length == 0",
	     rects.length == 0, TRUE);

	e.type = UI_EVENT_MOUSEDOWN;
	e.mouse.x = 40;
	e.mouse.y = 40;
	e.mouse.button = MOUSE_BUTTON_LEFT;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mousedown', x: 40, y: 40 }), "
	     "root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	e.type = UI_EVENT_MOUSEUP;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mouseup', x: 40, y: 40 }), "
	     "root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	e.type = UI_EVENT_MOUSEMOVE;
	e.mouse.x = 80;
	e.mouse.y = 80;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("app.trigger({ type: 'mousemove', x: 80, y: 80 }), "
	     "root.getDirtyRects().length == 0",
	     rects.length == 0, TRUE);

	e.mouse.x = 150;
	e.mouse.y = 150;
	ui_dispatch_event(&e);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);

	it_b("app.trigger({ type: 'mousemove', x: 150, y: 150 }), "
	     "root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	expected_rect.x = 21;
	expected_rect.y = 11;
	expected_rect.width = 50;
	expected_rect.height = 50;
	ui_widget_remove(child);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("child.destroy(), root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	expected_rect.x = 0;
	expected_rect.y = 0;
	expected_rect.width = 100;
	expected_rect.height = 100;
	ui_widget_remove(parent);
	ui_update();
	ui_widget_get_dirty_rects(root, &rects);
	it_b("parent.destroy(), root.getDirtyRects().length == 1",
	     rects.length == 1, TRUE);
	if (rects.length == 1) {
		rect = *(pd_rect_t *)list_get(&rects, 0);
	}
	it_rect("root.getDirtyRects()[0]", &rect, &expected_rect);
	list_destroy(&rects, free);

	lcui_destroy();
}

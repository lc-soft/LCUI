#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include "test.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define WIDGET_WIDTH 100
#define WIDGET_HEIGHT 100
#define DELTA 3.1415f

static struct {
	int step;
	int count;
	int pass;
	float x, y;
	LCUI_Widget widget;
} self = { 0, 0, 0, 0, 0, NULL };

static void test_move_widget(void *ar1, void *arg2)
{
	float scale, x, y;
	float screen_w, screen_h;
	scale = LCUIMetrics_GetScale();
	screen_w = SCREEN_WIDTH / scale;
	screen_h = SCREEN_HEIGHT / scale;
	switch (self.step) {
	case 0:
		x = self.x + DELTA;
		y = self.y + DELTA;
		if (x + WIDGET_WIDTH >= screen_w ||
		    y + WIDGET_HEIGHT >= screen_h) {
			x = screen_w - WIDGET_WIDTH;
			y = screen_h - WIDGET_HEIGHT;
			self.step += 1;
		}
		break;
	case 1:
		x = self.x;
		y = self.y - DELTA;
		if (y <= 0) {
			y = 0;
			self.step += 1;
		}
		break;
	case 2:
		x = self.x - DELTA;
		y = self.y;
		if (x <= 0) {
			x = 0;
			self.step += 1;
		}
		break;
	case 3:
		x = self.x;
		y = self.y + DELTA;
		if (y + WIDGET_HEIGHT >= screen_h) {
			y = screen_h - WIDGET_HEIGHT;
			self.step += 1;
		}
		break;
	case 4:
		x = self.x + DELTA;
		y = self.y - DELTA;
		if (x + WIDGET_WIDTH >= screen_w || y <= 0) {
			x = screen_w - WIDGET_WIDTH;
			y = 0;
			self.step += 1;
		}
		break;
	default:
		LCUI_Exit(self.pass - self.count);
		return;
	}
	Widget_Move(self.widget, x, y);
}

static int check_rect_correct(LCUI_Rect *corret, LCUI_Rect *actual)
{
	LCUI_Rect rect;
	if (LCUIRect_GetOverlayRect(corret, actual, &rect)) {
		if (rect.x == corret->x && rect.y == corret->y &&
		    rect.width == corret->width &&
		    rect.height == corret->height) {
			return 1;
		}
	}
	return 0;
}

static void check_widget_rect(LCUI_SysEvent ev, void *arg)
{
	int ret = 0;
	LCUI_RectF rectf;
	LCUI_Rect rect, old_rect;
	LCUI_Rect *paint_rect = &ev->paint.rect;

	rectf.x = self.x;
	rectf.y = self.y;
	rectf.width = WIDGET_WIDTH;
	rectf.height = WIDGET_WIDTH;
	LCUIMetrics_ComputeRectActual(&old_rect, &rectf);
	LCUIMetrics_ComputeRectActual(&rect, &self.widget->box.canvas);
	LCUIRect_MergeRect(&rect, &rect, &old_rect);
	CHECK2(check_rect_correct(&rect, paint_rect));
	if (ret != 0) {
		TEST_LOG("[%d] correct: (%d, %d, %d, %d),"
			 " actual: (%d, %d, %d, %d)\n",
			 self.step, rect.x, rect.y, rect.width, rect.height,
			 paint_rect->x, paint_rect->y, paint_rect->width,
			 paint_rect->height);
	} else {
		self.pass += 1;
	}
	self.count += 1;
	self.x = self.widget->x;
	self.y = self.widget->y;
	LCUI_PostSimpleTask(test_move_widget, NULL, NULL);
}

static void start_test(void *arg)
{
	test_move_widget(NULL, NULL);
	LCUI_BindEvent(LCUI_PAINT, check_widget_rect, NULL, NULL);
}

static LCUI_Widget create_widget(void)
{
	LCUI_Widget w, root;
	w = LCUIWidget_New(NULL);
	root = LCUIWidget_GetRoot();
	Widget_Append(root, w);
	Widget_Resize(w, WIDGET_WIDTH, WIDGET_HEIGHT);
	Widget_SetPosition(w, SV_ABSOLUTE);
	Widget_SetBoxSizing(w, SV_BORDER_BOX);
	Widget_SetBorder(w, 1, SV_SOLID, RGB(100, 100, 100));
	Widget_SetStyle(w, key_background_color, RGB(200, 200, 200), color);
	return w;
}

int test_widget_rect(void)
{
	int i, ret = 0;
	float values[3] = { 1.0f, 1.5f, 2.0f };

	for (i = 0; i < 3; ++i) {
		memset(&self, 0, sizeof(self));

		TEST_LOG("test widget rectagle in "
			 "%g%% scaling mode\n",
			 values[i] * 100);

		LCUI_Init();
		self.widget = create_widget();
		LCUIMetrics_SetScale(values[i]);
		LCUIDisplay_SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		/* 等一段时间后再开始测试，避免初始化 LCUI
		 * 时产生的脏矩形影响测试结果 */
		LCUITimer_Set(100, start_test, NULL, FALSE);
		LCUIWidget_Update();
		if (LCUI_Main() != 0) {
			ret -= 1;
		}
	}
	return ret;
}

#ifdef PREVIEW_MODE

int tests_count = 0;

int main(int argc, char **argv)
{
	int ret = test_widget_rect();
	PRINT_TEST_RESULT(self.count);
	return ret;
}

#endif

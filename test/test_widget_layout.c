#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/display.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/builder.h>
#include "test.h"

#define SelectWidget(name, id) LCUI_Widget name = LCUIWidget_GetById(id)

static int check_sidebar(void)
{
	int ret = 0;
	SelectWidget(sidebar, "sidebar");
	CHECK(sidebar->computed_style.z_index == 100);
	return 0;
}

static int check_body(void)
{
	int ret = 0;
	SelectWidget(body, "body");
	CHECK(body->box.border.x == 100);
	CHECK(body->box.border.width == 860);
	return ret;
}

static int check_navbar(void)
{
	int ret = 0;
	SelectWidget(navbar, "navbar");
	CHECK(navbar->box.border.width == 860);
	return 0;
}

static int check_navbar_btn(void)
{
	int ret = 0;
	SelectWidget(btn, "navbar-btn-1");
	CHECK(btn->height == 40);
	CHECK(btn->width < 108 && btn->width > 60);
	return ret;
}

static int check_content(void)
{
	int ret = 0;
	SelectWidget(content, "content");
	CHECK(content->width == 860);
	CHECK(content->height == 630);
	return ret;
}

static int check_alert(void)
{
	int ret = 0;
	SelectWidget(alert, "alert");
	SelectWidget(text, "alert-text");
	CHECK(alert->height == 84);
	CHECK(text->height == 18);
	return ret;
}

static int check_picture(void)
{
	int ret = 0;
	LCUI_BackgroundStyle *background;
	SelectWidget(picture, "picture");
	background = &picture->computed_style.background;
	CHECK(background->position.using_value);
	CHECK(background->position.value == SV_TOP_CENTER);
	CHECK(background->size.using_value);
	CHECK(background->size.value == SV_COVER);
	CHECK(picture->width == 128);
	CHECK(picture->height == 128);
	return ret;
}

static int check_offset_box(void)
{
	int ret = 0;
	LCUI_Widget prevbox;
	SelectWidget(box, "offset-box");
	prevbox = Widget_GetPrev(box);
	CHECK(box->computed_style.position == SV_RELATIVE);
	CHECK(box->x == 10);
	CHECK(box->y == prevbox->y + prevbox->height - 20);
	return ret;
}

static int check_overflow_box(void)
{
	int ret = 0;

	SelectWidget(box1, "overflow-box-1");
	SelectWidget(box2, "overflow-box-2");
	SelectWidget(box3, "overflow-box-3");
	CHECK(box1->x == 10);
	CHECK(box1->width == 820 + 20);
	CHECK(Widget_ComputeMaxAvaliableWidth(box1) == 820);
	CHECK(Widget_ComputeMaxAvaliableWidth(box2) ==
	      820 - (15 * 2 + 2) + 10 * 2);
	CHECK(Widget_ComputeMaxAvaliableWidth(box3) ==
	      820 - (15 * 2 + 2) * 2 + 10 * 2 * 2);
	return ret;
}

static int check_layout(void)
{
	int ret = 0;
	ret += check_body();
	ret += check_sidebar();
	ret += check_navbar();
	ret += check_navbar_btn();
	ret += check_alert();
	ret += check_content();
	ret += check_picture();
	ret += check_overflow_box();
	ret += check_offset_box();
	return ret;
}

#ifdef _WIN32
static void LoggerHandler(const char *str)
{
	OutputDebugStringA(str);
}

static void LoggerHandlerW(const wchar_t *str)
{
	OutputDebugStringW(str);
}
#endif

#ifdef PREVIEW_MODE
static void run_test(void *arg1, void *arg2)
{
	PRINT_TEST_RESULT(check_layout());
}
#endif

int test_widget_layout(void)
{
	int ret = 0;
	LCUI_Widget root, pack;

	LCUI_Init();
#ifdef _WIN32
	Logger_SetHandler(LoggerHandler);
	Logger_SetHandlerW(LoggerHandlerW);
#endif
	TEST_LOG("test widget layout\n");
	LCUIDisplay_SetSize(960, 680);
	root = LCUIWidget_GetRoot();
	CHECK(pack = LCUIBuilder_LoadFile("test_widget_layout.xml"));
	if (!pack) {
		LCUI_Destroy();
		return ret;
	}
	Widget_UpdateStyle(root, TRUE);
	Widget_Append(root, pack);
	Widget_Unwrap(pack);
	LCUIWidget_Update();
	check_overflow_box();
#ifdef PREVIEW_MODE
	LCUI_PostSimpleTask(run_test, NULL, NULL);
	return LCUI_Main();
#else
	ret += check_layout();
	LCUI_Destroy();
	return ret;
#endif
}

#ifdef PREVIEW_MODE

int tests_count = 0;

int main(int argc, char **argv)
{
	test_widget_layout();
}

#endif

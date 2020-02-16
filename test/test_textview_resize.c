#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/gui/css_parser.h>
#include "test.h"
#include "libtest.h"

/* clang-format off */

static struct {
	LCUI_Widget block;
	LCUI_Widget inline_block;
} self;

static const char *css = CodeToString(

.block {
	width: 100px;
	margin: 10px;
	padding: 10px;
	display: block;
	border: 1px solid #f00;
}

.inline-block {
	margin: 10px;
	padding: 10px;
	display: inline-block;
	border: 1px solid #f00;
}

.short-content {
	content: "hello!";
}

.long-content {
	content: "this is long long long long long long long text";
}

);

/* clang-format on */

static void build(void)
{
	LCUI_Widget root;

	LCUI_LoadCSSString(css, __FILE__);

	self.block = LCUIWidget_New("textview");
	self.inline_block = LCUIWidget_New("textview");

	Widget_SetId(self.block, "debug-block");
	Widget_SetId(self.inline_block, "debug-inline-block");
	TextView_SetTextW(self.block, L"block");
	TextView_SetTextW(self.inline_block, L"inline block");
	Widget_AddClass(self.block, "block");
	Widget_AddClass(self.inline_block, "inline-block");

	root = LCUIWidget_GetRoot();
	Widget_Append(root, self.block);
	Widget_Append(root, self.inline_block);
}

static void test_textview_set_text(void *arg)
{
	_DEBUG_MSG("set text\n");
	TextView_SetText(self.inline_block,
			 "long long long long long long long "
			 "long long long long long long long "
			 "long long long long long long long text");

	TextView_SetText(self.block, "long long long long long long long "
				     "long long long long long long long "
				     "long long long long long long long text");
}

static int check_textview_set_text(void)
{
	int ret = 0;

	LCUIWidget_Update();

	CHECK(self.block->width == 122.0f);
	CHECK(self.block->height > 140.0f);
	CHECK(self.inline_block->width > 520.0f);
	CHECK(self.inline_block->height < 45.0f);

	return ret;
}

static void test_textview_set_short_content_css(void *arg)
{
	_DEBUG_MSG("set text\n");
	Widget_AddClass(self.block, "short-content");
	Widget_AddClass(self.inline_block, "short-content");
}

static int check_textview_set_short_content_css(void)
{
	int ret = 0;

	LCUIWidget_Update();

	CHECK(self.block->width == 122.0f);
	CHECK(self.block->height < 45.0f);
	CHECK(self.inline_block->width < 70.0f);
	CHECK(self.inline_block->height < 45.0f);

	return ret;
}

static void test_textview_set_long_content_css(void *arg)
{
	_DEBUG_MSG("set text\n");
	Widget_RemoveClass(self.block, "short-content");
	Widget_RemoveClass(self.inline_block, "short-content");
	Widget_AddClass(self.block, "long-content");
	Widget_AddClass(self.inline_block, "long-content");
}

static int check_textview_set_long_content_css(void)
{
	int ret = 0;

	LCUIWidget_Update();

	CHECK(self.block->width == 122.0f);
	CHECK(self.block->height > 60.0f);
	CHECK(self.inline_block->width > 250.0f);
	CHECK(self.inline_block->height < 45.0f);
	return ret;
}

static void test_textview_set_font_size(void)
{
	Widget_SetStyleString(self.inline_block, "font-size", "24px");
	Widget_SetStyleString(self.inline_block, "line-height", "32px");
	LCUIWidget_Update();

	it_i("inlineText.height", (int)self.inline_block->height, 54);
}

static void test_textview_set_font_weight(void)
{
	float width = self.inline_block->width;

	Widget_SetStyleString(self.inline_block, "font-weight", "bold");
	LCUIWidget_Update();

	it_b("inlineText.width should be larger",
	     self.inline_block->width > width, TRUE);
}

int test_textview_resize(void)
{
	int ret = 0;

	LCUI_Init();

	build();

	test_textview_set_text(NULL);
	ret += check_textview_set_text();
	test_textview_set_short_content_css(NULL);
	ret += check_textview_set_short_content_css();
	test_textview_set_long_content_css(NULL);
	ret += check_textview_set_long_content_css();

	LCUI_Destroy();
	return ret;
}

#ifdef PREVIEW_MODE

#include <LCUI/display.h>

int tests_count = 0;

int main(void)
{
	LCUI_Init();
	LCUIDisplay_ShowRectBorder();

	build();
	//LCUI_SetTimeout(2000, (TimerCallback)test_textview_set_font_size, NULL);
	LCUI_SetTimeout(4000, (TimerCallback)test_textview_set_font_weight, NULL);
	// LCUI_SetTimeout(5000, test_textview_set_text, NULL);
	// LCUI_SetTimeout(10000, test_textview_set_short_content_css, NULL);
	// LCUI_SetTimeout(15000, test_textview_set_long_content_css, NULL);

	return LCUI_Main();
}

#endif

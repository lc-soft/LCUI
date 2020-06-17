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

static void check_textview_set_text(void)
{
	LCUIWidget_Update();

	it_b("check block width", self.block->width == 122.0f, TRUE);
	it_b("check block height", self.block->height > 140.0f, TRUE);
	it_b("check inline block width", self.inline_block->width > 520.0f,
	     TRUE);
	it_b("check inline block height", self.inline_block->height < 45.0f,
	     TRUE);
}

static void test_textview_set_short_content_css(void *arg)
{
	_DEBUG_MSG("set text\n");
	Widget_AddClass(self.block, "short-content");
	Widget_AddClass(self.inline_block, "short-content");
}

static void check_textview_set_short_content_css(void)
{
	LCUIWidget_Update();

	it_b("check block width", self.block->width == 122.0f, TRUE);
	it_b("check block height", self.block->height < 45.0f, TRUE);
	it_b("check inline block width", self.inline_block->width < 70.0f,
	     TRUE);
	it_b("check inline block height", self.inline_block->height < 45.0f,
	     TRUE);
}

static void test_textview_set_long_content_css(void *arg)
{
	_DEBUG_MSG("set text\n");
	Widget_RemoveClass(self.block, "short-content");
	Widget_RemoveClass(self.inline_block, "short-content");
	Widget_AddClass(self.block, "long-content");
	Widget_AddClass(self.inline_block, "long-content");
}

static void check_textview_set_long_content_css(void)
{
	LCUIWidget_Update();

	it_b("check block width", self.block->width == 122.0f, TRUE);
	it_b("check block height", self.block->height > 60.0f, TRUE);
	it_b("check inline block width", self.inline_block->width > 250.0f,
	     TRUE);
	it_b("check inline block height", self.inline_block->height < 45.0f,
	     TRUE);
}

void test_textview_resize(void)
{
	LCUI_Init();

	build();

	test_textview_set_text(NULL);
	describe("check textview set text", check_textview_set_text);
	test_textview_set_short_content_css(NULL);
	describe("check textview set short content css",
		 check_textview_set_short_content_css);
	test_textview_set_long_content_css(NULL);
	describe("check textview set long content css",
		 check_textview_set_long_content_css);

	LCUI_Destroy();
}

#ifdef PREVIEW_MODE

int tests_count = 0;

int main(void)
{
	LCUI_Init();

	build();
	LCUI_SetTimeout(2000, test_textview_set_text, NULL);
	LCUI_SetTimeout(4000, test_textview_set_short_content_css, NULL);
	LCUI_SetTimeout(6000, test_textview_set_long_content_css, NULL);

	return LCUI_Main();
}

#endif

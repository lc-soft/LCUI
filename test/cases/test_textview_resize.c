#include <LCUI.h>
#include <LCUI/ui/widgets/textview.h>
#include <LCUI/css.h>
#include "ctest.h"

/* clang-format off */

static struct {
	ui_widget_t* block;
	ui_widget_t* inline_block;
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
	ui_widget_t* root;

	ui_load_css_string(css, __FILE__);

	self.block = ui_create_widget("textview");
	self.inline_block = ui_create_widget("textview");

	ui_widget_set_id(self.block, "debug-block");
	ui_widget_set_id(self.inline_block, "debug-inline-block");
	ui_textview_set_text_w(self.block, L"block");
	ui_textview_set_text_w(self.inline_block, L"inline block");
	ui_widget_add_class(self.block, "block");
	ui_widget_add_class(self.inline_block, "inline-block");

	root = ui_root();
	ui_widget_append(root, self.block);
	ui_widget_append(root, self.inline_block);
}

static void test_textview_set_text(void *arg)
{
	_DEBUG_MSG("set text\n");
	ui_textview_set_text(self.inline_block,
			 "long long long long long long long "
			 "long long long long long long long "
			 "long long long long long long long text");

	ui_textview_set_text(self.block, "long long long long long long long "
				     "long long long long long long long "
				     "long long long long long long long text");
}

static void check_textview_set_text(void)
{
	ui_update();

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
	ui_widget_add_class(self.block, "short-content");
	ui_widget_add_class(self.inline_block, "short-content");
}

static void check_textview_set_short_content_css(void)
{
	ui_update();

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
	ui_widget_remove_class(self.block, "short-content");
	ui_widget_remove_class(self.inline_block, "short-content");
	ui_widget_add_class(self.block, "long-content");
	ui_widget_add_class(self.inline_block, "long-content");
}

static void check_textview_set_long_content_css(void)
{
	ui_update();

	it_b("check block width", self.block->width == 122.0f, TRUE);
	it_b("check block height", self.block->height > 60.0f, TRUE);
	it_b("check inline block width", self.inline_block->width > 250.0f,
	     TRUE);
	it_b("check inline block height", self.inline_block->height < 45.0f,
	     TRUE);
}

void test_textview_resize(void)
{
	lcui_init();

	build();

	test_textview_set_text(NULL);
	describe("check textview set text", check_textview_set_text);
	test_textview_set_short_content_css(NULL);
	describe("check textview set short content css",
		 check_textview_set_short_content_css);
	test_textview_set_long_content_css(NULL);
	describe("check textview set long content css",
		 check_textview_set_long_content_css);

	lcui_destroy();
}

#define PREVIEW_MODE
#include "./cases/test_scrollbar.c"
#include <platform/main.h>

int main(int argc, char **argv)
{
	lcui_init();
	ui_widget_resize(ui_root(), 800, 640);
	ui_load_css_string(test_css, __FILE__);
	/* We have two ways to build content view */
	BuildContentViewFromXML();
	BuildContentView();
	return lcui_main();
}

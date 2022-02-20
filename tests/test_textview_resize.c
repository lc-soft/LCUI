#define PREVIEW_MODE
#include "./cases/test_textview_resize.c"
#include <LCUI/main.h>

int main(int argc, char *argv[])
{
	lcui_init();

	build();
	lcui_set_timeout(2000, test_textview_set_text, NULL);
	lcui_set_timeout(4000, test_textview_set_short_content_css, NULL);
	lcui_set_timeout(6000, test_textview_set_long_content_css, NULL);

	return lcui_main();
}

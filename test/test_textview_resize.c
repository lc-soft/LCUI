#define PREVIEW_MODE
#include "./cases/test_textview_resize.c"

int main(void)
{
	LCUI_Init();

	build();
	lcui_timer_set_timeout(2000, test_textview_set_text, NULL);
	lcui_timer_set_timeout(4000, test_textview_set_short_content_css, NULL);
	lcui_timer_set_timeout(6000, test_textview_set_long_content_css, NULL);

	return LCUI_Main();
}

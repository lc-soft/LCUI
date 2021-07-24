#define PREVIEW_MODE
#include "./cases/test_textview_resize.c"

int main(void)
{
	LCUI_Init();

	build();
	LCUI_SetTimeout(2000, test_textview_set_text, NULL);
	LCUI_SetTimeout(4000, test_textview_set_short_content_css, NULL);
	LCUI_SetTimeout(6000, test_textview_set_long_content_css, NULL);

	return LCUI_Main();
}

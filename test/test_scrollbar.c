#define PREVIEW_MODE
#include "./cases/test_scrollbar.c"

int main(int argc, char **argv)
{
	LCUI_Init();
	LCUIDisplay_SetSize(800, 640);
	LCUI_LoadCSSString(test_css, __FILE__);
	/* We have two ways to build content view */
	BuildContentViewFromXML();
	BuildContentView();
	return LCUI_Main();
}

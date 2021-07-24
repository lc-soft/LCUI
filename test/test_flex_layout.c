#define PREVIEW_MODE
#include "./cases/test_flex_layout.c"

int main(int argc, char **argv)
{
	Logger_SetLevel(LOGGER_LEVEL_INFO);
	test_flex_layout();
	return LCUI_Main();
}

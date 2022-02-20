#define PREVIEW_MODE
#include "./cases/test_flex_layout.c"
#include <LCUI/main.h>

int main(int argc, char **argv)
{
	logger_set_level(LOGGER_LEVEL_ERROR);
	test_flex_layout();
	return lcui_main();
}

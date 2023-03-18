#define PREVIEW_MODE
#include "./cases/test_block_layout.c"
#include <platform/main.h>

int main(int argc, char **argv)
{
	logger_set_level(LOGGER_LEVEL_ERROR);
	test_block_layout();
	return lcui_main();
}

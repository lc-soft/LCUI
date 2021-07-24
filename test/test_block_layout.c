#define PREVIEW_MODE
#include "./cases/test_block_layout.c"

int main(int argc, char **argv)
{
	Logger_SetLevel(LOGGER_LEVEL_INFO);
	LCUI_Init();
	test_block_layout();
	return LCUI_Main();
}

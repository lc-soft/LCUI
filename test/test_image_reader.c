#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include "test.h"

int test_image_reader(void)
{
	LCUI_Graph img;
	int ret = 0, i, width, height;
	char file[256], *formats[] = { "png", "bmp", "jpg" };

	for (i = 0; i < 3; ++i) {
		width = height = 0;
		Graph_Init(&img);
		snprintf(file, 255, "test_image_reader.%s", formats[i]);
		TEST_LOG("image file: %s\n", file);
		CHECK(LCUI_ReadImageFile(file, &img) == 0);
		CHECK(img.width == 91 && img.height == 69);
		CHECK(LCUI_GetImageSize(file, &width, &height) == 0);
		TEST_LOG("image size: (%d, %d)\n", width, height);
		CHECK(width == 91 && height == 69);
		Graph_Free(&img);
	}
	return ret;
}

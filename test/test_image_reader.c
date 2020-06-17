#include <stdio.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/util/logger.h>
#include "test.h"
#include "libtest.h"

void test_image_reader(void)
{
	LCUI_Graph img;
	int i, width, height;
	char file[256], *formats[] = { "png", "bmp", "jpg" };

	for (i = 0; i < 3; ++i) {
		width = height = 0;
		Graph_Init(&img);
		snprintf(file, 255, "test_image_reader.%s", formats[i]);
		Logger_Debug("image file: %s\n", file);
		it_i("check LCUI_ReadImageFile", LCUI_ReadImageFile(file, &img),
		     0);
		it_i("check image width with ReadImageFile", img.width, 91);
		it_i("check image height with ReadImageFile", img.height, 69);
		it_i("check LCUI_GetImageSize",
		     LCUI_GetImageSize(file, &width, &height), 0);
		Logger_Debug("image size: (%d, %d)\n", width, height);
		it_i("check image width with GetImageSize", width, 91);
		it_i("check image height with GetImageSize", height, 69);
		Graph_Free(&img);
	}
}

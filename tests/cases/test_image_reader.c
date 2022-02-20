#include <stdio.h>
#include <LCUI.h>
#include <LCUI/pandagl.h>
#include <LCUI/image.h>
#include <LCUI/util.h>
#include "ctest.h"

void test_image_reader(void)
{
	pd_canvas_t img;
	int i, width, height;
	char file[256], *formats[] = { "png", "bmp", "jpg" };

	for (i = 0; i < 3; ++i) {
		width = height = 0;
		pd_canvas_init(&img);
		snprintf(file, 255, "test_image_reader.%s", formats[i]);
		logger_debug("image file: %s\n", file);
		it_i("check LCUI_ReadImageFile", LCUI_ReadImageFile(file, &img),
		     0);
		it_i("check image width with ReadImageFile", img.width, 91);
		it_i("check image height with ReadImageFile", img.height, 69);
		it_i("check LCUI_GetImageSize",
		     LCUI_GetImageSize(file, &width, &height), 0);
		logger_debug("image size: (%d, %d)\n", width, height);
		it_i("check image width with GetImageSize", width, 91);
		it_i("check image height with GetImageSize", height, 69);
		pd_canvas_destroy(&img);
	}
}

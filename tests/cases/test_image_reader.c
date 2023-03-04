#include <stdio.h>
#include <LCUI.h>
#include <ctest-custom.h>

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
		ctest_euqal_int("check pd_read_image_from_file", pd_read_image_from_file(file, &img),
		     0);
		ctest_euqal_int("check image width with ReadImageFile", img.width, 91);
		ctest_euqal_int("check image height with ReadImageFile", img.height, 69);
		ctest_euqal_int("check pd_read_image_size_from_file",
		     pd_read_image_size_from_file(file, &width, &height), 0);
		logger_debug("image size: (%d, %d)\n", width, height);
		ctest_euqal_int("check image width with GetImageSize", width, 91);
		ctest_euqal_int("check image height with GetImageSize", height, 69);
		pd_canvas_destroy(&img);
	}
}

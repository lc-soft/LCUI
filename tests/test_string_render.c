#include <pandagl.h>

int main(void)
{
	int ret;
	pd_canvas_t img;
	pd_pos_t pos = { 0, 80 };
	pd_rect_t area = { 0, 0, 320, 240 };
	pd_text_t *text = pd_text_create();
	pd_text_style_t style;

	/* 初始化字体处理功能 */
	pd_font_library_init();

	/* 创建一个图像，并使用灰色填充 */
	pd_canvas_init(&img);
	pd_canvas_create(&img, 320, 240);
	pd_canvas_fill(&img, pd_rgb(240, 240, 240));

	/* 设置文本的字体大小 */
	pd_text_style_init(&style);
	style.pixel_size = 24;
	style.has_pixel_size = TRUE;

	/* 设置文本图层的固定尺寸、文本样式、文本内容、对齐方式 */
	pd_text_set_fixed_size(text, 320, 240);
	pd_text_set_style(text, &style);
	pd_text_set_align(text, PD_TEXT_ALIGN_CENTER);
	pd_text_write(text, L"这是一段测试文本\nHello, World!", NULL);
	pd_text_update(text, NULL);

	/* 将文本图层绘制到图像中，然后将图像写入至 png 文件中 */
	pd_text_render_to(text, area, pos, &img);
	ret = pd_write_png_file("test_string_render.png", &img);
	pd_canvas_destroy(&img);

	/* 释放字体处理功能相关资源 */
	pd_font_library_destroy();
	return ret;
}

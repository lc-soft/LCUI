#include <LCUI.h>
#include <pandagl.h>

int main(void)
{
	int ret, fid;
	pd_canvas_t img;
	pd_font_bitmap_t bmp;
	pd_pos_t pos = { 25, 25 };
	pd_color_t bg = RGB(240, 240, 240);
	pd_color_t color = RGB(255, 0, 0);

	/* 初始化字体处理功能 */
	pd_font_library_init();

	/* 创建一个画布，并填充背景为灰色 */
	pd_canvas_init(&img);
	pd_canvas_create(&img, 100, 100);
	pd_canvas_fill(&img, bg);

	/* 载入字体文件 */
	ret = pd_font_library_load_file("C:/Windows/fonts/simsun.ttc");
	while (ret == 0) {
		/* 获取字体ID */
		fid = pd_font_library_get_font_id("SimSun", 0, 0);
		if (fid < 0) {
			break;
		}
		/* 渲染对应的文字位图，大小为 48 像素 */
		ret = pd_font_library_render_bitmap(&bmp, L'字', fid, 48);
		if (ret != 0) {
			break;
		}
		/* 绘制红色文字到图像上 */
		pd_canvas_mix_font_bitmap(&img, pos, &bmp, color);
		pd_write_png_file("test_char_render.png", &img);
		/* 释放内存资源 */
		pd_font_bitmap_destroy(&bmp);
		pd_canvas_destroy(&img);
		break;
	}

	/* 释放字体处理功能相关资源 */
	pd_font_library_destroy();
	return ret;
}

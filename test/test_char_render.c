#include <LCUI.h>
#include <LCUI/image.h>
#include <LCUI/font.h>

int main(void)
{
	int ret, fid;
	pd_canvas_t img;
	LCUI_FontBitmap bmp;
	pd_pos_t pos = { 25, 25 };
	pd_color_t bg = RGB(240, 240, 240);
	pd_color_t color = RGB(255, 0, 0);

	/* 初始化字体处理功能 */
	LCUI_InitFontLibrary();

	/* 创建一个画布，并填充背景为灰色 */
	pd_canvas_init(&img);
	pd_canvas_create(&img, 100, 100);
	pd_canvas_fill_rect(&img, bg, NULL, FALSE);

	/* 载入字体文件 */
	ret = LCUIFont_LoadFile("C:/Windows/fonts/simsun.ttc");
	while (ret == 0) {
		/* 获取字体ID */
		fid = LCUIFont_GetId("SimSun", 0, 0);
		if (fid < 0) {
			break;
		}
		/* 渲染对应的文字位图，大小为 48 像素 */
		ret = LCUIFont_RenderBitmap(&bmp, L'字', fid, 48);
		if (ret != 0) {
			break;
		}
		/* 绘制红色文字到图像上 */
		FontBitmap_Mix(&img, pos, &bmp, color);
		LCUI_WritePNGFile("test_char_render.png", &img);
		/* 释放内存资源 */
		FontBitmap_Free(&bmp);
		pd_canvas_free(&img);
		break;
	}

	/* 释放字体处理功能相关资源 */
	LCUI_FreeFontLibrary();
	return ret;
}

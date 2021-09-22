#include <LCUI.h>
#include <LCUI/image.h>
#include <LCUI/font.h>

int main(void)
{
	int ret, fid;
	LCUI_Graph img;
	LCUI_FontBitmap bmp;
	LCUI_Pos pos = { 25, 25 };
	LCUI_Color bg = RGB(240, 240, 240);
	LCUI_Color color = RGB(255, 0, 0);

	/* 初始化字体处理功能 */
	LCUI_InitFontLibrary();

	/* 创建一个画布，并填充背景为灰色 */
	Graph_Init(&img);
	Graph_Create(&img, 100, 100);
	Graph_FillRect(&img, bg, NULL, FALSE);

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
		Graph_Free(&img);
		break;
	}

	/* 释放字体处理功能相关资源 */
	LCUI_FreeFontLibrary();
	return ret;
}

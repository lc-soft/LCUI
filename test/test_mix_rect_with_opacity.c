#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int i, size;
	LCUI_Graph canvas;
	LCUI_Graph fore_canvas;
	LCUI_Rect rect;

	Graph_Init(&canvas);
	Graph_Init(&fore_canvas);
	Graph_Create(&canvas, 150, 150);
	// 画背景
	rect.x = 0;
	rect.y = 0;
	rect.width = 75;
	rect.height = 75;
	Graph_FillRect(&canvas, RGB(255, 221, 0), &rect, FALSE);
	rect.x = 75;
	Graph_FillRect(&canvas, RGB(102, 204, 0), &rect, FALSE);
	rect.x = 0;
	rect.y = 75;
	Graph_FillRect(&canvas, RGB(0, 153, 255), &rect, FALSE);
	rect.x = 75;
	Graph_FillRect(&canvas, RGB(255, 51, 0), &rect, FALSE);
	// 设置前景的 opacity 值
	fore_canvas.opacity = 0.2f;
	// 仅当色彩模式为 ARGB 时 opacity 属性才会生效
	fore_canvas.color_type = LCUI_COLOR_TYPE_ARGB;
	for (i = 0; i < 7; ++i) {
		size = 2 * (10 + 10 * i);
		// 使用新尺寸重新创建前景图
		Graph_Create(&fore_canvas, size, size);
		// 重新填充颜色
		Graph_FillRect(&fore_canvas, RGB(255, 255, 255), NULL, TRUE);
		// 将前景图混合到背景图中
		Graph_Mix(&canvas, &fore_canvas, 75 - size / 2, 75 - size / 2,
			  FALSE);
	}
	LCUI_WritePNGFile("test_mix_rect_with_opacity.png", &canvas);
	Graph_Free(&fore_canvas);
	Graph_Free(&canvas);
	return 0;
}

#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int i, size;
	pd_canvas_t canvas;
	pd_canvas_t fore_canvas;
	pd_rect_t rect;

	pd_canvas_init(&canvas);
	pd_canvas_init(&fore_canvas);
	pd_canvas_create(&canvas, 150, 150);
	// 画背景
	rect.x = 0;
	rect.y = 0;
	rect.width = 75;
	rect.height = 75;
	pd_canvas_fill_rect(&canvas, RGB(255, 221, 0), &rect, FALSE);
	rect.x = 75;
	pd_canvas_fill_rect(&canvas, RGB(102, 204, 0), &rect, FALSE);
	rect.x = 0;
	rect.y = 75;
	pd_canvas_fill_rect(&canvas, RGB(0, 153, 255), &rect, FALSE);
	rect.x = 75;
	pd_canvas_fill_rect(&canvas, RGB(255, 51, 0), &rect, FALSE);
	// 设置前景的 opacity 值
	fore_canvas.opacity = 0.2f;
	// 仅当色彩模式为 ARGB 时 opacity 属性才会生效
	fore_canvas.color_type = PD_COLOR_TYPE_ARGB;
	for (i = 0; i < 7; ++i) {
		size = 2 * (10 + 10 * i);
		// 使用新尺寸重新创建前景图
		pd_canvas_create(&fore_canvas, size, size);
		// 重新填充颜色
		pd_canvas_fill_rect(&fore_canvas, RGB(255, 255, 255), NULL, TRUE);
		// 将前景图混合到背景图中
		pd_canvas_mix(&canvas, &fore_canvas, 75 - size / 2, 75 - size / 2,
			  FALSE);
	}
	LCUI_WritePNGFile("test_mix_rect_with_opacity.png", &canvas);
	pd_canvas_free(&fore_canvas);
	pd_canvas_free(&canvas);
	return 0;
}

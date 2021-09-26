#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/painter.h>

int paint_background(pd_paint_context paint, pd_rect_t *box)
{
	pd_canvas_t image;
	pd_color_t green = RGB(102, 204, 0);
	pd_background_t bg = { 0 };

	pd_graph_init(&image);
	// 读取背景图片
	if (LCUI_ReadImageFile("test_image_reader.png", &image) != 0) {
		return -1;
	}
	// 设置背景色
	bg.color = green;
	// 设置背景图
	bg.image = &image;
	bg.size.width = image.width;
	bg.size.height = image.height;
	// 让背景图居中
	bg.position.x = (box->width - image.width) / 2;
	bg.position.y = (box->height - image.height) / 2;
	// 绘制背景
	pd_background_paint(&bg, box, paint);
	pd_graph_free(&image);
	return 0;
}

void paint_border(pd_paint_context paint, pd_rect_t *box)
{
	pd_border_t border = { 0 };
	pd_color_t black = RGB(0, 0, 0);

	border.top.color = black;
	border.top.style = SV_SOLID;
	border.top.width = 4;
	border.right.color = black;
	border.right.style = SV_SOLID;
	border.right.width = 4;
	border.bottom.color = black;
	border.bottom.style = SV_SOLID;
	border.bottom.width = 4;
	border.left.color = black;
	border.left.style = SV_SOLID;
	border.left.width = 4;
	border.top_left_radius = 32;
	border.top_right_radius = 32;
	border.bottom_left_radius = 32;
	border.bottom_right_radius = 32;
	pd_border_paint(&border, box, paint);
}

int main(void)
{
	int border_size = 4;

	pd_canvas_t canvas;
	pd_canvas_t layer;
	pd_color_t gray = RGB(240, 240, 240);
	pd_rect_t border_box = { 0, 0, 400, 300 };
	pd_rect_t bg_box = { border_box.x + border_size,
			     border_box.y + border_size,
			     border_box.width - border_size * 2,
			     border_box.height - border_size * 2 };
	pd_rect_t layer_rect = { 0, 0, border_box.width, border_box.height };
	pd_paint_context paint;

	pd_graph_init(&canvas);
	pd_graph_create(&canvas, 800, 600);
	pd_graph_fill_rect(&canvas, gray, NULL, FALSE);

	pd_graph_init(&layer);
	layer.color_type = PD_COLOR_TYPE_ARGB;
	pd_graph_create(&layer, layer_rect.width, layer_rect.height);

	// 创建绘制上下文
	paint = pd_painter_begin(&layer, &layer_rect);
	paint->with_alpha = TRUE;
	paint_background(paint, &bg_box);
	paint_border(paint, &border_box);
	pd_graph_mix(&canvas, &layer, (canvas.width - layer_rect.width) / 2,
		  (canvas.height - layer_rect.height) / 2, FALSE);
	LCUI_WritePNGFile("test_paint_border.png", &canvas);
	pd_graph_free(&canvas);
	return 0;
}

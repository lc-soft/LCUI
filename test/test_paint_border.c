#include <LCUI.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>
#include <LCUI/painter.h>

int paint_background(LCUI_PaintContext paint, LCUI_Rect *box)
{
	LCUI_Graph image;
	LCUI_Color green = RGB(102, 204, 0);
	LCUI_Background bg = { 0 };

	Graph_Init(&image);
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
	Background_Paint(&bg, box, paint);
	Graph_Free(&image);
	return 0;
}

void paint_border(LCUI_PaintContext paint, LCUI_Rect *box)
{
	LCUI_Border border = { 0 };
	LCUI_Color black = RGB(0, 0, 0);

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
	Border_Paint(&border, box, paint);
}

int main(void)
{
	int border_size = 4;

	LCUI_Graph canvas;
	LCUI_Graph layer;
	LCUI_Color gray = RGB(240, 240, 240);
	LCUI_Rect border_box = { 0, 0, 400, 300 };
	LCUI_Rect bg_box = { border_box.x + border_size,
			     border_box.y + border_size,
			     border_box.width - border_size * 2,
			     border_box.height - border_size * 2 };
	LCUI_Rect layer_rect = { 0, 0, border_box.width, border_box.height };
	LCUI_PaintContext paint;

	Graph_Init(&canvas);
	Graph_Create(&canvas, 800, 600);
	Graph_FillRect(&canvas, gray, NULL, FALSE);

	Graph_Init(&layer);
	layer.color_type = LCUI_COLOR_TYPE_ARGB;
	Graph_Create(&layer, layer_rect.width, layer_rect.height);

	// 创建绘制上下文
	paint = LCUIPainter_Begin(&layer, &layer_rect);
	paint->with_alpha = TRUE;
	paint_background(paint, &bg_box);
	paint_border(paint, &border_box);
	Graph_Mix(&canvas, &layer, (canvas.width - layer_rect.width) / 2,
		  (canvas.height - layer_rect.height) / 2, FALSE);
	LCUI_WritePNGFile("test_paint_border.png", &canvas);
	Graph_Free(&canvas);
	return 0;
}

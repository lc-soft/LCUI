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

void paint_border(LCUI_PaintContext paint, LCUI_Rect *box, int size, int radius)
{
	LCUI_Border border = { 0 };
	LCUI_Color black = RGB(0, 0, 0);

	border.top.color = black;
	border.top.style = SV_SOLID;
	border.top.width = size;
	border.right.color = black;
	border.right.style = SV_SOLID;
	border.right.width = size;
	border.bottom.color = black;
	border.bottom.style = SV_SOLID;
	border.bottom.width = size;
	border.left.color = black;
	border.left.style = SV_SOLID;
	border.left.width = size;
	border.top_left_radius = radius;
	border.top_right_radius = radius;
	border.bottom_left_radius = radius;
	border.bottom_right_radius = radius;
	Border_Paint(&border, box, paint);
}

int main(void)
{
	int border_size = 4;
	int border_radius = 32;
	int width = 800;
	int height = 600;

	LCUI_Graph canvas;
	LCUI_Graph layer;
	LCUI_Color gray = RGB(240, 240, 240);
	LCUI_BoxShadow shadow = { .x = 0,
				  .y = 0,
				  .blur = 40,
				  .spread = 0,
				  .color = ARGB(150, 0, 0, 0),
				  .top_left_radius = border_radius,
				  .top_right_radius = border_radius,
				  .bottom_left_radius = border_radius,
				  .bottom_right_radius = border_radius };
	LCUI_Rect shadow_box;
	LCUI_Rect border_box;
	LCUI_Rect bg_box;
	LCUI_Rect layer_rect;
	LCUI_PaintContext paint;

	Graph_Init(&canvas);
	Graph_Create(&canvas, width, height);
	Graph_FillRect(&canvas, gray, NULL, FALSE);

	// 设置居中的背景区域
	bg_box.width = 400;
	bg_box.height = 300;
	bg_box.x = (width - bg_box.width) / 2;
	bg_box.y = (height - bg_box.height) / 2;
	// 基于背景区域，计算边框区域
	border_box.x = bg_box.x - border_size;
	border_box.y = bg_box.y - border_size;
	border_box.width = bg_box.width + border_size * 2;
	border_box.height = bg_box.height + border_size * 2;
	// 基于边框区域，计算阴影区域
	BoxShadow_GetCanvasRect(&shadow, &border_box, &shadow_box);

	// 创建一个临时绘制层
	Graph_Init(&layer);
	layer_rect.x = 0;
	layer_rect.y = 0;
	layer_rect.width = shadow_box.width;
	layer_rect.height = shadow_box.height;
	layer.color_type = LCUI_COLOR_TYPE_ARGB;
	Graph_Create(&layer, layer_rect.width, layer_rect.height);

	// 基于临时绘制层创建绘制上下文
	paint = LCUIPainter_Begin(&layer, &layer_rect);
	paint->with_alpha = TRUE;
	// 将背景区域和边框区域的坐标转换成相对于阴影区域
	bg_box.x -= shadow_box.x;
	bg_box.y -= shadow_box.y;
	border_box.x -= shadow_box.x;
	border_box.y -= shadow_box.y;
	paint_background(paint, &bg_box);
	paint_border(paint, &border_box, border_size, border_radius);
	BoxShadow_Paint(&shadow, &layer_rect, border_box.width,
			border_box.height, paint);

	// 将临时绘制层混合到画布中
	Graph_Mix(&canvas, &layer, shadow_box.x, shadow_box.y, FALSE);
	LCUI_WritePNGFile("test_paint_boxshadow.png", &canvas);
	Graph_Free(&canvas);
	return 0;
}

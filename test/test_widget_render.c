﻿#include <LCUI.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/textview.h>
#include <LCUI/graph.h>
#include <LCUI/image.h>

int main(void)
{
	int ret;
	pd_canvas_t canvas;
	LCUI_Widget root, box, txt;
	pd_paint_context_t paint;
	pd_rect_t area = { 40, 40, 320, 240 };
	pd_color_t bgcolor = RGB(242, 249, 252);
	pd_color_t bdcolor = RGB(201, 230, 242);

	LCUI_Init();

	root = LCUIWidget_New(NULL);
	box = LCUIWidget_New(NULL);
	txt = LCUIWidget_New("textview");

	/* 创建一块灰色的画板 */
	pd_canvas_init(&canvas);
	pd_canvas_create(&canvas, 320, 240);
	pd_canvas_fill_rect(&canvas, RGB(240, 240, 240), NULL, FALSE);

	/* 初始化一个绘制实例，绘制区域为整个画板 */
	paint.with_alpha = FALSE;
	paint.rect.width = 320;
	paint.rect.height = 320;
	paint.rect.x = paint.rect.y = 0;
	pd_canvas_quote(&paint.canvas, &canvas, &area);

	/* 设定基本的样式和内容 */
	Widget_SetPadding(box, 20, 20, 20, 20);
	Widget_SetBorder(box, 1, SV_SOLID, bdcolor);
	Widget_SetStyle(box, key_background_color, bgcolor, color);
	TextView_SetTextW(txt, L"[size=24px]这是一段测试文本[/size]");
	Widget_Append(box, txt);
	Widget_Append(root, box);
	/* 标记需要更新样式 */
	Widget_UpdateStyle(txt, TRUE);
	Widget_UpdateStyle(box, TRUE);
	Widget_UpdateStyle(root, TRUE);
	/* 更新部件，此处的更新顺序必须是父级到子级 */
	Widget_Update(root);
	Widget_Update(box);
	Widget_Update(txt);

	/* 渲染部件 */
	Widget_Render(box, &paint);
	ret = LCUI_WritePNGFile("test_widget_render.png", &canvas);
	pd_canvas_free(&canvas);

	LCUI_Destroy();
	return ret;
}
